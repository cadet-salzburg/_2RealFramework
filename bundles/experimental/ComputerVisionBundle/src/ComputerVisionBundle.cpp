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

		unsigned short *init = makeCheckerboard< unsigned short >( 640, 480, 3, 20 );
		ImageSource checkerImg( init, false, 640, 480, ImageChannelOrder::RGB );
		Options< int > borderOptions = Options< int >( 0, "constant" )( 1, "replicate" )( 2, "reflect" )( 4, "reflect_101" );

		std::cout << "XXX" << checkerImg.getBitsPerPixel() << std::endl;

		BlockMetainfo gauss = info.exportBlock< OcvGaussianBlurBlock, WithoutContext >( "OcvGaussianBlurBlock" );
		gauss.setDescription( "applies gaussian blur to input image" );
		gauss.setCategory( "image filter" );
		// can handle any format
		gauss.addInlet< ImageSource >( "image_in", checkerImg );
		// must be odd
		gauss.addInlet< unsigned char >( "kernel_x", 1 );
		gauss.addInlet< unsigned char >( "kernel_y", 1 );
		// must be positive
		gauss.addInlet< double >( "sigma_x", 0. );
		gauss.addInlet< double >( "sigma_y", 0. );
		// those are the border interpolation options available for this function
		gauss.addInlet< int >( "boder_interpolation", 0, borderOptions );
		// format out == format in
		gauss.addOutlet< ImageSource >( "image_out" );

		//// either uchar in -> ushort out or float in - float out
		//BlockMetainfo sobel = info.exportBlock< OcvSobelBlock, WithoutContext >( "OcvSobelBlock" );
		//sobel.setDescription( "applies sobel filter to input image" );
		//sobel.setCategory( "image filter" );
		//sobel.addInlet< ImageT< unsigned char > >( "image_in", checkerImg );
		//sobel.addInlet< unsigned char>( "order_x", 1 );
		//sobel.addInlet< unsigned char >( "order_y", 1 );
		//sobel.addInlet< unsigned char >( "aperture", 3 );
		//sobel.addOutlet< ImageT< unsigned short > >( "image_out" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}