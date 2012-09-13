#include "_2RealDatatypes.h"

#include "FaceCastBlock.h"

#include "FaceTracking.h"
#include "FaceDetection.h"
#include "StopWatch.h"

#include <iostream>
#include <vector>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

template<typename T>
inline const T clamp( const T &value, const T &minValue, const T &maxValue )
{
	return std::max( minValue, std::min( value, maxValue ) );
}

inline double PI()
{
	__asm fldpi;	
}

inline double toRad( double deg )
{
	static double s = PI() / 180.0;
	return deg * s;
}

inline double toDeg( double rad )
{
	static double s = 180.0 / PI();
	return rad * s;
}

unsigned short avrgArea( const unsigned short *depthData, int x0, int y0, int x1, int y1, size_t stride, unsigned short maxValue )
{
	unsigned int cntr = 0;
	unsigned int sum = 0;

	int advance = stride - ( x1 - x0 );

	const unsigned short *ptr = depthData + stride * y0 + x0;
	for( int j = y0; j <= y1; j++, ptr += advance )
		for( int i = x0; i <= x1; i++, ptr++ )
		{
			if( *ptr && *ptr < maxValue )
			{
				sum += *ptr;
				cntr++;
			}
		}

	return ( cntr ? (unsigned short)( sum / cntr ) : 0 );
}

void inverseProjection( size_t size, const Vec3 *in, Vec3 *out, unsigned int width, unsigned int height, double f )
{
	double s = 1.0 / f;
	double a = - ( width * 0.5 ) * s;
	double b = - ( height * 0.5 ) * s;

	while( size-- )
	{
		double c = ( *in )[2] * s;

		( *out )[0] = (Vec2::Scalar)( -( ( *in )[0] * s + a ) * ( *in )[2] );
		( *out )[1] = (Vec2::Scalar)( -( ( *in )[1] * s + b ) * ( *in )[2] );
		( *out )[2] = (Vec2::Scalar)( ( *in )[2] );

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
	m_extrapolationMinSizeFace( 0.0, 0.0 ),
	m_faceDetection( new FaceDetection() ),
	m_stopWatch( new StopWatch() ),
	m_faceTracking( new FaceTracking( 4 ) )
{}

FaceCastBlock::~FaceCastBlock()
{
	if( m_faceDetection )
	{
		delete m_faceDetection;
		m_faceDetection = NULL;
	}

	if( m_faceTracking )
	{
		delete m_faceTracking;
		m_faceTracking = NULL;
	}

	if( m_stopWatch )
	{
		delete m_stopWatch;
		m_stopWatch = NULL;
	}
}

void FaceCastBlock::setup( BlockHandle &block )
{
	try
	{
		m_frames = 0;
		m_timeAccu = 0.0;
		m_timeOverall = 0.0;

		m_Block = block;

		m_imageIn = m_Block.getInletHandle( "ImageIn" );
		m_depthIn = m_Block.getInletHandle( "DepthIn" );

		m_resXIn = m_Block.getInletHandle( "ResX" );
		m_resYIn = m_Block.getInletHandle( "ResY" );

		m_faceScaleXIn = m_Block.getInletHandle( "FaceScaleX" );
		m_faceScaleYIn = m_Block.getInletHandle( "FaceScaleY" );

		m_cutoffIn = m_Block.getInletHandle( "DepthCutoff" );

		m_fovVerIn = m_Block.getInletHandle( "FovVer" );

		m_haarDetectionDownscaleIn = m_Block.getInletHandle( "HaarDetectionDownscale" );

		m_haarMinNeighboursFaceIn = m_Block.getInletHandle( "HaarMinNeighboursFace" );

		m_haarMinSizeFaceIn = m_Block.getInletHandle( "HaarMinSizeFace" );

		m_haarDoCannyPruningIn = m_Block.getInletHandle( "HaarDoCannyPruning" );
		m_haarScaleFactorIn = m_Block.getInletHandle( "HaarScalefactor" );

		m_equalizeHistogramIn = m_Block.getInletHandle( "EqualizeHistogram" );

		m_cascFileFaceIn = m_Block.getInletHandle( "CascadefileFace" );

		m_extrapolationDampingIn = m_Block.getInletHandle( "ExtrapolationDamping" );
		m_extrapolationCoherenceRiseIn = m_Block.getInletHandle( "ExtrapolationCoherenceRise" );

		m_affinityWeightPosIn = m_Block.getInletHandle( "AffinityWeightPos" );
		m_affinityWeightSizeIn = m_Block.getInletHandle( "AffinityWeightSize" );
		m_coherenceWeightDirIn = m_Block.getInletHandle( "CoherenceWeightDir" );
		m_coherenceWeightVelIn = m_Block.getInletHandle( "CoherenceWeightVel" );
		m_coherenceWeightSizeIn = m_Block.getInletHandle( "CoherenceWeightSize" );
		m_coherenceToleranceDirIn = m_Block.getInletHandle( "CoherenceToleranceDir" );
		m_coherenceToleranceVelIn = m_Block.getInletHandle( "CoherenceToleranceVel" );
		m_coherenceToleranceSizeIn = m_Block.getInletHandle( "CoherenceToleranceSize" );

		m_affinityThresholdIn = m_Block.getInletHandle( "AffinityThreshold" );
		m_coherenceThresholdIn = m_Block.getInletHandle( "CoherenceThreshold" );
		m_discardThresholdIn = m_Block.getInletHandle( "DiscardThreshold" );

		m_extrapolationMinSizeFace = Vec2( 0.0, 0.0 );

		m_faceOut = m_Block.getOutletHandle( "FaceOut" );

		const Image &image = m_imageIn.getReadableRef< Image >();

		m_faceDetection->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );

		m_faceDetection->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );

		m_faceDetection->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );

		m_faceDetection->doCannyPruning( m_haarDoCannyPruningIn.getReadableRef< bool >() );
		m_faceDetection->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );

		m_faceDetection->equalizeHist( m_equalizeHistogramIn.getReadableRef< bool >() );

		this->resizeCast();

		m_fov = toRad( m_fovVerIn.getReadableRef<double>() );

		if( !m_faceDetection->createImages( image ) )
			std::cerr << "warning: creating initial cv images failed" << std::endl;

		m_stopWatch->restart();
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
		const Image &image = m_imageIn.getReadableRef< Image >();

		if( m_haarDetectionDownscaleIn.hasChanged() )
			m_faceDetection->greyImgSmlScale( m_haarDetectionDownscaleIn.getReadableRef< double >() );

		if( m_cascFileFaceIn.hasChanged() )
			m_faceDetection->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );

		if( m_haarMinNeighboursFaceIn.hasChanged() )
			m_faceDetection->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );

		if( m_haarMinSizeFaceIn.hasChanged() )
			m_faceDetection->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );

		if( m_equalizeHistogramIn.hasChanged() )
			m_faceDetection->equalizeHist( m_equalizeHistogramIn.getReadableRef< bool >() );

		if( m_haarScaleFactorIn.hasChanged() )
			m_faceDetection->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );

		if( m_resXIn.hasChanged() || m_resYIn.hasChanged() )
			this->resizeCast();

		if( m_fovVerIn.hasChanged() )
			m_fov = toRad( m_fovVerIn.getReadableRef< double >() );

		if( m_imageIn.hasUpdated() )
		{
			double dt = m_stopWatch->milliSeconds();

			m_timeAccu += dt;
			m_timeOverall += dt;

			if( m_timeAccu > 1.0 )
			{
				m_frames = 0;
				m_timeAccu = 0.0;
			}

			m_stopWatch->restart();

			m_frames++;

			FeatureVector faces;

			m_faceDetection->detectFaces( image, faces );
			m_faceTracking->track( faces,
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

			if( m_faceTracking->trackingInfoList().size() )
			{
				const Image &depthImg = m_depthIn.getReadableRef<Image>();

				if( depthImg.getData() )
				{
					std::vector< FaceCast > &faces = m_faceOut.getWriteableRef< std::vector< FaceCast > >();
					faces.clear();

					for( TrackingInfoList::iterator itT = m_faceTracking->trackingInfoList().begin(); itT != m_faceTracking->trackingInfoList().end(); ++itT )
					{
						Space2D faceRegion = itT->getFaceTrajectory().getLastRegion();
						Vec2 center = ( faceRegion.getP0() + faceRegion.getP1() ) * 0.5;
						Vec2 size = faceRegion.getP1() - faceRegion.getP0();

						size[0] *= (Vec2::Scalar)( m_faceScaleXIn.getReadableRef<double>() );
						size[1] *= (Vec2::Scalar)( m_faceScaleYIn.getReadableRef<double>() );

						faceRegion.set( center - size * 0.5, center + size * 0.5 );

						faces.push_back( FaceCast( itT->getUserID(), m_resXIn.getReadableRef<unsigned int>(), m_resYIn.getReadableRef<unsigned int>(), faceRegion ) );
						FaceCast &cast = faces.back();

						if( !this->makeDepthCast( depthImg, faceRegion, cast, m_cutoffIn.getReadableRef<double>() ) )
							faces.pop_back();
					}
				}
				else
				{
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

bool FaceCastBlock::makeDepthCast( const _2Real::Image &depthImg, const _2Real::Space2D &area, _2Real::FaceCast &cast, double depthCutoff )
{
	if( !m_avrgArray.size() )
		throw std::runtime_error( "average array is of size 0" );

	if( !depthImg.getData() || ( depthImg.getWidth() * depthImg.getHeight() ) == 0 )
		throw std::runtime_error( "depth image is invalid" );

	double f = depthImg.getHeight() / ( 2.0 * tan( m_fov * 0.5 ) );

	double x0 = area.getP0()[0] * depthImg.getWidth();
	double y0 = area.getP0()[1] * depthImg.getHeight();

	double width = ( area.getP1()[0] - area.getP0()[0]  );
	double height = ( area.getP1()[1] - area.getP0()[1] );

	if( width <= std::numeric_limits<double>::epsilon() || height < std::numeric_limits<double>::epsilon() )
	{
		std::cerr << "FaceCastBlock: face area invalid" << std::endl;
		return false;
	}

	double stepSizeX = width * depthImg.getWidth() / m_avrgResX;
	double stepSizeY = height * depthImg.getHeight() / m_avrgResY;

	if( stepSizeX < 1.0 || stepSizeY < 1.0 )
	{
		std::cerr << "face rect too small for downsampling" << std::endl;
		return false;
	}

	double u0 = clamp( x0, 0.0, depthImg.getWidth() - 1.0 );
	double u1 = clamp( x0 + width * depthImg.getWidth(), 0.0, depthImg.getWidth() - 1.0 );
	double v0 = clamp( y0, 0.0, depthImg.getHeight() - 1.0 );
	double v1 = clamp( y0 + height * depthImg.getHeight(), 0.0, depthImg.getHeight() - 1.0 );

	unsigned short *avrgPtr = &( m_avrgArray[0] );

	unsigned short minDepth = ~0x00;
	for( int j = (int)v0; j < (int)v1; j++ )
	{
		unsigned short *depthPtr = (unsigned short*)depthImg.getData() + j * depthImg.getWidth() + (int)u0;
		for( int i = (int)u0; i < (int)u1; i++, depthPtr++ )
			if( *depthPtr && minDepth > *depthPtr )
				minDepth = *depthPtr;
	}

	unsigned short cutoff = (unsigned short)( minDepth + depthCutoff * 1000.0 );

	double v = y0;
	for( int j = 0; j < m_avrgResY; j++, v += stepSizeY )
	{
		double u = x0;

		for( int i = 0; i < m_avrgResX; i++, u += stepSizeX )
		{
			u0 = clamp( u, 0.0, depthImg.getWidth() - 1.0 );
			u1 = clamp( u + stepSizeX, 0.0, depthImg.getWidth() - 1.0 );

			v0 = clamp( v, 0.0, depthImg.getHeight() - 1.0 );
			v1 = clamp( v + stepSizeY, 0.0, depthImg.getHeight() - 1.0 );

			*( avrgPtr++ ) = avrgArea( (unsigned short*)depthImg.getData(), (int)u0, (int)v0, (int)u1, (int)v1, depthImg.getWidth(), cutoff );
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
				( *cIt )[0] = (Vec3::Scalar)( x0 + i * stepSizeX );
				( *cIt )[1] = (Vec3::Scalar)( y0 + j * stepSizeY );
				( *cIt )[2] = (Vec3::Scalar)( *avrgPtr );
				cIt++;
			}

			if( *( avrgPtr + 1 ) )
			{
				sum += *( avrgPtr + 1 );

				info.count++;
				info.indices[1] = cornerIndexCntr++;
				( *cIt )[0] = (Vec3::Scalar)( x0 + ( i + 1.0 ) * stepSizeX );
				( *cIt )[1] = (Vec3::Scalar)( y0 + j * stepSizeY );
				( *cIt )[2] = (Vec3::Scalar)( *( avrgPtr + 1 ) );
				cIt++;
			}

			if( *( avrgPtr + m_avrgResX ) )
			{
				sum += *( avrgPtr + m_avrgResX );

				info.count++;
				info.indices[2] = cornerIndexCntr++;
				( *cIt )[0] = (Vec3::Scalar)( x0 + i * stepSizeX );
				( *cIt )[1] = (Vec3::Scalar)( y0 + ( j + 1.0 ) * stepSizeY );
				( *cIt )[2] = (Vec3::Scalar)( *( avrgPtr + m_avrgResX ) );
				cIt++;
			}

			if( *( avrgPtr + m_avrgResX + 1 ) )
			{
				sum += *( avrgPtr + m_avrgResX + 1 );

				info.count++;
				info.indices[3] = cornerIndexCntr++;
				( *cIt )[0] = (Vec3::Scalar)( x0 + ( i + 1.0 ) * stepSizeX );
				( *cIt )[1] = (Vec3::Scalar)( y0 + ( j + 1.0 ) * stepSizeY );
				( *cIt )[2] = (Vec3::Scalar)( *( avrgPtr + m_avrgResX + 1 ) );
				cIt++;
			}

			if( sum && info.count > 2 )
			{
				( *vIt )[0] = (Vec3::Scalar)( x0 + ( i + 0.5 ) * stepSizeX );
				( *vIt )[1] = (Vec3::Scalar)( y0 + ( j + 0.5 ) * stepSizeY );
				( *vIt )[2] = (Vec3::Scalar)( (double)sum / info.count );

				infoList.push_back( info );

				vIt++;
				vertexCntr++;

				*( iIt++ ) = indexCntr;
			}
		}

	std::fill( iIt, indices.end(), ~0x00 );

	if( vertexCntr )
	{
		inverseProjection( vertexCntr, &( vertices[0] ), &( vertices[0] ), depthImg.getWidth(), depthImg.getHeight(), f );

		vIt = vertices.begin();
		for( int i = 0; i < vertexCntr; i++, ++vIt )
			*( vIt ) *= (Vec3::Scalar)( 0.001 );
	}

	if( cornerIndexCntr )
		inverseProjection( cornerIndexCntr, &( normalCorners[0] ), &( normalCorners[0] ), depthImg.getWidth(), depthImg.getHeight(), f );

	NormalList::iterator nIt = normals.begin();
	for( std::list<NormalCornerInfo>::const_iterator it = infoList.begin(); it != infoList.end(); ++it, nIt++ )
	{
		Vec3 d0( 0.0, 0.0, 0.0 );
		Vec3 d1( 0.0, 0.0, 0.0 );

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

		*nIt = d0.cross( d1 ).normalized();
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