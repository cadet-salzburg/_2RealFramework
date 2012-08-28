#include "_2RealDatatypes.h"
#include "FaceCastBlock.h"
#include "FaceTrackingImpl.h"

#include <iostream>
#include <vector>

//TODO
#include "bench.h"
//-----

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

using namespace faceTracking::impl;

template<typename T>
inline const T clamp( const T &value, const T &minValue, const T &maxValue )
{
	return std::max( minValue, std::min( value, maxValue ) );
}

inline float PI()
{
	__asm fldpi;	
}

inline float toRad( float deg )
{
	static float s = PI() / 180.0f;
	return deg * s;
}

inline float toDeg( float rad )
{
	static float s = 180.0f / PI();
	return rad * s;
}

unsigned short avrgArea( const unsigned short *depthData, int x0, int y0, int x1, int y1, size_t stride )
{
	unsigned int cntr = 0;
	unsigned int sum = 0;

	int advance = stride - ( x1 - x0 );

	const unsigned short *ptr = depthData + stride * y0 + x0;
	for( int j = y0; j <= y1; j++, ptr += advance )
		for( int i = x0; i <= x1; i++, ptr++ )
		{
			if( *ptr )
			{
				sum += *ptr;
				cntr++;
			}
		}

	return ( cntr ? (unsigned short)( sum / cntr ) : 0 );
}

void inverseProjection( size_t size, const Vec3 *in, Vec3 *out, unsigned int width, unsigned int height, float f )
{
	float s = 1.0f / f;
	float a = - ( width * 0.5f ) * s;
	float b = - ( height * 0.5f ) * s;

	while( size-- )
	{
		float c = ( *in )[2] * s;

		( *out )[0] = ( ( *in )[0] * s + a ) * ( *in )[2];
		( *out )[1] = -( ( *in )[1] * s + b ) * ( *in )[2];
		( *out )[2] = ( *in )[2];

		in++;
		out++;
	}
}


FaceCastBlock::FaceCastBlock() : 
	Block(),
	m_frames( 0 ),
	m_timeAccu( 0.0 ),
	m_timeOverall( 0.0 ),
	m_resX( 0 ),
	m_resY( 0 ),
	m_avrgResX( 1 ),
	m_avrgResY( 1 ),
	m_extrapolationMinSizeFace( 0.0f, 0.0f ),
	m_cvImpl( new CVImpl() ),
	m_timeImpl( new TimeImpl() ),
	m_faceTrackingImpl( new FaceTrackingImpl( 4 ) )
{}

FaceCastBlock::~FaceCastBlock()
{
	if( m_cvImpl )
	{
		delete m_cvImpl;
		m_cvImpl = NULL;
	}

	if( m_faceTrackingImpl )
	{
		delete m_faceTrackingImpl;
		m_faceTrackingImpl = NULL;
	}

	if( m_timeImpl )
	{
		delete m_timeImpl;
		m_timeImpl = NULL;
	}
}

void FaceCastBlock::setup( BlockHandle &block )
{
	try
	{
		std::cout << "SETUP facefeaturesblock" << std::endl;

		m_frames = 0;
		m_timeAccu = 0.0;
		m_timeOverall = 0.0;

		m_Block = block;

		m_imageIn = m_Block.getInletHandle( "image_in" );
		m_depthIn = m_Block.getInletHandle( "depth_in" );

		m_resXIn = m_Block.getInletHandle( "res_x" );
		m_resYIn = m_Block.getInletHandle( "res_y" );

		m_fovVerIn = m_Block.getInletHandle( "fov_ver" );

		m_haarDetectionDownscaleIn = m_Block.getInletHandle( "haar_detection_downscale" );

		m_haarMinNeighboursFaceIn = m_Block.getInletHandle( "haar_min_neighbours_face" );

		m_haarMinSizeFaceIn = m_Block.getInletHandle( "haar_minSizeFace" );

		m_haarDoCannyPruningIn = m_Block.getInletHandle( "haar_do_canny_pruning" );
		m_haarScaleFactorIn = m_Block.getInletHandle( "haar_scalefactor" );

		m_equalizeHistogramIn = m_Block.getInletHandle( "equalize_histogram" );

		m_cascFileFaceIn = m_Block.getInletHandle( "cascadefile_face" );

		m_extrapolationDampingIn = m_Block.getInletHandle( "extrapolation_damping" );
		m_extrapolationCoherenceRiseIn = m_Block.getInletHandle( "extrapolation_coherence_rise" );

		m_affinityWeightPosIn = m_Block.getInletHandle( "affinity_weight_pos" );
		m_affinityWeightSizeIn = m_Block.getInletHandle( "affinity_weight_size" );
		m_coherenceWeightDirIn = m_Block.getInletHandle( "coherence_weight_dir" );
		m_coherenceWeightVelIn = m_Block.getInletHandle( "coherence_weight_vel" );
		m_coherenceWeightSizeIn = m_Block.getInletHandle( "coherence_weight_size" );
		m_coherenceToleranceDirIn = m_Block.getInletHandle( "coherence_tolerance_dir" );
		m_coherenceToleranceVelIn = m_Block.getInletHandle( "coherence_tolerance_vel" );
		m_coherenceToleranceSizeIn = m_Block.getInletHandle( "coherence_tolerance_size" );

		m_affinityThresholdIn = m_Block.getInletHandle( "affinity_threshold" );
		m_coherenceThresholdIn = m_Block.getInletHandle( "coherence_threshold" );
		m_discardThresholdIn = m_Block.getInletHandle( "discard_threshold" );

		m_extrapolationMinSizeFace = Vec2( 0.0f, 0.0f );

		m_faceOut = m_Block.getOutletHandle( "face_out" );

		const Image &image = m_imageIn.getReadableRef< Image >();

		m_cvImpl->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );

		m_cvImpl->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );

		m_cvImpl->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );

		m_cvImpl->doCannyPruning( m_haarDoCannyPruningIn.getReadableRef< bool >() );
		m_cvImpl->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );

		m_cvImpl->equalizeHist( m_equalizeHistogramIn.getReadableRef< bool >() );

		this->resizeCast();

		m_fov = toRad( m_fovVerIn.getReadableRef<double>() );

		if( !m_cvImpl->createImages( image ) )
			std::cerr << "warning: creating initial cv images failed" << std::endl;

		m_timeImpl->restart();
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void FaceCastBlock::update()
{
	try
	{
		Bench bench( "update facecast block" );

		const Image &image = m_imageIn.getReadableRef< Image >();

		if( m_haarDetectionDownscaleIn.hasChanged() )
			m_cvImpl->greyImgSmlScale( m_haarDetectionDownscaleIn.getReadableRef< double >() );

		if( m_cascFileFaceIn.hasChanged() )
			m_cvImpl->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );

		if( m_haarMinNeighboursFaceIn.hasChanged() )
			m_cvImpl->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );

		if( m_haarMinSizeFaceIn.hasChanged() )
			m_cvImpl->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );

		if( m_equalizeHistogramIn.hasChanged() )
			m_cvImpl->equalizeHist( m_equalizeHistogramIn.getReadableRef< bool >() );

		if( m_haarScaleFactorIn.hasChanged() )
			m_cvImpl->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );

		if( m_resXIn.hasChanged() || m_resYIn.hasChanged() )
			this->resizeCast();

		if( m_fovVerIn.hasChanged() )
			m_fov = toRad( m_fovVerIn.getReadableRef< double >() );

		if( m_imageIn.hasUpdated() )
		{
			double dt = m_timeImpl->milliSeconds();

			m_timeAccu += dt;
			m_timeOverall += dt;

			if( m_timeAccu > 1.0 )
			{
				m_frames = 0;
				m_timeAccu = 0.0f;
			}

			m_timeImpl->restart();

			m_frames++;

			FeatureVector faces;

			m_cvImpl->detectFaces( image, faces );
			m_faceTrackingImpl->track( faces,
				m_affinityWeightPosIn.getReadableRef<double>(), 
				m_affinityWeightSizeIn.getReadableRef<double>(),
				m_affinityThresholdIn.getReadableRef<double>(),
				m_coherenceWeightDirIn.getReadableRef<double>(), 
				m_coherenceWeightVelIn.getReadableRef<double>(), 
				m_coherenceWeightSizeIn.getReadableRef<double>(),
				m_coherenceThresholdIn.getReadableRef<double>(),
				m_coherenceToleranceDirIn.getReadableRef<double>() / image.getWidth(),
				m_coherenceToleranceVelIn.getReadableRef<double>() / image.getWidth(),
				m_coherenceToleranceSizeIn.getReadableRef<double>() / image.getWidth(),
				m_discardThresholdIn.getReadableRef<double>(),
				m_extrapolationDampingIn.getReadableRef<double>(), 
				m_extrapolationCoherenceRiseIn.getReadableRef<double>(),
				m_extrapolationMinSizeFace,
				m_timeOverall );

			if( m_faceTrackingImpl->trackingInfoList().size() )
			{
				std::cout << "tracking " << m_faceTrackingImpl->trackingInfoList().size() << " faces" << std::endl;

				const Image &depthImg = m_depthIn.getReadableRef<Image>();

				if( depthImg.getData() )
				{
					std::vector< FaceCast > &faces = m_faceOut.getWriteableRef< std::vector< FaceCast > >();
					faces.clear();

					for( TrackingInfoList::iterator itT = m_faceTrackingImpl->trackingInfoList().begin(); itT != m_faceTrackingImpl->trackingInfoList().end(); ++itT )
					{
						faces.push_back( FaceCast( itT->getUserID(), m_resXIn.getReadableRef<unsigned int>(), m_resYIn.getReadableRef<unsigned int>() ) );

						FaceCast &cast = faces.back();

						if( !this->makeDepthCast( depthImg, itT->getFaceTrajectory().getLastRegion(), cast ) )
							faces.pop_back();
					}

					std::cout << "sending " << faces.size() << " faces" << std::endl;
				}
				else
				{
					std::cout << "no depth data available" << std::endl;
					m_faceOut.discard();
				}
			}
			else
				m_faceOut.discard();
		}
		else
		{
			m_faceOut.discard();
		}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void FaceCastBlock::shutdown() {}

bool FaceCastBlock::makeDepthCast( const _2Real::Image &depthImg, const _2Real::Space2D &area, _2Real::FaceCast &cast )
{
	if( !m_avrgArray.size() )
	{
		std::cerr << "average array is of size 0" << std::endl;
		return false;
	}

	if( !depthImg.getData() || ( depthImg.getWidth() * depthImg.getHeight() ) == 0 )
	{
		std::cerr << "depth image is invalid" << std::endl;
		return false;
	}

	float f = depthImg.getWidth() / ( 2.0 * tan( m_fov * 0.5 ) );

	float x0 = area.getP0()[0] * depthImg.getWidth();
	float y0 = area.getP0()[1] * depthImg.getHeight();

	float width = ( area.getP1()[0] - area.getP0()[0]  );
	float height = ( area.getP1()[1] - area.getP0()[1] );

	if( width <= std::numeric_limits<float>::epsilon() || height < std::numeric_limits<float>::epsilon() )
	{
		std::cerr << "area not valid" << std::endl;
		return false;
	}

	float stepSizeX = width * depthImg.getWidth() / m_avrgResX;
	float stepSizeY = height * depthImg.getHeight() / m_avrgResY;

	if( stepSizeX < 1.0f || stepSizeY < 1.0f )
	{
		std::cerr << "face rect too small for downsampling" << std::endl;
		return false;
	}

	float u0 = 0.0f;
	float u1 = 0.0f;
	float v0 = 0.0f;
	float v1 = 0.0f;

	unsigned short *avrgPtr = &( this->m_avrgArray[0] );

	float v = y0;
	for( int j = 0; j < m_avrgResY; j++, v += stepSizeY )
	{
		float u = x0;

		for( int i = 0; i < m_avrgResX; i++, u += stepSizeX )
		{
			u0 = clamp( u, 0.0f, depthImg.getWidth() - 1.0f );
			u1 = clamp( u + stepSizeX, 0.0f, depthImg.getWidth() - 1.0f );

			v0 = clamp( v, 0.0f, depthImg.getHeight() - 1.0f );
			v1 = clamp( v + stepSizeY, 0.0f, depthImg.getHeight() - 1.0f );

			*( avrgPtr++ ) = avrgArea( (unsigned short*)depthImg.getData(), u0, v0, u1, v1, depthImg.getWidth() );
		}
	}

	struct NormalCornerInfo
	{
		unsigned int count;
		unsigned int indices[4];
	};

	VertexList normalCorners;
	normalCorners.resize( m_resX * m_resY * 4 );

	std::list<NormalCornerInfo> infoList;

	int indexCntr = 0;
	int cornerIndexCntr = 0;
	int vertexCntr = 0;

	VertexList &vertices = cast.getVertices();
	NormalList &normals = cast.getNormals();
	IndexList &indices = cast.getIndices();

	VertexList::iterator vIt = vertices.begin();
	VertexList::iterator cIt = normalCorners.begin();
	IndexList::iterator iIt = indices.begin();
	avrgPtr = &( m_avrgArray[0] );
	for( int j = 0; j < m_resY; j++, avrgPtr++ )
		for( int i = 0; i < m_resX; i++, avrgPtr++, indexCntr++ )
		{
			NormalCornerInfo info;
			info.count = 0;
			info.indices[0] = ~0x00;
			info.indices[1] = ~0x00;
			info.indices[2] = ~0x00;
			info.indices[3] = ~0x00;

			int sum = 0;

			if( *avrgPtr )
			{
				sum += *avrgPtr;

				info.count++;
				info.indices[0] = cornerIndexCntr++;
				( *cIt )[0] = x0 + i * stepSizeX;
				( *cIt )[1] = y0 + j * stepSizeY;
				( *cIt )[2] = *avrgPtr;
				cIt++;
			}

			if( *( avrgPtr + 1 ) )
			{
				sum += *( avrgPtr + 1 );

				info.count++;
				info.indices[1] = cornerIndexCntr++;
				( *cIt )[0] = x0 + ( i + 1.0f ) * stepSizeX;
				( *cIt )[1] = y0 + j * stepSizeY;
				( *cIt )[2] = *( avrgPtr + 1 );
				cIt++;
			}

			if( *( avrgPtr + m_avrgResX ) )
			{
				sum += *( avrgPtr + m_avrgResX );

				info.count++;
				info.indices[2] = cornerIndexCntr++;
				( *cIt )[0] = x0 + i * stepSizeX;
				( *cIt )[1] = y0 + ( j + 1.0f ) * stepSizeY;
				( *cIt )[2] = *( avrgPtr + m_avrgResX );
				cIt++;
			}

			if( *( avrgPtr + m_avrgResX + 1 ) )
			{
				sum += *( avrgPtr + m_avrgResX + 1 );

				info.count++;
				info.indices[3] = cornerIndexCntr++;
				( *cIt )[0] = x0 + ( i + 1.0f ) * stepSizeX;
				( *cIt )[1] = y0 + ( j + 1.0f ) * stepSizeY;
				( *cIt )[2] = *( avrgPtr + m_avrgResX + 1 );
				cIt++;
			}

			if( sum && info.count > 2 )
			{
				( *vIt )[0] = x0 + ( i + 0.5f ) * stepSizeX;
				( *vIt )[1] = y0 + ( j + 0.5f ) * stepSizeY;
				( *vIt )[2] = (float)sum / info.count;

				infoList.push_back( info );

				vIt++;
				vertexCntr++;

				*( iIt++ ) = indexCntr;
			}
		}

	if( vertexCntr )
		inverseProjection( vertexCntr, &( vertices[0] ), &( vertices[0] ), depthImg.getWidth(), depthImg.getHeight(), f );

	if( cornerIndexCntr )
		inverseProjection( cornerIndexCntr, &( normalCorners[0] ), &( normalCorners[0] ), depthImg.getWidth(), depthImg.getHeight(), f );

	NormalList::iterator nIt = normals.begin();
	for( std::list<NormalCornerInfo>::const_iterator it = infoList.begin(); it != infoList.end(); ++it, nIt++ )
	{
		Vec3 d0( 0.0f, 0.0f, 0.0f );
		Vec3 d1( 0.0f, 0.0f, 0.0f );

		if( it->count == 3 )
		{
			if( it->indices[0] == ~0x00 )
			{
				Vec3 c1( normalCorners[it->indices[1]] );
				Vec3 c2( normalCorners[it->indices[2]] );
				Vec3 c3( normalCorners[it->indices[3]] );

				d0 = Vec3( c3[0] - c2[0], c3[1] - c2[1], c3[2] - c2[2] );
				d1 = Vec3( c1[0] - c2[0], c1[1] - c2[1], c1[2] - c2[2] );
			}
			else if( it->indices[1] == ~0x00 )
			{
				Vec3 c0( normalCorners[it->indices[0]] );
				Vec3 c2( normalCorners[it->indices[2]] );
				Vec3 c3( normalCorners[it->indices[3]] );

				d0 = Vec3( c2[0] - c0[0], c2[1] - c0[1], c2[2] - c0[2] );
				d1 = Vec3( c3[0] - c0[0], c3[1] - c0[1], c3[2] - c0[2] );
			}
			else if( it->indices[2] == ~0x00 )
			{
				Vec3 c0( normalCorners[it->indices[0]] );
				Vec3 c1( normalCorners[it->indices[1]] );
				Vec3 c3( normalCorners[it->indices[3]] );

				d0 = Vec3( c3[0] - c0[0], c3[1] - c0[1], c3[2] - c0[2] );
				d1 = Vec3( c1[0] - c0[0], c1[1] - c0[1], c1[2] - c0[2] );
			}
			else if( it->indices[3] == ~0x00 )
			{
				Vec3 c0( normalCorners[it->indices[0]] );
				Vec3 c1( normalCorners[it->indices[1]] );
				Vec3 c2( normalCorners[it->indices[2]] );

				d0 = Vec3( c2[0] - c0[0], c2[1] - c0[1], c2[2] - c0[2] );
				d1 = Vec3( c1[0] - c0[0], c1[1] - c0[1], c1[2] - c0[2] );
			}
		}
		else if( it->count == 4 )
		{
			Vec3 c0( normalCorners[it->indices[0]] );
			Vec3 c1( normalCorners[it->indices[1]] );
			Vec3 c2( normalCorners[it->indices[2]] );
			Vec3 c3( normalCorners[it->indices[3]] );

			d0 = Vec3( c3[0] - c0[0], c3[1] - c0[1], c3[2] - c0[2] );
			d1 = Vec3( c1[0] - c2[0], c1[1] - c2[1], c1[2] - c2[2] );
		}

		*nIt = d1.cross( d0 ).normalized();
	}

	return true;
}

void FaceCastBlock::resizeCast()
{
	m_resX = m_resXIn.getReadableRef<unsigned int>();
	m_resY = m_resYIn.getReadableRef<unsigned int>();

	m_avrgResX = m_resX + 1;
	m_avrgResY = m_resY + 1;

	m_avrgArray.resize( m_avrgResX * m_avrgResY );
}