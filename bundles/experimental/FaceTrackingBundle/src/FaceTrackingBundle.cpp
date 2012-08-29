#include "_2RealBundle.h"
#include "FaceCastBlock.h"
#include "FaceFeaturesBlock.h"
#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "FaceTrackingBundle" );
		info.setDescription( "face tracking stuff" );
		info.setAuthor( "roland aigner" );
		info.setCategory( "image processing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		//TODO: init with checker pattern
		Image initRGBImage( (unsigned char*)NULL, false, 640, 480, ImageChannelOrder::RGB );
		Image initDepthImage( (unsigned short*)NULL, false, 640, 480, ImageChannelOrder::A );

		BlockMetainfo faceCast = info.exportBlock< FaceCastBlock, WithoutContext>( "FaceCastBlock" );
		faceCast.setDescription( "creates 3D vertex and normal data for faces find in an RGBD image stream" );
		faceCast.setCategory( "" );	//TODO: set
		// can handle any format
		//TODO: only rgb/greyscale -- how to specify?
		faceCast.addInlet< Image >( "image_in", initRGBImage );
		faceCast.addInlet< Image >( "depth_in", initDepthImage );

		faceCast.addInlet< unsigned int >( "res_x", 40 );
		faceCast.addInlet< unsigned int >( "res_y", 40 );

		faceCast.addInlet< double >( "depth_cutoff", 0.1 );

		//default values taken from Asus Xtion
		//faceCast.addInlet< double >( "fov_hor", 58.5907 );	//redundant, not used
		faceCast.addInlet< double >( "fov_ver", 45.6427 );

		faceCast.addInlet< double >( "haar_detection_downscale", 0.75 );

		faceCast.addInlet< unsigned int >( "haar_min_neighbours_face", 3 );
		faceCast.addInlet< Vec2 >( "haar_minSizeFace", Vec2( 0.0, 0.0 ) );

		faceCast.addInlet< bool >( "haar_do_canny_pruning", false );
		faceCast.addInlet< double >( "haar_scalefactor", 1.1 );

		faceCast.addInlet< bool >( "equalize_histogram", false );

		faceCast.addInlet< std::string >( "cascadefile_face", "ressources\\FaceTrackingBundle\\haarcascade_frontalface_alt2.xml" );

		faceCast.addInlet< double >( "extrapolation_damping", 0.75 );
		faceCast.addInlet< double >( "extrapolation_coherence_rise", 0.27 );

		faceCast.addInlet< double >( "affinity_weight_pos", 0.6 );
		faceCast.addInlet< double >( "affinity_weight_size", 0.4 );

		faceCast.addInlet< double >( "coherence_weight_dir", 0.1 );
		faceCast.addInlet< double >( "coherence_weight_vel", 0.5 );
		faceCast.addInlet< double >( "coherence_weight_size", 0.4 );

		faceCast.addInlet< double >( "coherence_tolerance_dir", 0.75 );
		faceCast.addInlet< double >( "coherence_tolerance_vel", 0.75 );
		faceCast.addInlet< double >( "coherence_tolerance_size", 0.75 );

		faceCast.addInlet< double >( "affinity_threshold", 1.0 );
		faceCast.addInlet< double >( "coherence_threshold", 0.2 );
		faceCast.addInlet< double >( "discard_threshold", 0.5 );

		faceCast.addOutlet< std::vector< FaceCast > >( "face_out" );

		BlockMetainfo faceFeatures = info.exportBlock< FaceFeaturesBlock, WithoutContext>( "FaceFeaturesBlock" );
		faceFeatures.setDescription( "detects faces and face features in input image stream" );
		faceFeatures.setCategory( "" ); //TODO: set
		// can handle any format
		//TODO: only rgb/greyscale -- how to specify?

		faceFeatures.addInlet< Image >( "image_in", initRGBImage );

		faceFeatures.addInlet< double >( "haar_detection_downscale", 0.75 );

		faceFeatures.addInlet< unsigned int >( "haar_min_neighbours_face", 3 );
		faceFeatures.addInlet< Vec2 >( "haar_minSizeFace", Vec2( 0.0, 0.0 ) );

		faceFeatures.addInlet< bool >( "use_eyes", true );
		faceFeatures.addInlet< unsigned int >( "haar_min_neighbours_eyes", 3 );
		faceFeatures.addInlet< Vec2 >( "haar_min_size_eyes", Vec2( 0.0, 0.0 ) );

		faceFeatures.addInlet< bool >( "use_nose", true );
		faceFeatures.addInlet< unsigned int >( "haar_min_neighbours_nose", 3 );
		faceFeatures.addInlet< Vec2 >( "haar_min_size_nose", Vec2( 0.0, 0.0 ) );

		faceFeatures.addInlet< bool >( "use_mouth", true );
		faceFeatures.addInlet< unsigned int >( "haar_min_neighbours_mouth", 3 );
		faceFeatures.addInlet< Vec2 >( "haar_min_size_mouth", Vec2( 0.0, 0.0 ) );

		faceFeatures.addInlet< bool >( "haar_do_canny_pruning", false );
		faceFeatures.addInlet< double >( "haar_scalefactor", 1.1 );

		faceFeatures.addInlet< bool >( "equalize_histogram", false );

		faceFeatures.addInlet< std::string >( "cascadefile_face", "ressources\\FaceTrackingBundle\\haarcascade_frontalface_alt2.xml" );
		faceFeatures.addInlet< std::string >( "cascadefile_eyes", "ressources\\FaceTrackingBundle\\haarcascade_eye.xml" );
		faceFeatures.addInlet< std::string >( "cascadefile_nose", "ressources\\FaceTrackingBundle\\haarcascade_mcs_nose.xml" );
		faceFeatures.addInlet< std::string >( "cascadefile_mouth", "ressources\\FaceTrackingBundle\\haarcascade_mcs_mouth.xml" );

		faceFeatures.addInlet< double >( "extrapolation_damping", 0.75 );
		faceFeatures.addInlet< double >( "extrapolation_coherence_rise", 0.27 );

		faceFeatures.addInlet< double >( "affinity_weight_pos", 0.6 );
		faceFeatures.addInlet< double >( "affinity_weight_size", 0.4 );

		faceFeatures.addInlet< double >( "coherence_weight_dir", 0.1 );
		faceFeatures.addInlet< double >( "coherence_weight_vel", 0.5 );
		faceFeatures.addInlet< double >( "coherence_weight_size", 0.4 );

		faceFeatures.addInlet< double >( "coherence_tolerance_dir", 0.75 );
		faceFeatures.addInlet< double >( "coherence_tolerance_vel", 0.75 );
		faceFeatures.addInlet< double >( "coherence_tolerance_size", 0.75 );

		faceFeatures.addInlet< double >( "affinity_threshold", 1.0 );
		faceFeatures.addInlet< double >( "coherence_threshold", 0.2 );
		faceFeatures.addInlet< double >( "discard_threshold", 0.5 );

		faceFeatures.addOutlet< std::vector< FaceDesc > >( "face_out" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}