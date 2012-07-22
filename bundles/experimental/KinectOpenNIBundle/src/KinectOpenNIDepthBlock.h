#pragma once
#include "_2RealBlock.h"
#include "OpenNIDeviceManager.h"

using namespace _2Real::bundle;

class KinectOpenNIDepthBlock : public Block
{
public:
	KinectOpenNIDepthBlock( ContextBlock & context );
	~KinectOpenNIDepthBlock();
	void					shutdown();
	void					update();
	void					setup( BlockHandle &context );

private:

	InletHandle							m_DeviceIndexInletHandle;
	InletHandle							m_WidthInletHandle;
	InletHandle							m_HeightInletHandle;
	InletHandle							m_FpsInletHandle;
	InletHandle							m_IsMirroredInletHandle;
	OutletHandle						m_ImageOutletHandle;
	OutletHandle						m_WidthOutletHandle;
	OutletHandle						m_HeightOutletHandle;
	OpenNIDeviceManager*				m_OpenNIDeviceManager;
	int									m_iCurrentDevice;
	int									m_iWidth;
	int									m_iHeight;
	int									m_iFps;
	bool								m_bIsMirrored;
};