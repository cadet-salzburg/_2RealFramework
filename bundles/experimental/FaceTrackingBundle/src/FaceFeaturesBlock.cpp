#include "_2RealDatatypes.h"

#include "FaceFeaturesBlock.h"
#include "FaceTrackingImpl.h"

#include "_2RealApplication.h"

#include <iostream>

//TODO
#include "bench.h"
//-----

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;


using namespace faceTracking::impl;

FaceFeaturesBlock::FaceFeaturesBlock() : 
	Block(), 
	m_frames( 0 ),
	m_timeAccu( 0.0 ),
	m_timeOverall( 0.0 ),
	m_extrapolationMinSizeFace( 0.0f, 0.0f ),
	m_extrapolationMinSizeEye( 0.0f, 0.0f ),
	m_extrapolationMinSizeNose( 0.0f, 0.0f ),
	m_extrapolationMinSizeMouth( 0.0f, 0.0f ),
	m_cvImpl( new CVImpl() ),
	m_timeImpl( new TimeImpl() ),
	m_faceTrackingImpl( new FaceTrackingImpl( 4 ) )
{}

FaceFeaturesBlock::~FaceFeaturesBlock()
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

void FaceFeaturesBlock::setup( BlockHandle &block )
{
	try
	{
		std::cout << "SETUP facefeaturesblock" << std::endl;

		m_frames = 0;
		m_timeAccu = 0.0;
		m_timeOverall = 0.0;

		m_Block = block;

		m_imageIn = m_Block.getInletHandle( "image_in" );

		m_haarDetectionDownscaleIn = m_Block.getInletHandle( "haar_detection_downscale" );

		m_useEyesIn = m_Block.getInletHandle( "use_eyes" );
		m_useNoseIn = m_Block.getInletHandle( "use_nose" );
		m_useMouthIn = m_Block.getInletHandle( "use_mouth" );

		m_haarMinNeighboursFaceIn = m_Block.getInletHandle( "haar_min_neighbours_face" );
		m_haarMinNeighboursEyesIn = m_Block.getInletHandle( "haar_min_neighbours_eyes" );
		m_haarMinNeighboursNoseIn = m_Block.getInletHandle( "haar_min_neighbours_nose" );
		m_haarMinNeighboursMouthIn = m_Block.getInletHandle( "haar_min_neighbours_mouth" );

		m_haarMinSizeFaceIn = m_Block.getInletHandle( "haar_minSizeFace" );
		m_haarMinSizeEyesIn = m_Block.getInletHandle( "haar_min_size_eyes" );
		m_haarMinSizeNoseIn = m_Block.getInletHandle( "haar_min_size_nose" );
		m_haarMinSizeMouthIn = m_Block.getInletHandle( "haar_min_size_mouth" );

		m_haarDoCannyPruningIn = m_Block.getInletHandle( "haar_do_canny_pruning" );
		m_haarScaleFactorIn = m_Block.getInletHandle( "haar_scalefactor" );

		m_equalizeHistogramIn = m_Block.getInletHandle( "equalize_histogram" );

		m_cascFileFaceIn = m_Block.getInletHandle( "cascadefile_face" );
		m_cascFileEyesIn = m_Block.getInletHandle( "cascadefile_eyes" );
		m_cascFileNoseIn = m_Block.getInletHandle( "cascadefile_nose" );
		m_cascFileMouthIn = m_Block.getInletHandle( "cascadefile_mouth" );

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
		m_extrapolationMinSizeEye = Vec2( 0.0f, 0.0f );
		m_extrapolationMinSizeNose = Vec2( 0.0f, 0.0f );
		m_extrapolationMinSizeMouth = Vec2( 0.0f, 0.0f );

		m_faceOut = m_Block.getOutletHandle( "face_out" );

		const Image &image = m_imageIn.getReadableRef< Image >();

		m_cvImpl->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );
		m_cvImpl->loadEyesCascade( m_cascFileEyesIn.getReadableRef< std::string >() );
		m_cvImpl->loadNoseCascade( m_cascFileNoseIn.getReadableRef< std::string >() );
		m_cvImpl->loadMouthCascade( m_cascFileMouthIn.getReadableRef< std::string >() );

		m_cvImpl->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );
		m_cvImpl->minNeighboursEyes( m_haarMinNeighboursEyesIn.getReadableRef< unsigned int >() );
		m_cvImpl->minNeighboursNose( m_haarMinNeighboursNoseIn.getReadableRef< unsigned int >() );
		m_cvImpl->minNeighboursMouth( m_haarMinNeighboursMouthIn.getReadableRef< unsigned int >() );

		m_cvImpl->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );
		m_cvImpl->minSizeEyes( m_haarMinSizeEyesIn.getReadableRef< Vec2 >() );
		m_cvImpl->minSizeNose( m_haarMinSizeNoseIn.getReadableRef< Vec2 >() );
		m_cvImpl->minSizeMouth( m_haarMinSizeMouthIn.getReadableRef< Vec2 >() );

		m_cvImpl->doCannyPruning( m_haarDoCannyPruningIn.getReadableRef< bool >() );
		m_cvImpl->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );

		m_cvImpl->equalizeHist( m_equalizeHistogramIn.getReadableRef< bool >() );

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

void FaceFeaturesBlock::update()
{
	try
	{
		Bench bench( "update facefeatures block" );

		const Image &image = m_imageIn.getReadableRef< Image >();

		if( m_haarDetectionDownscaleIn.hasChanged() )
			m_cvImpl->greyImgSmlScale( m_haarDetectionDownscaleIn.getReadableRef< double >() );

		if( m_cascFileFaceIn.hasChanged() )
			m_cvImpl->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );
		if( m_cascFileEyesIn.hasChanged() )
			m_cvImpl->loadEyesCascade( m_cascFileEyesIn.getReadableRef< std::string >() );
		if( m_cascFileNoseIn.hasChanged() )
			m_cvImpl->loadNoseCascade( m_cascFileNoseIn.getReadableRef< std::string >() );
		if( m_cascFileMouthIn.hasChanged() )
			m_cvImpl->loadMouthCascade( m_cascFileMouthIn.getReadableRef< std::string >() );

		if( m_haarMinNeighboursFaceIn.hasChanged() )
			m_cvImpl->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );
		if( m_haarMinNeighboursEyesIn.hasChanged() )
			m_cvImpl->minNeighboursEyes( m_haarMinNeighboursEyesIn.getReadableRef< unsigned int >() );
		if( m_haarMinNeighboursNoseIn.hasChanged() )
			m_cvImpl->minNeighboursNose( m_haarMinNeighboursNoseIn.getReadableRef< unsigned int >() );
		if( m_haarMinNeighboursMouthIn.hasChanged() )
			m_cvImpl->minNeighboursMouth( m_haarMinNeighboursMouthIn.getReadableRef< unsigned int >() );

		if( m_haarMinSizeFaceIn.hasChanged() )
			m_cvImpl->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );
		if( m_haarMinSizeEyesIn.hasChanged() )
			m_cvImpl->minSizeEyes( m_haarMinSizeEyesIn.getReadableRef< Vec2 >() );
		if( m_haarMinSizeNoseIn.hasChanged() )
			m_cvImpl->minSizeNose( m_haarMinSizeNoseIn.getReadableRef< Vec2 >() );
		if( m_haarMinSizeMouthIn.hasChanged() )
			m_cvImpl->minSizeMouth( m_haarMinSizeMouthIn.getReadableRef< Vec2 >() );

		if( m_equalizeHistogramIn.hasChanged() )
			m_cvImpl->equalizeHist( m_equalizeHistogramIn.getReadableRef< bool >() );

		if( m_haarScaleFactorIn.hasChanged() )
			m_cvImpl->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );

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

				for( TrackingInfoList::iterator itT = m_faceTrackingImpl->trackingInfoList().begin(); itT != m_faceTrackingImpl->trackingInfoList().end(); ++itT )
				{
					if( itT->getFaceTrajectory().getEntryCount() )
					{
						FeatureVector eyesCandidates;
						FeatureVector noseCandidates;
						FeatureVector mouthCandidates;

						m_cvImpl->detectFeatures( image, itT->getFaceTrajectory().getLastRegion(), 
							eyesCandidates, noseCandidates, mouthCandidates, 
							m_useEyesIn.getReadableRef< bool >(),
							m_useNoseIn.getReadableRef< bool >(),
							m_useMouthIn.getReadableRef< bool >() );

						itT->addFromFeatureCandidates( eyesCandidates, noseCandidates, mouthCandidates, m_timeOverall, m_affinityWeightPosIn.getReadableRef<double>(), m_affinityWeightSizeIn.getReadableRef<double>() );
					}
				}

				std::vector< FaceDesc > &faces = m_faceOut.getWriteableRef< std::vector< FaceDesc > >();
				faces.clear();

				for( TrackingInfoList::iterator itT = m_faceTrackingImpl->trackingInfoList().begin(); itT != m_faceTrackingImpl->trackingInfoList().end(); ++itT )
				{
					FaceDesc desc( itT->getUserID(), itT->getFaceTrajectory().getLastRegion() );

					if( m_useEyesIn.getReadableRef< bool >() )
					{
						if( itT->getEyeLeftTrajectory().getEntryCount() )
						{
							desc.eyeLeft( FeatureDesc( itT->getEyeLeftTrajectory().getLastRegion() ) );
							std::cout << "added left eye" << std::endl;
						}
						if( itT->getEyeRightTrajectory().getEntryCount() )
						{
							desc.eyeRight( FeatureDesc( itT->getEyeRightTrajectory().getLastRegion() ) );
							std::cout << "added right eye" << std::endl;
						}
					}
					if( m_useNoseIn.getReadableRef< bool >() && itT->getNoseTrajectory().getEntryCount() )
					{
						desc.nose( FeatureDesc( itT->getNoseTrajectory().getLastRegion() ) );
						std::cout << "added nose" << std::endl;
					}
					if( m_useMouthIn.getReadableRef< bool >() && itT->getNoseTrajectory().getEntryCount() )
					{
						desc.mouth( FeatureDesc( itT->getMouthTrajectory().getLastRegion() ) );
						std::cout << "added mouth" << std::endl;
					}

					faces.push_back( desc );
				}

				std::cout << "sending " << faces.size() << " faces" << std::endl;
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

void FaceFeaturesBlock::shutdown() {}