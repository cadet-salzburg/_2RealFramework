#include "OpenNIDeviceManager.h"

#include <iostream>
#include <string>

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;
using namespace _2RealKinectWrapper;


OpenNIDeviceManager::OpenNIDeviceManager() : ContextBlock()
{
}

OpenNIDeviceManager::~OpenNIDeviceManager()
{
}

void OpenNIDeviceManager::setup( BlockHandle &context )
{
	try
	{
		m_2RealKinect = _2RealKinect::getInstance();
		std::cout << "_2RealKinectWrapper Version: " << m_2RealKinect->getVersion() << std::endl;
		m_iNumDevices = m_2RealKinect->getNumberOfDevices();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void OpenNIDeviceManager::update()
{
	try
	{
		rescanDeviceList();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void OpenNIDeviceManager::shutdown()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	for( unsigned int i = 0; i<m_DevicesInUse.size(); i++)
	{
		if(m_DevicesInUse[i].m_bIsUsed)
		{
		}
			
	}
}

void OpenNIDeviceManager::initDeviceList()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	
	m_DevicesInUse.clear();
	for(unsigned int i=0; i<m_iNumDevices; i++)
	{
		//m_DevicesInUse.push_back(DeviceItem(m_VideoInputContoller->getDeviceName(i), false));
	}
}

void OpenNIDeviceManager::rescanDeviceList()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	printf("rescanning\n");
	int numDevices = m_2RealKinect->getNumberOfDevices();
	if(numDevices != m_iNumDevices)
	{
		// reinit devices
		m_iNumDevices = numDevices;
	}
}

int	OpenNIDeviceManager::getFirstFreeDevices()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	for(unsigned int i=0; i<m_DevicesInUse.size(); i++)
	{
		if(!m_DevicesInUse[i].m_bIsUsed)
			return i;
	}
	return -1;	// no device is free
}

bool OpenNIDeviceManager::isDeviceRunning(const unsigned int deviceIdx)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	return true;
}

bool OpenNIDeviceManager::isDeviceAvailable(const unsigned int deviceIdx)
{
	return  (deviceIdx < m_DevicesInUse.size());
}

bool OpenNIDeviceManager::isDeviceFree(const unsigned int deviceIdx)
{
	if(isDeviceAvailable(deviceIdx))
		return  !m_DevicesInUse[deviceIdx].m_bIsUsed;
	else
	{
		return false;
	}
}

bool OpenNIDeviceManager::bindDevice(const unsigned int deviceIdx, int w, int h, int fps)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	if(isDeviceFree(deviceIdx))
	{
		// setup devices
		return m_DevicesInUse[deviceIdx].m_bIsUsed = true;
	}
	else
	{
		return false;
	}
}

void OpenNIDeviceManager::unbindDevice(const unsigned int deviceIdx)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	if(!isDeviceFree(deviceIdx))
	{
		// stop device
		m_DevicesInUse[deviceIdx].m_bIsUsed = false;
	}
}


bool OpenNIDeviceManager::setCameraParams(const unsigned int deviceIdx, int w, int h, int fps)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	if(!isDeviceFree(deviceIdx))
	{
		m_VideoInputContoller->stopDevice( deviceIdx );
		m_VideoInputContoller->setIdealFramerate(deviceIdx, fps);
		return m_DevicesInUse[deviceIdx].m_bIsUsed = m_VideoInputContoller->setupDevice( deviceIdx, w, h );
	}
	return false;
}

unsigned int OpenNIDeviceManager::getNumberOfConnectedDevices() 
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	return m_iNumDevices;
}


_2Real::ImageT<unsigned char> OpenNIDeviceManager::getPixels( const unsigned int deviceIdx )
{
	// this seems to be locked anyway by videoinput lib, and every device in our case is just grabbed by a single block
	try
	{
		if(m_VideoInputContoller->isFrameNew(deviceIdx))
		{
			//unsigned char *pixels = m_VideoInputContoller->getPixels( deviceIdx, true, true );

			m_DevicesInUse[deviceIdx].m_Image = _2Real::ImageT<unsigned char>( pixels, false,
				m_VideoInputContoller->getWidth( deviceIdx ), 
				m_VideoInputContoller->getHeight( deviceIdx ),
				_2Real::ImageChannelOrder::RGB );
		}
	}
	catch ( ... )
	{
		std::cerr << "Couldn't get the pixels" << std::endl;
	}

	return m_DevicesInUse[deviceIdx].m_Image;
}
