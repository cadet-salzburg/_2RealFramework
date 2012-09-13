#include "_2RealBundle.h"
#include "FaceCastBlock.h"
#include "FaceFeaturesBlock.h"
#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;

//TODO: remove this when _2Real utils have arrived
template<typename T>
void fillChecker( T *buffer, size_t width, size_t height, size_t depth )
{
	for( int i = 0; i < width; i++ )
	{
		for( int j = 0; j < height; j++ )
		{
			if( i >> 3 & 0x01 ^ j >> 3 & 0x01 )
			{
				for( int k = 0; k < depth; k++ )
					*( buffer++ ) = std::numeric_limits<T>::max();
			}
			else
			{
				for( int k = 0; k < depth; k++ )
					*( buffer++ ) = std::numeric_limits<T>::min();
			}
		}
	}
}

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		std::cout << "hallo" << std::endl;

		info.setName( "FaceTrackingBundle" );
		info.setDescription( "face tracking stuff" );
		info.setAuthor( "roland aigner" );
		info.setCategory( "image processing" );
		info.setContact( "support@cadet.at" );
		info.setVersion( 0, 1, 0 );

		unsigned char *rgbChecker = new unsigned char[640 * 480 * 3];
		unsigned short *depthChecker = new unsigned short[640 * 480 * 1];

		fillChecker( rgbChecker, 640, 480, 3 );
		fillChecker( depthChecker, 640, 480, 1 );

		Image initRGBImage( rgbChecker, true, 640, 480, ImageChannelOrder::RGB );
		Image initDepthImage( depthChecker, true, 640, 480, ImageChannelOrder::A );
		/*
		Image initRGBImage( (unsigned char*)NULL, false, 640, 480, ImageChannelOrder::RGB );
		Image initDepthImage( (unsigned short*)NULL, false, 640, 480, ImageChannelOrder::A );
		*/

		BlockMetainfo faceCast = info.exportBlock< FaceCastBlock, WithoutContext>( "FaceCastBlock" );
		faceCast.setDescription( "Creates 3D vertex and normal data for faces find in an RGBD image stream" );
		faceCast.setCategory( "image processing" );
		//TODO: only rgb/greyscale allowed -- how to specify?
		faceCast.addInlet< Image >( "ImageIn", initRGBImage );
		faceCast.addInlet< Image >( "DepthIn", initDepthImage );

		faceCast.addInlet< unsigned int >( "ResX", 40 );
		faceCast.addInlet< unsigned int >( "ResY", 40 );

		faceCast.addInlet< double >( "FaceScaleX", 1.35 );
		faceCast.addInlet< double >( "FaceScaleY", 1.6 );

		faceCast.addInlet< double >( "DepthCutoff", 0.1 );

		//default values taken from Asus Xtion Pro
		//faceCast.addInlet< double >( "FovHor", 58.5907 );	//redundant, not used
		faceCast.addInlet< double >( "FovVer", 45.6427 );

		faceCast.addInlet< double >( "HaarDetectionDownscale", 0.75 );

		faceCast.addInlet< unsigned int >( "HaarMinNeighboursFace", 3 );
		faceCast.addInlet< Vec2 >( "HaarMinSizeFace", Vec2( 0.0, 0.0 ) );

		faceCast.addInlet< bool >( "HaarDoCannyPruning", false );
		faceCast.addInlet< double >( "HaarScalefactor", 1.1 );

		faceCast.addInlet< bool >( "EqualizeHistogram", false );

		faceCast.addInlet< std::string >( "CascadefileFace", "ressources\\FaceTrackingBundle\\haarcascade_frontalface_alt2.xml" );

		faceCast.addInlet< double >( "ExtrapolationDamping", 0.75 );
		faceCast.addInlet< double >( "ExtrapolationCoherenceRise", 0.27 );

		faceCast.addInlet< double >( "AffinityWeightPos", 0.6 );
		faceCast.addInlet< double >( "AffinityWeightSize", 0.4 );

		faceCast.addInlet< double >( "CoherenceWeightDir", 0.1 );
		faceCast.addInlet< double >( "CoherenceWeightVel", 0.5 );
		faceCast.addInlet< double >( "CoherenceWeightSize", 0.4 );

		faceCast.addInlet< double >( "CoherenceToleranceDir", 0.75 );
		faceCast.addInlet< double >( "CoherenceToleranceVel", 0.75 );
		faceCast.addInlet< double >( "CoherenceToleranceSize", 0.75 );

		faceCast.addInlet< double >( "AffinityThreshold", 1.0 );
		faceCast.addInlet< double >( "CoherenceThreshold", 0.2 );
		faceCast.addInlet< double >( "DiscardThreshold", 0.5 );

		faceCast.addOutlet< std::vector< FaceCast > >( "FaceOut" );

		BlockMetainfo faceFeatures = info.exportBlock< FaceFeaturesBlock, WithoutContext>( "FaceFeaturesBlock" );
		faceFeatures.setDescription( "Detects faces and face features in input image stream" );
		faceFeatures.setCategory( "image processing" );
		// can handle any format
		//TODO: only rgb/greyscale allowed -- how to specify?
		faceFeatures.addInlet< Image >( "ImageIn", initRGBImage );

		faceFeatures.addInlet< double >( "HaarDetectionDownscale", 0.75 );

		faceFeatures.addInlet< unsigned int >( "HaarMinNeighboursFace", 3 );
		faceFeatures.addInlet< Vec2 >( "HaarMinSizeFace", Vec2( 0.0, 0.0 ) );

		faceFeatures.addInlet< bool >( "UseEyes", true );
		faceFeatures.addInlet< unsigned int >( "HaarMinNeighboursEyes", 3 );
		faceFeatures.addInlet< Vec2 >( "HaarMinSizeEyes", Vec2( 0.0, 0.0 ) );

		faceFeatures.addInlet< bool >( "UseNose", true );
		faceFeatures.addInlet< unsigned int >( "HaarMinNeighboursNose", 3 );
		faceFeatures.addInlet< Vec2 >( "HaarMinSizeNose", Vec2( 0.0, 0.0 ) );

		faceFeatures.addInlet< bool >( "UseMouth", true );
		faceFeatures.addInlet< unsigned int >( "HaarMinNeighboursMouth", 3 );
		faceFeatures.addInlet< Vec2 >( "HaarMinSizeMouth", Vec2( 0.0, 0.0 ) );

		faceFeatures.addInlet< bool >( "HaarDoCannyPruning", false );
		faceFeatures.addInlet< double >( "HaarScalefactor", 1.1 );

		faceFeatures.addInlet< bool >( "EqualizeHistogram", false );

		faceFeatures.addInlet< std::string >( "CascadefileFace", "ressources\\FaceTrackingBundle\\haarcascade_frontalface_alt2.xml" );
		faceFeatures.addInlet< std::string >( "CascadefileEyes", "ressources\\FaceTrackingBundle\\haarcascade_eye.xml" );
		faceFeatures.addInlet< std::string >( "CascadefileNose", "ressources\\FaceTrackingBundle\\haarcascade_mcs_nose.xml" );
		faceFeatures.addInlet< std::string >( "CascadefileMouth", "ressources\\FaceTrackingBundle\\haarcascade_mcs_mouth.xml" );

		faceFeatures.addInlet< double >( "Extrapolation_damping", 0.75 );
		faceFeatures.addInlet< double >( "Extrapolation_coherence_rise", 0.27 );

		faceFeatures.addInlet< double >( "AffinityWeightPos", 0.6 );
		faceFeatures.addInlet< double >( "AffinityWeightSize", 0.4 );

		faceFeatures.addInlet< double >( "CoherenceWeightDir", 0.1 );
		faceFeatures.addInlet< double >( "CoherenceWeightVel", 0.5 );
		faceFeatures.addInlet< double >( "CoherenceWeightSize", 0.4 );

		faceFeatures.addInlet< double >( "CoherenceToleranceDir", 0.75 );
		faceFeatures.addInlet< double >( "CoherenceToleranceVel", 0.75 );
		faceFeatures.addInlet< double >( "CoherenceToleranceSize", 0.75 );

		faceFeatures.addInlet< double >( "AffinityThreshold", 1.0 );
		faceFeatures.addInlet< double >( "CoherenceThreshold", 0.2 );
		faceFeatures.addInlet< double >( "DiscardThreshold", 0.5 );

		faceFeatures.addOutlet< std::vector< FaceDesc > >( "FaceOut" );
	}
	catch ( Exception &e )
	{
		std::cout << e.message() << std::endl;
		e.rethrow();
	}
	catch ( std::exception &e )
	{
		std::cout << e.what() << std::endl;
	}
}