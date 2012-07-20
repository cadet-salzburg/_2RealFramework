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
		BlockMetainfo blockInfo = info.exportBlock< KinectOpenNIRgbBlock, WithContext >( "KinectOpenNIRgbBlock" );

		blockInfo.addInlet<int>( "DeviceIndex", 0 );
		blockInfo.addInlet<int>( "Width", 320 );
		blockInfo.addInlet<int>( "Height", 240 );
		blockInfo.addInlet<int>( "Fps", 30 );
		blockInfo.addOutlet< ImageT<unsigned char> >("ImageData");
		blockInfo.addOutlet<int>( "Width" );
		blockInfo.addOutlet<int>( "Height" );
		blockInfo.setDescription( "RGB Kinect Image Block" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}