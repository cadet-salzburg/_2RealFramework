#include "CameraDeviceManager.h"
#include "VideoInputBlock.h"

#include "_2RealBundle.h"
#include "_2RealBundleMetaInfo.h"
#include "_2RealBlockMetaInfo.h"
#include "_2RealContextBlockMetaInfo.h"
#include "_2RealException.h"
#include "_2RealEnum.h"
#include "_2RealImageT.h"
#include <sstream>

using namespace _2Real;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setDescription( "Camera Capture" );
		info.setAuthor( "Nikolas Psarudakis" );
		info.setCategory( "experimental" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< CameraDeviceManager >();
		BlockMetainfo cameraCapture = info.exportBlock< VideoInputBlock, WithContext >( "VideoInputBlock" );

		cameraCapture.addInlet<int>("deviceIndexInlet", 2);
		cameraCapture.addOutlet<ImageT<unsigned char>>("imageOutlet");

		cameraCapture.setDescription( "Video Input" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}