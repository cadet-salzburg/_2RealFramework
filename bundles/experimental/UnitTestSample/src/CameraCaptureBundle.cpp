#include "CameraDeviceManager.h"
#include "CameraCaptureBlock.h"

#include "_2RealBundle.h"

using namespace _2Real::bundle;
using			_2Real::Exception;

using std::string;
using std::cout;
using std::endl;

//
void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setDescription( "Camera Capture Bundle" );
		info.setAuthor( "Robert Praxmarer" );
		info.setCategory( "experimental" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< CameraDeviceManager >();

		BlockMetainfo cameraCapture = info.exportBlock< CameraCaptureBlock, WithContext >( "CameraCaptureBlock" );
		cameraCapture.addInlet<double>("doubleInlet", 0);
		cameraCapture.addOutlet<_2Real::ImageT<unsigned char>>("imageOutlet");
		cameraCapture.addOutlet<double>("doubleOutlet");

		cameraCapture.setDescription( "Camera Capture" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}