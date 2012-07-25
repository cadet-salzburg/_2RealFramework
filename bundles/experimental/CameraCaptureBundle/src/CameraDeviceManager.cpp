#include "CameraDeviceManager.h"

#include <iostream>
#include <string>

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;


CameraDeviceManager::CameraDeviceManager() : ContextBlock()
{
	// this is super weird, videoinputlib in multithreaded environment set coinitializeex for multithread mode and complain that another library already did
	// neither _2Real nor Poco use com interfaces so not sure why this next line is needed but otherwise the whole thing won't work, sometimes I had coding ;-(
	CoUninitialize();
	m_VideoInputContoller = new videoInput();
}

CameraDeviceManager::~CameraDeviceManager()
{
	delete m_VideoInputContoller;
}

void CameraDeviceManager::setup( BlockHandle &context )
{
	try
	{
		initDeviceList();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void CameraDeviceManager::update()
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

void CameraDeviceManager::shutdown()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	for( unsigned int i = 0; i<m_DevicesInUse.size(); i++)
	{
		if(m_DevicesInUse[i].m_bIsUsed)
			m_VideoInputContoller->stopDevice( i );
	}
}

void CameraDeviceManager::initDeviceList()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	m_iNumDevices = m_VideoInputContoller->listDevices();	// rescan devices silently (true==silent, no console output)

	m_DevicesInUse.clear();
	for(unsigned int i=0; i<m_iNumDevices; i++)
	{
		m_DevicesInUse.push_back(DeviceItem(m_VideoInputContoller->getDeviceName(i), false));
	}
}

void CameraDeviceManager::rescanDeviceList()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	printf("rescanning\n");
	int numDevices = m_VideoInputContoller->listDevices(true);	// rescan devices silently (true==silent, no console output)
	if(numDevices != m_iNumDevices)
	{
		// stop all devices in the graph
		for( unsigned int i = 0; i<m_DevicesInUse.size(); i++)
		{
			if(m_DevicesInUse[i].m_bIsUsed)
				m_VideoInputContoller->stopDevice( i );
		}

		initDeviceList();		// reinit list
		m_iNumDevices = numDevices;
	}
}

int	CameraDeviceManager::getFirstFreeDevices()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	for(unsigned int i=0; i<m_DevicesInUse.size(); i++)
	{
		if(!m_DevicesInUse[i].m_bIsUsed)
			return i;
	}
	return -1;	// no device is free
}

bool CameraDeviceManager::isDeviceRunning(const unsigned int deviceIdx)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	return m_VideoInputContoller->isDeviceSetup(deviceIdx);
}

bool CameraDeviceManager::isDeviceAvailable(const unsigned int deviceIdx)
{
	return  (deviceIdx < m_DevicesInUse.size());
}

bool CameraDeviceManager::isDeviceFree(const unsigned int deviceIdx)
{
	if(isDeviceAvailable(deviceIdx))
		return  !m_DevicesInUse[deviceIdx].m_bIsUsed;
	else
	{
		return false;
	}
}

bool CameraDeviceManager::bindDevice(const unsigned int deviceIdx, int w, int h, int fps)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	if(isDeviceFree(deviceIdx))
	{
		m_VideoInputContoller->setIdealFramerate(deviceIdx, fps);
		return m_DevicesInUse[deviceIdx].m_bIsUsed = m_VideoInputContoller->setupDevice( deviceIdx, w, h );
	}
	else
	{
		return false;
	}
}

void CameraDeviceManager::unbindDevice(const unsigned int deviceIdx)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	if(!isDeviceFree(deviceIdx))
	{
		m_VideoInputContoller->stopDevice( deviceIdx );
		m_DevicesInUse[deviceIdx].m_bIsUsed = false;
	}
}


bool CameraDeviceManager::setCameraParams(const unsigned int deviceIdx, int w, int h, int fps)
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

unsigned int CameraDeviceManager::getNumberOfConnectedDevices() 
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	return m_iNumDevices;
}


_2Real::Image& CameraDeviceManager::getPixels( const unsigned int deviceIdx )
{
	// this seems to be locked anyway by videoinput lib, and every device in our case is just grabbed by a single block
	try
	{
		if(m_VideoInputContoller->isFrameNew(deviceIdx))
		{
			unsigned char *pixels = m_VideoInputContoller->getPixels( deviceIdx, true, true );
			m_DevicesInUse[deviceIdx].m_Image = _2Real::Image( pixels, false,
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

int CameraDeviceManager::getVideoWidth( const unsigned int  deviceIdx )
{
	return m_VideoInputContoller->getWidth( deviceIdx );
}

int CameraDeviceManager::getVideoHeight( const unsigned int  deviceIdx )
{
	return m_VideoInputContoller->getHeight( deviceIdx );
}
