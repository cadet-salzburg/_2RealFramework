#include "CameraCaptureBlock.h"

#include <iostream>
#include <string>

using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

void CameraCaptureBlock::setup( BlockHandle &context )
{
	try
	{
		m_DoubleInletHandle = context.getInletHandle( "doubleInlet" );
		m_DoubleOutletHandle = context.getOutletHandle( "doubleOutlet" );
		m_ImageOutletHandle = context.getOutletHandle( "imageOutlet" );
		m_Image = _2Real::ImageT<unsigned char>(320,240, _2Real::ImageChannelOrder::RGBA);
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void CameraCaptureBlock::update()
{
	try
	{
		static int i=0;
		static float f=0;

		m_DoubleOutletHandle.getWriteableRef<double>()=m_DoubleInletHandle.getReadableRef<double>();

		_2Real::ImageT<unsigned char>::iterator iter  = m_Image.iter();
		while(iter.nextLine())
		while(iter.nextPixel())
		{
			iter.r() = rand()%255;
			iter.g() = rand()%255;
			iter.b() = rand()%255;
			iter.a() = rand()%255;
		}

		m_ImageOutletHandle.getWriteableRef<_2Real::ImageT<unsigned char>>() = m_Image;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};