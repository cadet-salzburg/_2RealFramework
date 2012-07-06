#pragma once

#include "_2RealBlock.h"

class CameraCaptureBlock : public _2Real::bundle::Block
{

public:

	CameraCaptureBlock( _2Real::bundle::ContextBlock & context ) : Block() {}
	~CameraCaptureBlock() {}
	void shutdown() {}
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::InletHandle				m_DoubleInletHandle;
	_2Real::bundle::OutletHandle			m_DoubleOutletHandle;
	_2Real::bundle::OutletHandle			m_ImageOutletHandle;

	_2Real::ImageT<unsigned char>			m_Image;
};