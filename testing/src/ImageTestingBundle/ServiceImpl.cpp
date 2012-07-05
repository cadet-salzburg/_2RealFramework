#include "ServiceImpl.h"

#include "_2RealBlock.h"

#include <iostream>

using namespace _2Real::bundle;
using _2Real::ImageT;
using _2Real::ImageChannelOrder;
using _2Real::Exception;

using std::cout;
using std::endl;

void ImageOut::setup( BlockHandle &context )
{
	try
	{
		m_Out = context.getOutletHandle( "image outlet" );

		ImageT< float > &outImg = m_Out.getWriteableRef< ImageT< float > >();
		outImg = ImageT< float >( 4, 3, ImageChannelOrder::RGBA );

		ImageT< float >::iterator it = outImg.iter();
		while( it.nextLine() )
		{
			while ( it.nextPixel() )
			{
				it.r() = 0.0f;
				it.g() = 0.0f;
				it.b() = 0.0f;
				it.a() = 0.0f;
			}
		}
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ImageOut::update()
{
	try
	{
		ImageT< float > &imgOut = m_Out.getWriteableRef< ImageT< float > >();
		ImageT< float >::iterator itOut = imgOut.iter();
		while( itOut.nextLine() )
		{
			while ( itOut.nextPixel() )
			{
				itOut.r() = float( rand()%255 + 1 ) / 255.0f;
				itOut.g() = float( rand()%255 + 1 ) / 255.0f;
				itOut.b() = float( rand()%255 + 1 ) / 255.0f;
				itOut.a() = float( rand()%255 + 1 ) / 255.0f;
			}
		}
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ImageOut::shutdown()
{
	try
	{
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ImageInOut::setup( BlockHandle &context )
{
	try
	{
		m_In = context.getInletHandle( "image inlet" );
		m_Out = context.getOutletHandle( "image outlet" );

		ImageT< float > &outImg = m_Out.getWriteableRef< ImageT< float > >();
		outImg = ImageT< float >( 640, 640, ImageChannelOrder::RGBA );

		ImageT< float >::iterator it = outImg.iter();
		while( it.nextLine() )
		{
			while ( it.nextPixel() )
			{
				it.r() = 0.0f;
				it.g() = 0.0f;
				it.b() = 0.0f;
				it.a() = 0.0f;
			}
		}
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ImageInOut::update()
{
	try
	{
		ImageT< float > &outImg = m_Out.getWriteableRef< ImageT< float > >();
		ImageT< float > const& inImg = m_In.getReadableRef< ImageT< float > >();

		ImageT< float >::iterator outIt = outImg.iter();
		ImageT< float >::const_iterator inIt = inImg.const_iter();

		//outImg = inImg;

		while( inIt.nextLine() && outIt.nextLine() )
		{
			while ( inIt.nextPixel() && outIt.nextPixel() )
			{
				outIt.r() = inIt.a() - 1.0f;
				outIt.g() = inIt.g() + 1.0f;
				outIt.b() = inIt.b() + 1.0f;
				outIt.a() = 100.0f;
			}
		}
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ImageInOut::shutdown()
{
	try
	{
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ImageIn::setup( BlockHandle &context )
{
	try
	{
		m_Counter = 0;
		m_In = context.getInletHandle( "image inlet" );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ImageIn::update()
{
	try
	{
		ImageT< float > const& inImg = m_In.getReadableRef< ImageT< float > >();
		ImageT< float >::const_iterator inIt = inImg.const_iter();

		while( inIt.nextLine() )
		{
			while ( inIt.nextPixel()  )
			{
				cout << inIt.r() << " " << inIt.g() << " " << inIt.b() << " " << inIt.a() << endl;
			}
		}
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ImageIn::shutdown()
{
	try
	{
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}