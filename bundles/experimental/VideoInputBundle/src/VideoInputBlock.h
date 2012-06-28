#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"
#include "CameraDeviceManager.h"

class VideoInputBlock : public _2Real::Block
{
public:
	VideoInputBlock( _2Real::ContextBlock & context );
	~VideoInputBlock() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );

private:
	_2Real::InletHandle				m_DeviceIndexHandle;
	_2Real::OutletHandle			m_ImageOutletHandle;
	CameraDeviceManager*			m_CameraDeviceManager;
};