#include "_2RealBundle.h"
#include "OcvGaussianBlurBlock.h"
#include "OcvSobelBlock.h"
#include "ImageHelpers.h"
#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "ComputerVisionBundle" );
		info.setDescription( "computer vision stuff" );
		info.setAuthor( "gerlinde emsenhuber" );
		info.setCategory( "image processing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		unsigned char *init = makeCheckerboard< unsigned char >( 640, 480, 3, 17 );
		ImageT< unsigned char > checkerImg( init, true, 640, 480, ImageChannelOrder::RGB );

		// can handle any format
		BlockMetainfo gaussianBlur = info.exportBlock< OcvGaussianBlurBlock, WithoutContext >( "OcvGaussianBlurBlock" );
		gaussianBlur.setDescription( "applies gaussian blur to input image" );
		gaussianBlur.setCategory( "image filter" );
		gaussianBlur.addInlet< ImageT< unsigned char > >( "image_in", checkerImg );
		gaussianBlur.addInlet< unsigned char >( "kernel_x", 1 );
		gaussianBlur.addInlet< unsigned char >( "kernel_y", 1 );
		gaussianBlur.addInlet< double >( "sigma_x", 0. );
		gaussianBlur.addInlet< double >( "sigma_y", 0. );
		gaussianBlur.addOutlet< ImageT< unsigned char > >( "image_out" );

		// either uchar in -> ushort out or float in - float out
		BlockMetainfo sobel = info.exportBlock< OcvSobelBlock, WithoutContext >( "OcvSobelBlock" );
		sobel.setDescription( "applies sobel filter to input image" );
		sobel.setCategory( "image filter" );
		sobel.addInlet< ImageT< unsigned char > >( "image_in", checkerImg );
		sobel.addInlet< unsigned char>( "order_x", 1 );
		sobel.addInlet< unsigned char >( "order_y", 1 );
		sobel.addInlet< unsigned char >( "aperture", 3 );
		sobel.addOutlet< ImageT< unsigned short > >( "image_out" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}