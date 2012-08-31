#include "_2RealDatatypes.h"

#include "FaceFeaturesBlock.h"

#include "FaceTracking.h"
#include "FaceDetection.h"
#include "StopWatch.h"

#include "_2RealApplication.h"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;


FaceFeaturesBlock::FaceFeaturesBlock() : 
	Block(), 
	m_frames( 0 ),
	m_timeAccu( 0.0 ),
	m_timeOverall( 0.0 ),
	m_extrapolationMinSizeFace( 0.0, 0.0 ),
	m_extrapolationMinSizeEye( 0.0, 0.0 ),
	m_extrapolationMinSizeNose( 0.0, 0.0 ),
	m_extrapolationMinSizeMouth( 0.0, 0.0 ),
	m_faceDetection( new FaceDetection() ),
	m_stopWatch( new StopWatch() ),
	m_faceTracking( new FaceTracking( 4 ) )
{}

FaceFeaturesBlock::~FaceFeaturesBlock()
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

void FaceFeaturesBlock::setup( BlockHandle &block )
{
	try
	{
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

		m_extrapolationMinSizeFace = Vec2( 0.0, 0.0 );
		m_extrapolationMinSizeEye = Vec2( 0.0, 0.0 );
		m_extrapolationMinSizeNose = Vec2( 0.0, 0.0 );
		m_extrapolationMinSizeMouth = Vec2( 0.0, 0.0 );

		m_faceOut = m_Block.getOutletHandle( "face_out" );

		const Image &image = m_imageIn.getReadableRef< Image >();

		m_faceDetection->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );
		m_faceDetection->loadEyesCascade( m_cascFileEyesIn.getReadableRef< std::string >() );
		m_faceDetection->loadNoseCascade( m_cascFileNoseIn.getReadableRef< std::string >() );
		m_faceDetection->loadMouthCascade( m_cascFileMouthIn.getReadableRef< std::string >() );

		m_faceDetection->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );
		m_faceDetection->minNeighboursEyes( m_haarMinNeighboursEyesIn.getReadableRef< unsigned int >() );
		m_faceDetection->minNeighboursNose( m_haarMinNeighboursNoseIn.getReadableRef< unsigned int >() );
		m_faceDetection->minNeighboursMouth( m_haarMinNeighboursMouthIn.getReadableRef< unsigned int >() );

		m_faceDetection->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );
		m_faceDetection->minSizeEyes( m_haarMinSizeEyesIn.getReadableRef< Vec2 >() );
		m_faceDetection->minSizeNose( m_haarMinSizeNoseIn.getReadableRef< Vec2 >() );
		m_faceDetection->minSizeMouth( m_haarMinSizeMouthIn.getReadableRef< Vec2 >() );

		m_faceDetection->doCannyPruning( m_haarDoCannyPruningIn.getReadableRef< bool >() );
		m_faceDetection->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );

		m_faceDetection->equalizeHist( m_equalizeHistogramIn.getReadableRef< bool >() );

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

void FaceFeaturesBlock::update()
{
	try
	{
		const Image &image = m_imageIn.getReadableRef< Image >();

		if( m_haarDetectionDownscaleIn.hasChanged() )
			m_faceDetection->greyImgSmlScale( m_haarDetectionDownscaleIn.getReadableRef< double >() );

		if( m_cascFileFaceIn.hasChanged() )
			m_faceDetection->loadFaceCascade( m_cascFileFaceIn.getReadableRef< std::string >() );
		if( m_cascFileEyesIn.hasChanged() )
			m_faceDetection->loadEyesCascade( m_cascFileEyesIn.getReadableRef< std::string >() );
		if( m_cascFileNoseIn.hasChanged() )
			m_faceDetection->loadNoseCascade( m_cascFileNoseIn.getReadableRef< std::string >() );
		if( m_cascFileMouthIn.hasChanged() )
			m_faceDetection->loadMouthCascade( m_cascFileMouthIn.getReadableRef< std::string >() );

		if( m_haarMinNeighboursFaceIn.hasChanged() )
			m_faceDetection->minNeighboursFace( m_haarMinNeighboursFaceIn.getReadableRef< unsigned int >() );
		if( m_haarMinNeighboursEyesIn.hasChanged() )
			m_faceDetection->minNeighboursEyes( m_haarMinNeighboursEyesIn.getReadableRef< unsigned int >() );
		if( m_haarMinNeighboursNoseIn.hasChanged() )
			m_faceDetection->minNeighboursNose( m_haarMinNeighboursNoseIn.getReadableRef< unsigned int >() );
		if( m_haarMinNeighboursMouthIn.hasChanged() )
			m_faceDetection->minNeighboursMouth( m_haarMinNeighboursMouthIn.getReadableRef< unsigned int >() );

		if( m_haarMinSizeFaceIn.hasChanged() )
			m_faceDetection->minSizeFace( m_haarMinSizeFaceIn.getReadableRef< Vec2 >() );
		if( m_haarMinSizeEyesIn.hasChanged() )
			m_faceDetection->minSizeEyes( m_haarMinSizeEyesIn.getReadableRef< Vec2 >() );
		if( m_haarMinSizeNoseIn.hasChanged() )
			m_faceDetection->minSizeNose( m_haarMinSizeNoseIn.getReadableRef< Vec2 >() );
		if( m_haarMinSizeMouthIn.hasChanged() )
			m_faceDetection->minSizeMouth( m_haarMinSizeMouthIn.getReadableRef< Vec2 >() );

		if( m_equalizeHistogramIn.hasChanged() )
			m_faceDetection->equalizeHist( m_equalizeHistogramIn.getReadableRef< bool >() );

		if( m_haarScaleFactorIn.hasChanged() )
			m_faceDetection->haarScaleFactor( m_haarScaleFactorIn.getReadableRef< double >() );

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

				for( TrackingInfoList::iterator itT = m_faceTracking->trackingInfoList().begin(); itT != m_faceTracking->trackingInfoList().end(); ++itT )
				{
					if( itT->getFaceTrajectory().getEntryCount() )
					{
						FeatureVector eyesCandidates;
						FeatureVector noseCandidates;
						FeatureVector mouthCandidates;

						m_faceDetection->detectFeatures( image, itT->getFaceTrajectory().getLastRegion(), 
							eyesCandidates, noseCandidates, mouthCandidates, 
							m_useEyesIn.getReadableRef< bool >(),
							m_useNoseIn.getReadableRef< bool >(),
							m_useMouthIn.getReadableRef< bool >() );

						itT->addFromFeatureCandidates( eyesCandidates, noseCandidates, mouthCandidates, m_timeOverall, m_affinityWeightPosIn.getReadableRef<double>(), m_affinityWeightSizeIn.getReadableRef<double>() );
					}
				}

				std::vector< FaceDesc > &faces = m_faceOut.getWriteableRef< std::vector< FaceDesc > >();
				faces.clear();

				for( TrackingInfoList::iterator itT = m_faceTracking->trackingInfoList().begin(); itT != m_faceTracking->trackingInfoList().end(); ++itT )
				{
					FaceDesc desc( itT->getUserID(), itT->getFaceTrajectory().getLastRegion() );

					if( m_useEyesIn.getReadableRef< bool >() )
					{
						if( itT->getEyeLeftTrajectory().getEntryCount() )
						{
							desc.eyeLeft( FeatureDesc( itT->getEyeLeftTrajectory().getLastRegion() ) );
						}
						if( itT->getEyeRightTrajectory().getEntryCount() )
						{
							desc.eyeRight( FeatureDesc( itT->getEyeRightTrajectory().getLastRegion() ) );
						}
					}
					if( m_useNoseIn.getReadableRef< bool >() && itT->getNoseTrajectory().getEntryCount() )
					{
						desc.nose( FeatureDesc( itT->getNoseTrajectory().getLastRegion() ) );
					}
					if( m_useMouthIn.getReadableRef< bool >() && itT->getNoseTrajectory().getEntryCount() )
					{
						desc.mouth( FeatureDesc( itT->getMouthTrajectory().getLastRegion() ) );
					}

					faces.push_back( desc );
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

void FaceFeaturesBlock::shutdown() {}