#include "SerialDeviceManager.h"
#include "ofSerial.h"
#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;


SerialDeviceManager::SerialDeviceManager() 
: ContextBlock()
, m_iSerialReferenceCount( 0 )
{
}


SerialDeviceManager::~SerialDeviceManager()
{
}

void SerialDeviceManager::setup( _2Real::bundle::BlockHandle &context )
{
	// nothing
}

void SerialDeviceManager::update()
{
	try
	{
		// nothing
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void SerialDeviceManager::shutdown()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	clearSerialList();
}

void SerialDeviceManager::registerSerialBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	if (m_iSerialReferenceCount == 0)
	{
		initSerialDeviceList();
	}

	// Increase reference counter
	m_iSerialReferenceCount++;
}

void SerialDeviceManager::unregisterSerialBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Decrease reference counter
	m_iSerialReferenceCount--;

	// If the reference counter reaches zero clear the Serial list and reset all important values
	if ( m_iSerialReferenceCount <= 0 )
	{
		m_iSerialReferenceCount = 0;
		clearSerialList();
	}
}

void SerialDeviceManager::clearSerialList()
{
	for (stdext::hash_map<std::string, DeviceItem*>::iterator it = m_SerialDevices.begin(); it != m_SerialDevices.end(); it++)
	{
		SerialDeviceManager::DeviceItem *pDeviceItem = it->second;
		delete pDeviceItem;
	}
	m_SerialDevices.clear();
}

SerialDeviceManager::DeviceItem	*SerialDeviceManager::getDeviceItem(const std::string identifier)
{
	stdext::hash_map<std::string, DeviceItem*>::const_iterator it = m_SerialDevices.find(identifier);
	if (it == m_SerialDevices.end())
	{
		return NULL;
	}
	SerialDeviceManager::DeviceItem *pDeviceItem = it->second;
	return pDeviceItem;
}

unsigned int SerialDeviceManager::getSerialPortCount()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	return m_SerialDevices.size();
}

bool SerialDeviceManager::isSerialDeviceRunning( const std::string identifier )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	stdext::hash_map<std::string, DeviceItem*>::const_iterator it = m_SerialDevices.find(identifier);
	if (it == m_SerialDevices.end())
	{
		return false;
	}
	SerialDeviceManager::DeviceItem *pDeviceItem = it->second;

	return pDeviceItem->m_bIsUsed;
}

vector<unsigned char> SerialDeviceManager::getSerialMessage( const std::string identifier )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Read message from Serial and write the message into an std::vector<unsigned char>
	vector<unsigned char> message;

	if ( isSerialDeviceRunning( identifier ) )
	{
		SerialDeviceManager::DeviceItem	*pDev = getDeviceItem(identifier);
		int length = pDev->m_pSerial->available();
		if (length <= 0)
		{
			return message;
		}
		unsigned char *buffer = new unsigned char[length];
		int read = pDev->m_pSerial->readBytes(buffer, length);
		if (read != length)
		{
			std::cout << "failed to read all " << length << " bytes from " << identifier << std::endl;
		}
		if (read <= 0)
		{
			delete[] buffer;
			return message;
		}
		length = read;

		for (int i=0; i<length; i++)
		{
			message.push_back(buffer[i]);
		}
		delete[] buffer;
	}
	else
	{
		std::cout << "failed to read from " << identifier << " as it is not open" << std::endl;
	}
	
	return message;
}

void SerialDeviceManager::sendSerialMessage( const std::string identifier, std::vector<unsigned char> &message )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	if ( isSerialDeviceRunning( identifier ) )
	{
		int length = message.size();
		if (length == 0)
		{
			return;
		}
		unsigned char *buffer = new unsigned char[length];
		for (std::vector<unsigned char>::iterator it = message.begin(); it != message.end(); it++)
		{
			*buffer = *it;
			buffer++;
		}
		buffer -= length;

		SerialDeviceManager::DeviceItem	*pDev = getDeviceItem(identifier);
		if (pDev->m_pSerial->writeBytes(buffer, length) != length)
		{
			std::cout << "failed to write " << length << " bytes to " << identifier << std::endl;
		}
		delete[] buffer;
	}
	else
	{
		std::cout << "failed to write to " << identifier << " as it is not open" << std::endl;
	}
}

void SerialDeviceManager::initSerialDeviceList()
{
	ofSerial ofs;
	std::vector<ofSerialDeviceInfo> devices = ofs.getDeviceList();

	for (std::vector<ofSerialDeviceInfo>::iterator it =  devices.begin(); it != devices.end(); it++)
	{
		ofSerialDeviceInfo devInfo = *it;
		// add port to list
		DeviceItem *pItem = new DeviceItem(devInfo.getDeviceName());
		pItem->m_pSerial = new ofSerial();
		m_SerialDevices[devInfo.getDeviceName()] = pItem;
	}
}

bool SerialDeviceManager::isSerialDeviceAvailable( const std::string identifier ) 
{
	return (getDeviceItem(identifier) != NULL);
}

bool SerialDeviceManager::isSerialDeviceFree( const std::string identifier ) 
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	stdext::hash_map<std::string, DeviceItem*>::const_iterator it = m_SerialDevices.find(identifier);
	if (it == m_SerialDevices.end())
	{
		return false;
	}
	SerialDeviceManager::DeviceItem *pDeviceItem = it->second;

	return !pDeviceItem->m_bIsUsed;
}


bool SerialDeviceManager::bindSerialDevice( const std::string identifier, unsigned int baudrate )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Only bind the device if the device is actually available and not used yet
	if ( isSerialDeviceFree( identifier ) )
	{
		SerialDeviceManager::DeviceItem	*pDev = getDeviceItem(identifier);
		if (pDev->m_pSerial->setup(identifier, baudrate))
		{
			pDev->m_bIsUsed = true;
			std::cout << "comport " << identifier << " opened with baud rate " << baudrate << std::endl;
			return true;
		}
		else
		{
			std::cout << "failed to open comport " << identifier << " with baud rate " << baudrate << std::endl;
			return false;
		}
	}
	return false;
}

// Unbind a MidiIn device via an index
void SerialDeviceManager::unbindSerialDevice( const std::string identifier )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Only bind the device if the device is actually available and not used yet
	if ( isSerialDeviceRunning( identifier ) )
	{
		SerialDeviceManager::DeviceItem	*pDev = getDeviceItem(identifier);
		pDev->m_pSerial->close();
		pDev->m_bIsUsed = false;
		std::cout << "comport " << identifier << " closed" << std::endl;
	}
}

SerialDeviceManager::DeviceItem::DeviceItem( std::string strDescription, bool bIsUsed) 
: m_strDescription ( strDescription )
, m_bIsUsed ( bIsUsed )
, m_pSerial (NULL)
{
}

SerialDeviceManager::DeviceItem::~DeviceItem()
{
	if (m_pSerial)
	{
		m_pSerial->close();
		delete m_pSerial;
	}
}
