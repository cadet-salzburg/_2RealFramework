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
	//	rescanDevices();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void CameraDeviceManager::shutdown()
{
	for( int i = 0; i<m_DevicesInUse.size(); i++)
	{
		if(m_DevicesInUse[i].m_bIsUsed)
			m_VideoInputContoller->stopDevice( i );
	}
}

void CameraDeviceManager::initDeviceList()
{
	m_iNumDevices = m_VideoInputContoller->listDevices();	// rescan devices silently (true==silent, no console output)

	m_DevicesInUse.clear();
	for(int i=0; i<m_iNumDevices; i++)
	{
		m_DevicesInUse.push_back(DeviceItem(m_VideoInputContoller->getDeviceName(i), false));
	}
}

int	CameraDeviceManager::getFirstFreeDevices()
{
	for(int i=0; i<m_DevicesInUse.size(); i++)
	{
		if(!m_DevicesInUse[i].m_bIsUsed)
			return i;
	}
	return -1;	// no device is free
}

void CameraDeviceManager::rescanDeviceList()
{
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

bool CameraDeviceManager::bindDevice(const unsigned int deviceIdx)
{
	m_iNumDevices = m_VideoInputContoller->listDevices();
	if(isDeviceFree(deviceIdx))
	{
		bool ret = m_VideoInputContoller->setupDevice( deviceIdx );
		return m_DevicesInUse[deviceIdx].m_bIsUsed = ret;
	}
	else
	{
		return false;
	}
}

void CameraDeviceManager::unbindDevice(const unsigned int deviceIdx)
{
	if(!isDeviceFree(deviceIdx))
	{
		m_VideoInputContoller->stopDevice( deviceIdx );
		m_DevicesInUse[deviceIdx].m_bIsUsed = false;
	}
}

unsigned int CameraDeviceManager::getNumberOfConnectedDevices() const
{
	return m_iNumDevices;
}


_2Real::ImageT<unsigned char> CameraDeviceManager::getPixels( const unsigned int deviceIdx )
{
	try
	{
		if(m_VideoInputContoller->isFrameNew(deviceIdx))
		{
			unsigned char *pixels = m_VideoInputContoller->getPixels( deviceIdx, true, true );
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

int CameraDeviceManager::getVideoWidth( const unsigned int  deviceIdx )
{
	return m_VideoInputContoller->getWidth( deviceIdx );
}

int CameraDeviceManager::getVideoHeight( const unsigned int  deviceIdx )
{
	return m_VideoInputContoller->getHeight( deviceIdx );
}
