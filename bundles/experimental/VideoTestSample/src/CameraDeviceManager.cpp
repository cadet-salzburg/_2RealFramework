#include "CameraDeviceManager.h"

#include <iostream>
#include <string>

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

int CameraDeviceManager::m_NumDevices = 0;

void CameraDeviceManager::setup( BlockHandle &context )
{
	try
	{
		m_NumDevices = m_VideoInputContoller.listDevices();
		for( int idx = 0; idx < m_NumDevices; ++idx )
		{
			m_VideoInputContoller.setupDevice( idx );
		}
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

int	 CameraDeviceManager::getNumberOfDevices() const
{
	return m_NumDevices;
}

bool CameraDeviceManager::isDeviceAvailable( const unsigned int deviceIdx )
{
	try
	{
		return  !m_VideoInputContoller.isDeviceSetup( deviceIdx );
	}
	catch ( ... )
	{
		std::cerr << "The device index is out of bounds" << std::endl;
	}
}

_2Real::ImageT<unsigned char> CameraDeviceManager::getPixels( const unsigned int deviceIdx )
{
	try
	{
		unsigned char *pixels = m_VideoInputContoller.getPixels( deviceIdx );
		int w =  m_VideoInputContoller.getWidth( deviceIdx );
		int h  = m_VideoInputContoller.getHeight( deviceIdx );
		_2Real::ImageT<unsigned char> img( pixels, false, w, h, _2Real::ImageChannelOrder::RGB );
		return img;
	}
	catch ( ... )
	{
		std::cerr << "Couldn't get the pixels" << std::endl;
	}
}