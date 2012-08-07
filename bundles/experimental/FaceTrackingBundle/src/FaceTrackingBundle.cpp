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
		Image initImage;

		BlockMetainfo faceCast = info.exportBlock< FaceCastBlock, WithoutContext>( "FaceCastBlock" );
		faceCast.setDescription( "creates 3D vertex and normal data for faces find in an RGBD image stream" );
		faceCast.setCategory( "" );	//TODO: set
		// can handle any format
		//TODO: only rgb/greyscale -- how to specify?
		faceCast.addInlet< Image >( "image_in", initImage );
		faceCast.addInlet< Image >( "depth_in", initImage );

		faceCast.addOutlet< unsigned int >( "userid_out" );
		faceCast.addOutlet< Vec2 >( "center_out" );
		faceCast.addOutlet< Vec3 >( "com_out" );
		faceCast.addOutlet< std::vector< Vec3 > >( "vertices_out" );
		faceCast.addOutlet< std::vector< Vec3 > >( "normals_out" );

		BlockMetainfo faceFeatures = info.exportBlock< FaceFeaturesBlock, WithoutContext>( "FaceFeaturesBlock" );
		faceFeatures.setDescription( "detects faces and face features in input image stream" );
		faceFeatures.setCategory( "" ); //TODO: set
		// can handle any format
		//TODO: only rgb/greyscale -- how to specify?

		faceFeatures.addInlet< Image >( "image_in", initImage );

		faceFeatures.addInlet< bool >( "use_face", true );
		faceFeatures.addInlet< bool >( "use_eyes", true );
		faceFeatures.addInlet< bool >( "use_nose", true );
		faceFeatures.addInlet< bool >( "use_mouth", true );

		faceFeatures.addInlet< std::string >( "cascadefile_face", "ressources\\FaceTrackingBundle\\haarcascade_frontalface_alt2.xml" );
		faceFeatures.addInlet< std::string >( "cascadefile_eyes", "ressources\\FaceTrackingBundle\\haarcascade_eye.xml" );
		faceFeatures.addInlet< std::string >( "cascadefile_nose", "ressources\\FaceTrackingBundle\\haarcascade_mcs_nose.xml" );
		faceFeatures.addInlet< std::string >( "cascadefile_mouth", "ressources\\FaceTrackingBundle\\haarcascade_mcs_mouth.xml" );

		faceFeatures.addOutlet< std::vector< Space2D > >( "face_out" );
		faceFeatures.addOutlet< std::vector< Space2D > >( "eyes_out" );
		faceFeatures.addOutlet< std::vector< Space2D > >( "nose_out" );
		faceFeatures.addOutlet< std::vector< Space2D > >( "mouth_out" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}