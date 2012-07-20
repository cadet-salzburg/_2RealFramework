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
		initDeviceList();
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
	
	m_2RealKinect->shutdown();
}

void OpenNIDeviceManager::initDeviceList()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	
	m_DevicesInUse.clear();
	for(unsigned int i=0; i<m_iNumDevices; i++)
	{
		m_DevicesInUse.push_back(DeviceItem(false));
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
		// setup device
		bool bResult = false;

		uint32_t resolution = IMAGE_COLOR_320X240;
		if(w == 640 && h == 480)
		{
			 resolution = IMAGE_COLOR_640X480;
		}

		bResult = m_2RealKinect->configure( deviceIdx,  COLORIMAGE, resolution  );

		if( bResult )
		{
			std::cout << "_2RealKinectWrapper Device " << deviceIdx << " started successfully!..." << std::endl;
		}
		m_2RealKinect->startGenerator( deviceIdx, COLORIMAGE );

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
		return true;
	}
	return false;
}

unsigned int OpenNIDeviceManager::getNumberOfConnectedDevices() 
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	return m_iNumDevices;
}


_2Real::ImageT<unsigned char> OpenNIDeviceManager::getRgbImage( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
	//	if(m_2RealKinect->isNewData(deviceIdx, COLORIMAGE))
		{
			int imageWidth = m_2RealKinect->getImageWidth( deviceIdx, COLORIMAGE );		
			int imageHeight = m_2RealKinect->getImageHeight( deviceIdx, COLORIMAGE );
			unsigned char* pixels = m_2RealKinect->getImageData( deviceIdx, COLORIMAGE ).get();

			m_DevicesInUse[deviceIdx].m_Image = _2Real::ImageT<unsigned char>( pixels, false, imageWidth, imageHeight, _2Real::ImageChannelOrder::RGB );
		}
	}
	catch ( ... )
	{
		std::cerr << "Couldn't get the rgb image" << std::endl;
	}

	return m_DevicesInUse[deviceIdx].m_Image;
}
