#pragma once
#include "_2RealBlock.h"
#include "CameraDeviceManager.h"

using namespace _2Real::bundle;

class CameraCaptureBlock : public Block
{
public:
	CameraCaptureBlock( ContextBlock & context );
	~CameraCaptureBlock();
	void					shutdown();
	void					update();
	void					setup( BlockHandle &context );

private:

	InletHandle							m_DeviceIndexHandle;
	InletHandle							m_WidthHandle;
	InletHandle							m_HeightHandle;
	InletHandle							m_FpsHandle;
	OutletHandle						m_ImageOutletHandle;
	CameraDeviceManager*				m_CameraDeviceManager;
	int									m_iCurrentCamera;
	int									m_iWidth;
	int									m_iHeight;
	int									m_iFps;
};