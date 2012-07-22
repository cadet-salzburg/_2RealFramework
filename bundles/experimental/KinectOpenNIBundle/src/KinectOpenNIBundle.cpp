#include "OpenNIDeviceManager.h"
#include "KinectOpenNIRgbBlock.h"
#include "KinectOpenNIDepthBlock.h"
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
		rgbBlockInfo.addInlet<bool>( "IsMirrored", false );
		rgbBlockInfo.addInlet<bool>( "IsAlignedToDepth", false );
		rgbBlockInfo.addOutlet< ImageT<unsigned char> >("ImageData");
		rgbBlockInfo.addOutlet<int>( "Width" );
		rgbBlockInfo.addOutlet<int>( "Height" );
		rgbBlockInfo.setDescription( "OpenNI Kinect Rgb Image Block" );

		BlockMetainfo depthBlockInfo = info.exportBlock< KinectOpenNIDepthBlock, WithContext >( "KinectOpenNIDepthBlock" );
		depthBlockInfo.addInlet<int>( "DeviceIndex", 0 );
		depthBlockInfo.addInlet<int>( "Width", 640 );
		depthBlockInfo.addInlet<int>( "Height", 480 );
		depthBlockInfo.addInlet<int>( "Fps", 30 );
		depthBlockInfo.addInlet<bool>( "IsMirrored", false );
		depthBlockInfo.addOutlet< ImageT<unsigned char> >("ImageData");
		depthBlockInfo.addOutlet<int>( "Width" );
		depthBlockInfo.addOutlet<int>( "Height" );
		depthBlockInfo.setDescription( "OpenNI Kinect Depth Image Block" );

		BlockMetainfo userSkeletonBlockInfo = info.exportBlock< KinectOpenNIUserSkeletonBlock, WithContext >( "KinectOpenNIUserSkeletonBlock" );
		userSkeletonBlockInfo.addInlet<int>( "DeviceIndex", 0 );
		userSkeletonBlockInfo.addInlet<int>( "Width", 640 );
		userSkeletonBlockInfo.addInlet<int>( "Height", 480 );
		userSkeletonBlockInfo.addInlet<int>( "Fps", 30 );
		userSkeletonBlockInfo.addInlet<bool>( "IsMirrored", false );
		userSkeletonBlockInfo.addOutlet< ImageT<unsigned char> >("ImageData");
		userSkeletonBlockInfo.addOutlet<int>( "Width" );
		userSkeletonBlockInfo.addOutlet<int>( "Height" );
		userSkeletonBlockInfo.setDescription( "OpenNI Kinect User/Skeleton Image Block" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}