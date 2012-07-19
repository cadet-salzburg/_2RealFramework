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

	InletHandle							m_DeviceIndexInletHandle;
	InletHandle							m_WidthInletHandle;
	InletHandle							m_HeightInletHandle;
	InletHandle							m_FpsInletHandle;
	OutletHandle						m_ImageOutletHandle;
	OutletHandle						m_WidthOutletHandle;
	OutletHandle						m_HeightOutletHandle;
	CameraDeviceManager*				m_CameraDeviceManager;
	int									m_iCurrentCamera;
	int									m_iWidth;
	int									m_iHeight;
	int									m_iFps;
};