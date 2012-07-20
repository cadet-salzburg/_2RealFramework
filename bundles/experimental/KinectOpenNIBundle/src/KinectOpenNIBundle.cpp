#include "OpenNIDeviceManager.h"
#include "KinectOpenNIRgbBlock.h"

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
		info.setName("KinectOpenNIBundle");
		info.setDescription( "Kinect OpenNI Bundle" );
		info.setAuthor( "Robert Praxmarer, Nikolas Psaroudakis" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< OpenNIDeviceManager >();
		BlockMetainfo cameraCapture = info.exportBlock< KinectOpenNIRgbBlock, WithContext >( "KinectOpenNIRgbBlock" );

		cameraCapture.addInlet<int>( "DeviceIndex", 0 );
		cameraCapture.addInlet<int>( "Width", 320 );
		cameraCapture.addInlet<int>( "Height", 240 );
		cameraCapture.addInlet<int>( "Fps", 30 );
		cameraCapture.addOutlet< ImageT<unsigned char> >("ImageData");
		cameraCapture.addOutlet<int>( "Width" );
		cameraCapture.addOutlet<int>( "Height" );
		cameraCapture.setDescription( "RGB Kinect Image Block" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}