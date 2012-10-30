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

		unsigned char *init = makeCheckerboard< unsigned char >( 640, 480, 1, 6 );
		Image checkerImg( init, true, 640, 480, ImageChannelOrder::R );
		Options< int > borderOptions = Options< int >( 0, "constant" )( 1, "replicate" )( 2, "reflect" )( 4, "reflect_101" );

		BlockMetainfo gauss = info.exportBlock< OcvGaussianBlurBlock, WithoutContext >( "OcvGaussianBlurBlock" );
		gauss.setDescription( "applies gaussian blur to input image" );
		gauss.setCategory( "image filter" );
		// can handle any format
		gauss.addInlet< Image >( "InImage", checkerImg );
		// must be odd
		gauss.addInlet< unsigned char >( "kernel_x", 5 );
		gauss.addInlet< unsigned char >( "kernel_y", 5 );
		// must be positive
		gauss.addInlet< double >( "sigma_x", 1.1 );
		gauss.addInlet< double >( "sigma_y", 1.1 );
		// those are the border interpolation options available for this function
		gauss.addInlet< int >( "boder_interpolation", 0, borderOptions );
		// format out == format in
		gauss.addOutlet< Image >( "OutImage" );

		BlockMetainfo eq = info.exportBlock< OcvHistogramEqualizationBlock, WithoutContext >( "OcvHistogramEqualizationBlock" );
		eq.setDescription( "xxx" );
		eq.setCategory( "image filter" );
		eq.addInlet< Image >( "ImageData", checkerImg );
		eq.addOutlet< Image >( "ImageData" );

		////// either uchar in -> ushort out or float in - float out
		//BlockMetainfo sobel = info.exportBlock< OcvSobelBlock, WithoutContext >( "OcvSobelBlock" );
		//sobel.setDescription( "applies sobel filter to input image" );
		//sobel.setCategory( "image filter" );
		//sobel.addInlet< Image >( "image_in", checkerImg );
		//sobel.addInlet< unsigned char>( "order_x", 1 );
		//sobel.addInlet< unsigned char >( "order_y", 1 );
		//sobel.addInlet< unsigned char >( "aperture", 3 );
		//sobel.addOutlet< Image >( "image_out" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}