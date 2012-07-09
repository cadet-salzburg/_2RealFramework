#include "CameraDeviceManager.h"

#include <iostream>
#include <string>

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;


CameraDeviceManager::CameraDeviceManager()
	:ContextBlock(),
	 m_CurrentDeviceIndex( 0 )
{

}

CameraDeviceManager::~CameraDeviceManager()
{
	
}

void CameraDeviceManager::setup( BlockHandle &context )
{
	try
	{
		m_NumDevices = m_VideoInputContoller.listDevices();
		if ( m_NumDevices >= 1)
			m_VideoInputContoller.setupDevice( 0 );
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

	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

int	 CameraDeviceManager::getNumberOfConnectedDevices() const
{
	return m_NumDevices;
}

bool CameraDeviceManager::deviceIsSetup( const unsigned int deviceIdx )
{
	try
	{
		return  !m_VideoInputContoller.isDeviceSetup( deviceIdx );
	}
	catch ( ... )
	{
		std::cerr << "The device index is out of bounds" << std::endl;
	}
	return false;
}

void CameraDeviceManager::switchToDevice( const unsigned int deviceIdx )
{
	try
	{
		if ( deviceIdx != m_CurrentDeviceIndex )
		{
			m_VideoInputContoller.stopDevice( m_CurrentDeviceIndex );
			m_VideoInputContoller.setupDevice( deviceIdx );
			m_CurrentDeviceIndex = deviceIdx;
		}
	}
	catch ( ... )
	{
		std::cerr << "The device index is out of bounds" << std::endl;
	}
}

_2Real::ImageT<unsigned char> CameraDeviceManager::getPixels( const unsigned int deviceIdx )
{
	_2Real::ImageT<unsigned char> img;

	try
	{
		if(m_VideoInputContoller.isFrameNew(deviceIdx))
		{
			unsigned char *pixels = m_VideoInputContoller.getPixels( deviceIdx, true, true );
			int w =  m_VideoInputContoller.getWidth( deviceIdx );
			int h  = m_VideoInputContoller.getHeight( deviceIdx );
			img = _2Real::ImageT<unsigned char>( pixels, false, w, h, _2Real::ImageChannelOrder::RGB );
		}
	}
	catch ( ... )
	{
		std::cerr << "Couldn't get the pixels" << std::endl;
	}

	return img;
}

int CameraDeviceManager::getVideoWidth()
{
	return m_VideoInputContoller.getWidth( m_CurrentDeviceIndex );
}

int CameraDeviceManager::getVideoHeight()
{
	return m_VideoInputContoller.getHeight( m_CurrentDeviceIndex );
}
