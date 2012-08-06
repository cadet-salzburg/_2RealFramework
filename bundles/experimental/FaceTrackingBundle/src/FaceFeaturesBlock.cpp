#include "_2RealDatatypes.h"
#include "FaceFeaturesBlock.h"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

FaceFeaturesBlock::FaceFeaturesBlock() : 
	Block(), 
	m_bpp( 0 ),
	m_width( 0 ),
	m_height( 0 ),
	m_numChannels( 0 ),
	m_imageType( ImageType::UNSIGNED_BYTE ),
	m_channelOrder( ImageChannelOrder::RGB )
{}

FaceFeaturesBlock::~FaceFeaturesBlock()
{}

void FaceFeaturesBlock::reinit( unsigned int width, unsigned int height, unsigned int bpp )
{
	//TODO: reallocate cvImage

	m_width = width;
	m_height = height;
	m_bpp = bpp;

	std::cout << "resolution set to " << m_width << " x " << m_height << " x " << m_bpp << std::endl;
}

void FaceFeaturesBlock::setup( BlockHandle &block )
{
	try
	{
		std::cout << "SETUP" << std::endl;

		m_Block = block;
		m_imageIn = m_Block.getInletHandle( "image_in" );
		m_facesOut = m_Block.getOutletHandle( "faces_out" );
		
		const Image &image = m_imageIn.getReadableRef< Image >();

		m_numChannels = image.getNumberOfChannels();
		m_imageType = image.getImageType();
		m_channelOrder = image.getChannelOrder();

		std::cout << "number of channels: " << m_numChannels << std::endl;
		std::cout << "imageType : " << m_imageType << std::endl;
		std::cout << "channel order: " << m_channelOrder << std::endl;

		this->reinit( image.getWidth(), image.getHeight(), image.getBitsPerPixel() );
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

void FaceFeaturesBlock::update()
{
	try
	{
		std::cout << "UPDATE" << std::endl;

		const Image &image = m_imageIn.getReadableRef< Image >();

		if( image.getNumberOfChannels() != m_numChannels )
		{
			std::cout << "number of channels changed from " << m_numChannels << " to " << image.getNumberOfChannels() << std::endl;
			m_numChannels = image.getNumberOfChannels();
		}

		if( image.getImageType() != m_imageType )
		{
			std::cout << "imageType changed from " << m_imageType << " to " << image.getImageType() << std::endl;
			m_imageType = image.getImageType();

			//test
			if( ImageType::DOUBLE != m_imageType )
				std::cout << "not double" << std::endl;
			//------
		}

		if( image.getChannelOrder() != m_channelOrder )
		{
			std::cout << "channel order changed from " << m_channelOrder << " to " << image.getChannelOrder() << std::endl;
			m_channelOrder = image.getChannelOrder();

			//test
			if( ImageChannelOrder::RGBA != m_channelOrder )
				std::cout << "not RGBA" << std::endl;
			//------
		}

		if( image.getWidth() != m_width ||
			image.getHeight() != m_height ||
			image.getBitsPerPixel() != m_bpp )
		{
			this->reinit( image.getWidth(), image.getHeight(), image.getBitsPerPixel() );
		}

		std::vector< Space2D > faces = m_facesOut.getWriteableRef< std::vector< Space2D > >();

		m_facesOut.discard();
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

void FaceFeaturesBlock::shutdown() {}