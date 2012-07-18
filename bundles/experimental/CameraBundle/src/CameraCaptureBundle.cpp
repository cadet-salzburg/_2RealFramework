#include "CameraDeviceManager.h"
#include "CameraCaptureBlock.h"

#include "_2RealBundle.h"
#include <sstream>
#include <iostream>

using namespace _2Real;

using namespace _2Real::bundle;
using			_2Real::Exception;
using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName("CameraCaptureBundle");
		info.setDescription( "Camera Capture" );
		info.setAuthor( "Robert Praxmarer" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< CameraDeviceManager >();
		BlockMetainfo cameraCapture = info.exportBlock< CameraCaptureBlock, WithContext >( "CameraCaptureBlock" );

		cameraCapture.addInlet<int>( "DeviceIndex", 0 );
		cameraCapture.addInlet<int>( "Width", 320 );
		cameraCapture.addInlet<int>( "Height", 240 );
		cameraCapture.addInlet<int>( "Fps", 30 );
		cameraCapture.addOutlet< ImageT<unsigned char> >("ImageData");
		cameraCapture.addOutlet<int>( "Width" );
		cameraCapture.addOutlet<int>( "Height" );

		cameraCapture.setDescription( "Camera Capture" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}