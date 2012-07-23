#include <iostream>
#include <string>
#include <sstream>
#include "KinectOpenNIIrBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

void KinectOpenNIIrBlock::setup( BlockHandle &block )
{
	// set generator type so base class object works on that generator, this inheritance helps avoid useless codeduplicatio, you could use templates as well
	setGeneratorType(_2RealKinectWrapper::INFRAREDIMAGE);
	KinectOpenNIBlockBase::setup(block);
}
