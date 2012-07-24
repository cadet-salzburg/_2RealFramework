#include <iostream>
#include <string>
#include <sstream>
#include "KinectOpenNIUserSkeletonBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

void KinectOpenNIUserSkeletonBlock::setup( BlockHandle &block )
{
	try
	{
		KinectOpenNIDepthBlock::setup(block);
		setGeneratorType(_2RealKinectWrapper::USERIMAGE);
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void KinectOpenNIUserSkeletonBlock::update()
{
	try
	{
		KinectOpenNIDepthBlock::update();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

