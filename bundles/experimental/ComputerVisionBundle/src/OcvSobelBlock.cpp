#include "OcvSobelBlock.h"
#include "_2RealDatatypes.h"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

OcvSobelBlock::OcvSobelBlock() : Block() {}
OcvSobelBlock::~OcvSobelBlock() {}

void OcvSobelBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;
		ImageT< unsigned short > &output = m_Block.getOutletHandle( "image_out" ).getWriteableRef< ImageT< unsigned short > >();

		size_t sz = 640 * 480 * 3;
		unsigned short *init = new unsigned short[ sz ];
		unsigned short *p = init;
		for ( unsigned int i=0; i<480; ++i )
		{
			for ( unsigned int j=0; j<640; ++j )
			{
				*p = static_cast< unsigned short >( 0 );
				++p;
				*p = static_cast< unsigned short >( 0 );
				++p;
				*p = static_cast< unsigned short >( 0 );
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

void OcvSobelBlock::update()
{
	try
	{
		ImageT< unsigned short > &output = m_Block.getOutletHandle( "image_out" ).getWriteableRef< ImageT< unsigned short > >();

		vector< InletHandle > inlets = m_Block.getAllInletHandles();

		// inlets are accessible in the same order they were declared in the metadata
		ImageT< unsigned char > const& input = inlets[ 0 ].getReadableRef< ImageT < unsigned char > >();
		unsigned char orderX = inlets[ 1 ].getReadableRef< unsigned char >();
		unsigned char orderY = inlets[ 2 ].getReadableRef< unsigned char >();
		unsigned char aperture = inlets[ 3 ].getReadableRef< unsigned char >();

		if ( aperture != 1 && aperture != 3 && aperture != 5 && aperture != 7 )
		{
			// (?) throwing an exception is not really a good idea in this case
			// ( exc requires re-setup, and that really would not help )
			// nonetheless, the user should be notified somehow?
			m_Block.getOutletHandle( "image_out" ).discard();
			return;
		}

		// (?) where to get the format from, C1 - C4 comes from channel order; 8U whatever comes from the template type
		// thankfully, the conversion thing itself is super easy
		cv::Mat matSrc( input.getWidth(), input.getHeight(), CV_8UC3, input.getData() );
		cv::Mat matDst( output.getWidth(), output.getHeight(), CV_16UC3, output.getData() );
		cv::Sobel( matSrc, matDst, 2, orderX, orderY, aperture, 1.0, 0.0, 4 );
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

void OcvSobelBlock::shutdown() {}