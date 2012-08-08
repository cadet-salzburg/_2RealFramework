#include "ShapeWrapDeviceManager.h"
#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;


ShapeWrapDeviceManager::ShapeWrapDeviceManager() 
: ContextBlock()
, m_iShapeWrapReferenceCount( 0 )
{
}


ShapeWrapDeviceManager::~ShapeWrapDeviceManager()
{
}

void ShapeWrapDeviceManager::setup( _2Real::bundle::BlockHandle &context )
{
	// nothing
}

void ShapeWrapDeviceManager::update()
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

void ShapeWrapDeviceManager::shutdown()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	clearShapeWrapList();
}

void ShapeWrapDeviceManager::registerShapeWrapBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	if (m_iShapeWrapReferenceCount == 0)
	{
		initShapeWrapDeviceList();
	}

	// Increase reference counter
	m_iShapeWrapReferenceCount++;
}

void ShapeWrapDeviceManager::unregisterShapeWrapBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Decrease reference counter
	m_iShapeWrapReferenceCount--;

	// If the reference counter reaches zero clear the ShapeWrap list and reset all important values
	if ( m_iShapeWrapReferenceCount <= 0 )
	{
		m_iShapeWrapReferenceCount = 0;
		clearShapeWrapList();
	}
}

void ShapeWrapDeviceManager::clearShapeWrapList()
{
	for (stdext::hash_map<std::string, DeviceItem*>::iterator it = m_ShapeWrapDevices.begin(); it != m_ShapeWrapDevices.end(); it++)
	{
		ShapeWrapDeviceManager::DeviceItem *pDeviceItem = it->second;
		delete pDeviceItem;
	}
	m_ShapeWrapDevices.clear();
}

ShapeWrapDeviceManager::DeviceItem	*ShapeWrapDeviceManager::getDeviceItem(const std::string identifier)
{
	stdext::hash_map<std::string, DeviceItem*>::const_iterator it = m_ShapeWrapDevices.find(identifier);
	if (it == m_ShapeWrapDevices.end())
	{
		return NULL;
	}
	ShapeWrapDeviceManager::DeviceItem *pDeviceItem = it->second;
	return pDeviceItem;
}

unsigned int ShapeWrapDeviceManager::getShapeWrapPortCount()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	return m_ShapeWrapDevices.size();
}

bool ShapeWrapDeviceManager::isShapeWrapDeviceRunning( const std::string identifier )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	stdext::hash_map<std::string, DeviceItem*>::const_iterator it = m_ShapeWrapDevices.find(identifier);
	if (it == m_ShapeWrapDevices.end())
	{
		return false;
	}
	ShapeWrapDeviceManager::DeviceItem *pDeviceItem = it->second;

	return pDeviceItem->m_bIsUsed;
}

vector<unsigned char> ShapeWrapDeviceManager::getShapeWrapMessage( const std::string identifier )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Read message from ShapeWrap and write the message into an std::vector<unsigned char>
	vector<unsigned char> message;

	if ( isShapeWrapDeviceRunning( identifier ) )
	{
		ShapeWrapDeviceManager::DeviceItem	*pDev = getDeviceItem(identifier);

		// TODO
	}
	else
	{
		std::cout << "failed to read from " << identifier << " as it is not open" << std::endl;
	}
	
	return message;
}

void ShapeWrapDeviceManager::sendShapeWrapMessage( const std::string identifier, std::vector<unsigned char> &message )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	if ( isShapeWrapDeviceRunning( identifier ) )
	{
		// TODO
	}
	else
	{
		std::cout << "failed to write to " << identifier << " as it is not open" << std::endl;
	}
}

void ShapeWrapDeviceManager::initShapeWrapDeviceList()
{
	// TODO
}

bool ShapeWrapDeviceManager::isShapeWrapDeviceAvailable( const std::string identifier ) 
{
	return (getDeviceItem(identifier) != NULL);
}

bool ShapeWrapDeviceManager::isShapeWrapDeviceFree( const std::string identifier ) 
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	stdext::hash_map<std::string, DeviceItem*>::const_iterator it = m_ShapeWrapDevices.find(identifier);
	if (it == m_ShapeWrapDevices.end())
	{
		return false;
	}
	ShapeWrapDeviceManager::DeviceItem *pDeviceItem = it->second;

	return !pDeviceItem->m_bIsUsed;
}


bool ShapeWrapDeviceManager::bindShapeWrapDevice( const std::string identifier, unsigned int baudrate )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Only bind the device if the device is actually available and not used yet
	if ( isShapeWrapDeviceFree( identifier ) )
	{
		ShapeWrapDeviceManager::DeviceItem	*pDev = getDeviceItem(identifier);
		// TODO
	}
	return false;
}

// Unbind a MidiIn device via an index
void ShapeWrapDeviceManager::unbindShapeWrapDevice( const std::string identifier )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Only bind the device if the device is actually available and not used yet
	if ( isShapeWrapDeviceRunning( identifier ) )
	{
		// TODO
	}
}

ShapeWrapDeviceManager::DeviceItem::DeviceItem( std::string strDescription, bool bIsUsed) 
: m_strDescription ( strDescription )
, m_bIsUsed ( bIsUsed )
// , m_pShapeWrap (NULL)
{
}

ShapeWrapDeviceManager::DeviceItem::~DeviceItem()
{
//	if (m_pShapeWrap)
//	{
//		delete m_pShapeWrap;
//	}
}
