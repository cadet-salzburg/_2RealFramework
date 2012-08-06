#pragma once

#include "_2RealBlock.h"
//#include "_2RealDatatypes.h"

class FaceFeaturesBlock : public _2Real::bundle::Block
{

public:

	FaceFeaturesBlock();
	~FaceFeaturesBlock();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	unsigned int m_bpp;
	unsigned int m_width;
	unsigned int m_height;

	unsigned int				m_numChannels;
	_2Real::ImageType			m_imageType;
	_2Real::ImageChannelOrder	m_channelOrder;

	_2Real::bundle::BlockHandle		m_Block;
	_2Real::bundle::InletHandle		m_imageIn;
	_2Real::bundle::OutletHandle	m_facesOut;

	void reinit( unsigned int width, unsigned int height, unsigned int bpp );
};