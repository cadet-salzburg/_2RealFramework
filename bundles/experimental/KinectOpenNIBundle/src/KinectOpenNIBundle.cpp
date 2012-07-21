#include "OpenNIDeviceManager.h"
#include "KinectOpenNIRgbBlock.h"
#include "KinectOpenNIUserSkeletonBlock.h"

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

		BlockMetainfo rgbBlockInfo = info.exportBlock< KinectOpenNIRgbBlock, WithContext >( "KinectOpenNIRgbBlock" );
		rgbBlockInfo.addInlet<int>( "DeviceIndex", 0 );
		rgbBlockInfo.addInlet<int>( "Width", 640 );
		rgbBlockInfo.addInlet<int>( "Height", 480 );
		rgbBlockInfo.addInlet<int>( "Fps", 30 );
		rgbBlockInfo.addOutlet< ImageT<unsigned char> >("ImageData");
		rgbBlockInfo.addOutlet<int>( "Width" );
		rgbBlockInfo.addOutlet<int>( "Height" );
		rgbBlockInfo.setDescription( "RGB Kinect Rgb Image Block" );

		BlockMetainfo userSkeletonBlockInfo = info.exportBlock< KinectOpenNIUserSkeletonBlock, WithContext >( "KinectOpenNIUserSkeletonBlock" );
		userSkeletonBlockInfo.addInlet<int>( "DeviceIndex", 0 );
		userSkeletonBlockInfo.addInlet<int>( "Width", 640 );
		userSkeletonBlockInfo.addInlet<int>( "Height", 480 );
		userSkeletonBlockInfo.addInlet<int>( "Fps", 30 );
		userSkeletonBlockInfo.addOutlet< ImageT<unsigned char> >("ImageData");
		userSkeletonBlockInfo.addOutlet<int>( "Width" );
		userSkeletonBlockInfo.addOutlet<int>( "Height" );
		userSkeletonBlockInfo.setDescription( "RGB Kinect User/Skeleton Image Block" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}