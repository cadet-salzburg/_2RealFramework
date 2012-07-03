#include "CameraDeviceManager.h"
#include "VideoInputBlock.h"

#include "_2RealBundle.h"
#include <sstream>

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
		info.setDescription( "Camera Capture" );
		info.setAuthor( "Robert Praxmarer" );
		info.setCategory( "experimental" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< CameraDeviceManager >();
		BlockMetainfo cameraCapture = info.exportBlock< VideoInputBlock, WithContext >( "VideoInputBlock" );

		cameraCapture.addParameter<int>( "DeviceIndex", 0 );
		cameraCapture.addOutlet<ImageT<unsigned char>>("imageOutlet");

		cameraCapture.setDescription( "Camera Capture" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}