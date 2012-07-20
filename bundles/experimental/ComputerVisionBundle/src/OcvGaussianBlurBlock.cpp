#include "OcvGaussianBlurBlock.h"
#include "_2RealDatatypes.h"
#include "opencv2/imgproc/imgproc.hpp"

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
		ImageT< unsigned char > &output = m_Block.getOutletHandle( "image_out" ).getWriteableRef< ImageT< unsigned char > >();

		size_t sz = 640 * 480 * 3;
		unsigned char *init = new unsigned char[ sz ];
		unsigned char *p = init;
		for ( unsigned int i=0; i<480; ++i )
		{
			for ( unsigned int j=0; j<640; ++j )
			{
				*p = static_cast< unsigned char >( 0 );
				++p;
				*p = static_cast< unsigned char >( 0 );
				++p;
				*p = static_cast< unsigned char >( 0 );
				++p;
			}
		}
		output.assign( init, true, 640, 480, ImageChannelOrder::RGB );
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
		ImageT< unsigned char > &output = m_Block.getOutletHandle( "image_out" ).getWriteableRef< ImageT< unsigned char > >();

		vector< InletHandle > inlets = m_Block.getAllInletHandles();

		// inlets are accessible in the same order they were declared in the metadata
		ImageT< unsigned char > const& input = inlets[ 0 ].getReadableRef< ImageT < unsigned char > >();
		unsigned char kernelX = inlets[ 1 ].getReadableRef< unsigned char >();
		unsigned char kernelY = inlets[ 2 ].getReadableRef< unsigned char >();
		double const& sigmaX = inlets[ 3 ].getReadableRef< double >();
		double const& sigmaY = inlets[ 4 ].getReadableRef< double >();

		if ( sigmaX < 0. || sigmaY < 0. )
		{
			// (?) throwing an exception is not really a good idea in this case
			// ( exc requires re-setup, and that really would not help )
			// nonetheless, the user should be notified somehow?
			m_Block.getOutletHandle( "image_out" ).discard();
			return;
		}

		if ( kernelX%2 != 1 || kernelY%2 != 1 )
		{
			// (?) throwing an exception is not really a good idea in this case
			// ( exc requires re-setup, and that really would not help )
			// nonetheless, the user should be notified somehow?
			m_Block.getOutletHandle( "image_out" ).discard();
			return;
		}

		//ImageT< unsigned char > const& input = m_Block.getInletHandle( "image_in" ).getReadableRef< ImageT < unsigned char > >();
		//unsigned char kernelX = m_Block.getInletHandle( "kernel_x" ).getReadableRef< unsigned char >();
		//unsigned char kernelY = m_Block.getInletHandle( "kernel_y" ).getReadableRef< unsigned char >();
		//double sigmaX = m_Block.getInletHandle( "sigma_x" ).getReadableRef< double >();
		//double sigmaY = m_Block.getInletHandle( "sigma_y" ).getReadableRef< double >();

		// (?) where to get the format from, C1 - C4 comes from channel order; 8U whatever comes from the template type
		// thankfully, the conversion thing itself is super easy
		cv::Mat matSrc( input.getWidth(), input.getHeight(), CV_8UC3, input.getData() );
		cv::Mat matDst( output.getWidth(), output.getHeight(), CV_8UC3, output.getData() );
		cv::GaussianBlur( matSrc, matDst, cv::Size( kernelX, kernelY ), sigmaX, sigmaY, cv::BORDER_DEFAULT );
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