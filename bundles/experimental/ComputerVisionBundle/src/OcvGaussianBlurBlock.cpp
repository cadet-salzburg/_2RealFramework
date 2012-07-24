#include "OcvGaussianBlurBlock.h"
#include "_2RealDatatypes.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "ImageHelpers.h"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

OcvGaussianBlurBlock::OcvGaussianBlurBlock() : Block() {}
OcvGaussianBlurBlock::~OcvGaussianBlurBlock() {}

void OcvGaussianBlurBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;
		//ImageSource &output = m_Block.getOutletHandle( "image_out" ).getWriteableRef< ImageSource >();

		//size_t sz = 640 * 480 * 3;
		//unsigned char *init = new unsigned char[ sz ];
		//unsigned char *p = init;
		//for ( unsigned int i=0; i<480; ++i )
		//{
		//	for ( unsigned int j=0; j<640; ++j )
		//	{
		//		*p = static_cast< unsigned char >( 0 );
		//		++p;
		//		*p = static_cast< unsigned char >( 0 );
		//		++p;
		//		*p = static_cast< unsigned char >( 0 );
		//		++p;
		//	}
		//}
		//output.assign( init, true, 640, 480, ImageChannelOrder::RGB );
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void OcvGaussianBlurBlock::update()
{
	try
	{
		ImageSource &output = m_Block.getOutletHandle( "image_out" ).getWriteableRef< ImageSource >();

		vector< InletHandle > inlets = m_Block.getAllInletHandles();

		// inlets are accessible in the same order they were declared in the metadata
		ImageSource const& input = inlets[ 0 ].getReadableRef< ImageSource>();
		unsigned char kernelX = inlets[ 1 ].getReadableRef< unsigned char >();
		unsigned char kernelY = inlets[ 2 ].getReadableRef< unsigned char >();
		double const& sigmaX = inlets[ 3 ].getReadableRef< double >();
		double const& sigmaY = inlets[ 4 ].getReadableRef< double >();
		int const& border = inlets[ 5 ].getReadableRef< int >();

		// requires a range -> maye be checked by the fw in the future
		if ( sigmaX < 0. || sigmaY < 0. )
		{
			// (?) throwing an exception is not really a good idea in this case
			// ( exc requires re-setup, and that really would not help )
			// nonetheless, the user should be notified somehow?
			m_Block.getOutletHandle( "image_out" ).discard();
			return;
		}

		// kernel size must be odd. i don't think the framework should handle this
		if ( kernelX%2 != 1 || kernelY%2 != 1 )
		{
			// (?) throwing an exception is not really a good idea in this case
			// ( exc requires re-setup, and that really would not help )
			// nonetheless, the user should be notified somehow?
			m_Block.getOutletHandle( "image_out" ).discard();
			return;
		}

		cv::Mat *matSrc = convertToCvMat( input );
		cv::Mat *matDst = convertToCvMat( output );
		cv::GaussianBlur( *matSrc, *matDst, cv::Size( kernelX, kernelY ), sigmaX, sigmaY, border );
		delete matSrc;
		delete matDst;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void OcvGaussianBlurBlock::shutdown() {}