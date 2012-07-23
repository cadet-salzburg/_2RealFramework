#pragma once
#include "_2RealBlock.h"
#include "OpenNIDeviceManager.h"

using namespace _2Real::bundle;

class KinectOpenNIBlockBase : public Block
{
public:
					KinectOpenNIBlockBase( ContextBlock & context );
	virtual			~KinectOpenNIBlockBase();
	virtual void	shutdown();
	virtual void	update();
	virtual void	setup( BlockHandle &context );
	void			setGeneratorType(_2RealKinectWrapper::_2RealGenerator generatorType);

protected:
	InletHandle								m_DeviceIndexInletHandle;
	InletHandle								m_WidthInletHandle;
	InletHandle								m_HeightInletHandle;
	InletHandle								m_FpsInletHandle;
	InletHandle								m_IsMirroredInletHandle;
	OutletHandle							m_ImageOutletHandle;
	OutletHandle							m_WidthOutletHandle;
	OutletHandle							m_HeightOutletHandle;
	OpenNIDeviceManager*					m_OpenNIDeviceManager;
	_2RealKinectWrapper::_2RealGenerator	m_GeneratorType;
	int										m_iCurrentDevice;
	int										m_iWidth;
	int										m_iHeight;
	int										m_iFps;
	bool									m_bIsMirrored;
};