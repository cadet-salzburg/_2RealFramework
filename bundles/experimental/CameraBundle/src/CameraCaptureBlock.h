#pragma once
#include "_2RealBlock.h"
#include "CameraDeviceManager.h"

using namespace _2Real::bundle;

class VideoInputBlock : public Block
{
public:
	VideoInputBlock( ContextBlock & context );
	~VideoInputBlock() {}
	void					shutdown() {}
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
	double								m_iFps;
};