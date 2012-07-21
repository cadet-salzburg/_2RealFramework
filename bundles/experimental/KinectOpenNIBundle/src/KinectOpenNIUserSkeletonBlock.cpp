#include <iostream>
#include <string>
#include <sstream>
#include "KinectOpenNIUserSkeletonBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

KinectOpenNIUserSkeletonBlock::KinectOpenNIUserSkeletonBlock( ContextBlock & context )
	:Block()
{
	m_OpenNIDeviceManager = static_cast<OpenNIDeviceManager*>( &context );
}

KinectOpenNIUserSkeletonBlock::~KinectOpenNIUserSkeletonBlock()
{
}

void KinectOpenNIUserSkeletonBlock::setup( BlockHandle &block )
{
	try
	{
		// inlet handles
		m_DeviceIndexInletHandle = block.getInletHandle("DeviceIndex");
		m_WidthInletHandle = block.getInletHandle("Width");
		m_HeightInletHandle = block.getInletHandle("Height");
		m_FpsInletHandle = block.getInletHandle("Fps");

		m_iWidth = m_WidthInletHandle.getReadableRef<int>();
		m_iHeight = m_HeightInletHandle.getReadableRef<int>();
		m_iFps = m_FpsInletHandle.getReadableRef<int>();

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

void KinectOpenNIUserSkeletonBlock::update()
{
	try
	{
		if(m_OpenNIDeviceManager->getNumberOfConnectedDevices()<=0)	// if there is no cameras connected there is nothing todo so return
			return;

		int deviceIndex = m_DeviceIndexInletHandle.getReadableRef<int>();
		int w = m_WidthInletHandle.getReadableRef<int>();
		int h = m_HeightInletHandle.getReadableRef<int>();
		int fps = m_FpsInletHandle.getReadableRef<int>();

		// camera index changed
		if(deviceIndex != m_iCurrentDevice)
		{
			// unbind old cam
			if(m_iCurrentDevice>=0)
			{
				m_OpenNIDeviceManager->unbindGenerator( m_iCurrentDevice,_2RealKinectWrapper::USERIMAGE );
				m_iCurrentDevice = -1;
			}

			if( m_OpenNIDeviceManager->bindGenerator( deviceIndex, _2RealKinectWrapper::USERIMAGE, m_iWidth, m_iHeight, m_iFps ) )
			{
				m_iCurrentDevice = deviceIndex;
				m_WidthOutletHandle.getWriteableRef<int>() = m_OpenNIDeviceManager->getWidth(m_iCurrentDevice, _2RealKinectWrapper::USERIMAGE);
				m_HeightOutletHandle.getWriteableRef<int>() = m_OpenNIDeviceManager->getHeight(m_iCurrentDevice, _2RealKinectWrapper::USERIMAGE);
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

			//if( m_OpenNIDeviceManager->isDeviceRunning(m_iCurrentDevice))
			{
				m_ImageOutletHandle.getWriteableRef<_2Real::ImageT<unsigned char> >() = m_OpenNIDeviceManager->getImage( m_iCurrentDevice, _2RealKinectWrapper::USERIMAGE );
			}
			//else
			//{
			//	m_iCurrentDevice = -1;		// it was all running and ok bu through a rescan a change in the device list got noticed and so all devices have to quit
			//}
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void KinectOpenNIUserSkeletonBlock::shutdown()
{
	std::cout << "shutdown block" << std::endl;
	if(m_iCurrentDevice>=0)
	{
		m_OpenNIDeviceManager->unbindGenerator( m_iCurrentDevice, _2RealKinectWrapper::USERIMAGE );
		m_iCurrentDevice = -1;
	}
}

