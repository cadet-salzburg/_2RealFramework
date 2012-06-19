#include "CameraDeviceManager.h"
#include "CameraCaptureBlock.h"

#include "_2RealBundle.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

using _2Real::BundleMetainfo;
using _2Real::BlockMetainfo;
using _2Real::Enumeration;
using _2Real::Enums;
using _2Real::Exception;

using std::string;
using std::cout;
using std::endl;

//
void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setDescription( "Camera Capture" );
		info.setAuthor( "Robert Praxmarer" );
		info.setCategory( "experimental" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		info.exportBundleContext< CameraDeviceManager >();

		info.addContextParameter< unsigned int >( "context number", (unsigned int)0 );
		info.addContextParameter< Enumeration >( "context enum", Enumeration( Enums( "enum 1", "value 1" )("enum 2", "value 2"), "no value" ) );

		BlockMetainfo cameraCapture = info.exportBlock< CameraCaptureBlock >( "CameraCaptureBlock" );
		cameraCapture.addInlet<int>("intValue", 0);
		cameraCapture.addInlet<float>("floatValue", 0);
		cameraCapture.addOutlet<int>("intValue", 10);
		cameraCapture.addOutlet<float>("floatValue", 0);
		cameraCapture.addOutlet<float>("floatValue1", 0);

		cameraCapture.setDescription( "Camera Capture" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}