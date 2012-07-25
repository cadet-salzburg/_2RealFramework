#include <iostream>
#include <string>
#include <sstream>
#include "KinectOpenNIBlockBase.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

KinectOpenNIBlockBase::KinectOpenNIBlockBase( ContextBlock & context )
	:Block()
{
	m_OpenNIDeviceManager = static_cast<OpenNIDeviceManager*>( &context );
}

KinectOpenNIBlockBase::~KinectOpenNIBlockBase()
{
}

void KinectOpenNIBlockBase::setGeneratorType(_2RealKinectWrapper::_2RealGenerator generatorType)
{
	m_GeneratorType = generatorType;
}

void KinectOpenNIBlockBase::setup( BlockHandle &block )
{
	try
	{
		// inlet handles
		m_DeviceIndexInletHandle = block.getInletHandle("DeviceIndex");
		m_WidthInletHandle = block.getInletHandle("Width");
		m_HeightInletHandle = block.getInletHandle("Height");
		m_FpsInletHandle = block.getInletHandle("Fps");
		m_IsMirroredInletHandle = block.getInletHandle("IsMirrored");
		
		m_iWidth = m_WidthInletHandle.getReadableRef<int>();
		m_iHeight = m_HeightInletHandle.getReadableRef<int>();
		m_iFps = m_FpsInletHandle.getReadableRef<int>();
		m_bIsMirrored = m_IsMirroredInletHandle.getReadableRef<bool>();
	
		// outlet handles
		m_ImageOutletHandle = block.getOutletHandle("ImageData" );
		m_WidthOutletHandle = block.getOutletHandle("Width" );
		m_HeightOutletHandle = block.getOutletHandle("Height" );

		m_WidthOutletHandle.getWriteableRef<int>() = 0;
		m_HeightOutletHandle.getWriteableRef<int>() = 0;

		m_iCurrentDevice = -1;	// no device set yet
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void KinectOpenNIBlockBase::update()
{
	try
	{
		if(m_OpenNIDeviceManager->getNumberOfConnectedDevices()<=0)	// if there is no cameras connected there is nothing todo so return
		{
			m_WidthOutletHandle.discard();
			m_HeightOutletHandle.discard();
			m_ImageOutletHandle.discard();
			return;
		}

		int deviceIndex = m_DeviceIndexInletHandle.getReadableRef<int>();
		int w = m_WidthInletHandle.getReadableRef<int>();
		int h = m_HeightInletHandle.getReadableRef<int>();
		int fps = m_FpsInletHandle.getReadableRef<int>();
		bool bIsMirrored = m_IsMirroredInletHandle.getReadableRef<bool>();
	
		// camera index changed
		if(deviceIndex != m_iCurrentDevice)
		{
			// unbind old cam
			if(m_iCurrentDevice>=0)
			{
				m_OpenNIDeviceManager->unbindGenerator( m_iCurrentDevice, m_GeneratorType );
				m_iCurrentDevice = -1;
			}

			if( m_OpenNIDeviceManager->bindGenerator( deviceIndex, m_GeneratorType, m_iWidth, m_iHeight, m_iFps ) )
			{
				m_iCurrentDevice = deviceIndex;
				m_WidthOutletHandle.getWriteableRef<int>() = m_OpenNIDeviceManager->getWidth(m_iCurrentDevice, m_GeneratorType);
				m_HeightOutletHandle.getWriteableRef<int>() = m_OpenNIDeviceManager->getHeight(m_iCurrentDevice, m_GeneratorType);
			}
		}

		// there is a camera so grab image or set new parameters on this device if there are
		if(m_iCurrentDevice>=0)  
		{
			// resolution changed
			if(w != m_iWidth || h != m_iHeight || fps != m_iFps)
			{
				m_iWidth  = w;
				m_iHeight = h;
				m_iFps = fps;
				//if(!m_OpenNIDeviceManager->setCameraParams(m_iCurrentDevice, m_iWidth, m_iHeight, m_iFps))	// failed ?
				//	m_iCurrentDevice = -1;
				//else
				//{
				//	m_WidthOutletHandle.getWriteableRef<int>() = m_OpenNIDeviceManager->getVideoWidth(m_iCurrentDevice);
				//	m_HeightOutletHandle.getWriteableRef<int>() = m_OpenNIDeviceManager->getVideoHeight(m_iCurrentDevice);
				//}
			}
			else if(bIsMirrored != m_bIsMirrored)
			{
				m_OpenNIDeviceManager->setMirrored(m_iCurrentDevice, m_GeneratorType, bIsMirrored);
				m_bIsMirrored = bIsMirrored;
			}

			//if( m_OpenNIDeviceManager->isDeviceRunning(m_iCurrentDevice))
			{
				m_ImageOutletHandle.getWriteableRef<_2Real::Image >() = m_OpenNIDeviceManager->getImage( m_iCurrentDevice, m_GeneratorType );
			}
			//else
			//{
			//	m_iCurrentDevice = -1;		// it was all running and ok bu through a rescan a change in the device list got noticed and so all devices have to quit
			//}
		}
		else
		{
			m_ImageOutletHandle.discard();
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void KinectOpenNIBlockBase::shutdown()
{
	std::cout << "shutdown block" << std::endl;
	if(m_iCurrentDevice>=0)
	{
		m_OpenNIDeviceManager->unbindGenerator( m_iCurrentDevice, m_GeneratorType );
		m_iCurrentDevice = -1;
	}
}
