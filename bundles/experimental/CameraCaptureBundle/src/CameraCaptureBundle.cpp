#include "CameraDeviceManager.h"
#include "CameraCaptureBlock.h"

#include "_2RealBundle.h"
#include "_2RealBundleMetaInfo.h"
#include "_2RealBlockMetaInfo.h"
#include "_2RealContextBlockMetaInfo.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

using namespace _2Real;

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

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< CameraDeviceManager >();

		BlockMetainfo cameraCapture = info.exportBlock< CameraCaptureBlock, WithContext >( "CameraCaptureBlock" );
		cameraCapture.addInlet<int>("intValue", 0);
		cameraCapture.addInlet<float>("floatValue", 0);
		cameraCapture.addOutlet<int>("intValue");
		cameraCapture.addOutlet<float>("floatValue");
		cameraCapture.addOutlet<float>("floatValue1");

		cameraCapture.setDescription( "Camera Capture" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}