#pragma once

#include "_2RealBlock.h"

class OcvGaussianBlurBlock : public _2Real::bundle::Block
{

public:

	OcvGaussianBlurBlock();
	~OcvGaussianBlurBlock();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::BlockHandle			m_Block;
	unsigned int						m_OutWidth;
	unsigned int						m_OutHeight;
	_2Real::ImageType					m_OutImageType;
	_2Real::ImageChannelOrder			m_OutChannelOrder;

};