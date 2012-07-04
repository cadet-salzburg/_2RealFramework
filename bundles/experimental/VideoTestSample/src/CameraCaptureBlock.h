#pragma once
#include "_2RealBlock.h"
#include "CameraDeviceManager.h"

class VideoInputBlock : public _2Real::bundle::Block
{
public:
	VideoInputBlock( _2Real::bundle::ContextBlock & context );
	~VideoInputBlock() {}
	void shutdown() {}
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:
	_2Real::bundle::ParameterHandle			m_DeviceIndexHandle;
	_2Real::bundle::OutletHandle			m_ImageOutletHandle;
	CameraDeviceManager*			m_CameraDeviceManager;
};