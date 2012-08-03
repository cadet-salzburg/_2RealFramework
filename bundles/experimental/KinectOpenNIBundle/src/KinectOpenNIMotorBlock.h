#pragma once
#include "_2RealBlock.h"
#include "OpenNIDeviceManager.h"

using namespace _2Real::bundle;

class KinectOpenNIMotorBlock : public Block
{
public:
					KinectOpenNIMotorBlock( ContextBlock & context );
	virtual			~KinectOpenNIMotorBlock();
	virtual void	shutdown();
	virtual void	update();
	virtual void	setup( BlockHandle &context );

protected:

	InletHandle								m_DeviceIndexInletHandle;
	InletHandle								m_AngleInletHandle;
	OutletHandle							m_AngleOutletHandle;

	OpenNIDeviceManager*					m_OpenNIDeviceManager;
	int										m_iCurrentDevice;
	int										m_iInputAngle;
	int										m_iCurrentAngle;
};