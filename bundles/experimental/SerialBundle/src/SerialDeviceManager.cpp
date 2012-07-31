#include "SerialDeviceManager.h"

#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;


SerialDeviceManager::SerialDeviceManager(void) : 
	ContextBlock(),
	m_iSerialReferenceCount( 0 ),
	m_iNumSerialDevices( 0 )
{
}


SerialDeviceManager::~SerialDeviceManager(void)
{
	clearSerialList();
}

void SerialDeviceManager::setup( _2Real::bundle::BlockHandle &context )
{
	
}

void SerialDeviceManager::update()
{
	try
	{
		// TODO
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
	try
	{
	/*
			for ( unsigned int i = 0; i < m_SerialDevices.size(); i++ )
			{
				if ( m_SerialDevices[i].m_bIsUsed )
					m_SerialDevices[i].m_Serial->closePort();
			}
	*/
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void SerialDeviceManager::registerSerialBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

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
		m_iNumSerialDevices = 0;
		clearSerialList();
	}
}

void SerialDeviceManager::clearSerialList()
{
	m_SerialDevices.clear();
}

/*
bool SerialDeviceManager::bindSerialDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Only bind the device if the device is actually available and not used yet
	if ( isSerialDeviceFree( deviceIdx ) )
	{
		try
		{
			// Open the Serial port
			m_SerialDevices[deviceIdx].m_Serial->openPort( deviceIdx );
			// Set the isUsed flag of this device to true
			m_SerialDevices[deviceIdx].m_bIsUsed = true;
			return true;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			// Set the isUsed flag of that device back to false if something went wrong while opening the port
			m_SerialOutDevicesInUse[deviceIdx].m_bIsUsed = false;
			return false;
		}
	}
}

bool SerialDeviceManager::bindSerialOutDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Only bind the device if the device is actually available and not used yet
	if ( isSerialOutDeviceFree( deviceIdx ) )
	{
		try
		{
			// Open the SerialOut port
			m_SerialOutDevicesInUse[deviceIdx].m_Serial->openPort( deviceIdx );
			// Set the isUsed flag of this device to true
			m_SerialOutDevicesInUse[deviceIdx].m_bIsUsed = true;
			return true;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			// Set the isUsed flag of that device back to false if something went wrong while opening the port
			m_SerialOutDevicesInUse[deviceIdx].m_bIsUsed = false;
			return false;
		}
	}
}

void SerialDeviceManager::unbindSerialDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Unbind the device if it is available and running
	if ( isSerialDeviceRunning( deviceIdx ) )
	{
		try
		{
			// Close the Serial port
			m_SerialDevices[deviceIdx].m_Serial->closePort();
			// Set the isUsed flag to false
			m_SerialDevices[deviceIdx].m_bIsUsed = false;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			// Set the isUsed flag to false if something went wrong while closing the port
			m_SerialDevices[deviceIdx].m_bIsUsed = false;
		}
	}
}

void SerialDeviceManager::unbindSerialOutDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Unbind the device if it is available and running
	if ( isSerialOutDeviceRunning( deviceIdx ) )
	{
		try
		{
			// Close the SerialOut port
			m_SerialOutDevicesInUse[deviceIdx].m_Serial->closePort();
			// Set the isUsed flag to false
			m_SerialOutDevicesInUse[deviceIdx].m_bIsUsed = false;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			// Set the isUsed flag to false if something went wrong while closing the port
			m_SerialOutDevicesInUse[deviceIdx].m_bIsUsed = false;
		}
	}
}

unsigned int SerialDeviceManager::getSerialPortCount()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	return m_iNumSerialDevices;
}

unsigned int SerialDeviceManager::getSerialOutPortCount()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	return m_iNumSerialOutDevices;
}

bool SerialDeviceManager::isSerialDeviceRunning( const unsigned int deviceIdx )
{
	return isSerialDeviceAvailable( deviceIdx ) && m_SerialDevices[deviceIdx].m_bIsUsed;
}

bool SerialDeviceManager::isSerialOutDeviceRunning( const unsigned int deviceIdx )
{
	return isSerialOutDeviceAvailable( deviceIdx ) && m_SerialOutDevicesInUse[deviceIdx].m_bIsUsed;
}

vector<unsigned char> SerialDeviceManager::getSerialMessage( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Read message from Serial with the device index deviceIdx and write the message into
	// an std::vector<unsigned char>
	vector<unsigned char> message;
	vector<unsigned char> tmpMessage;	// needed for keeping up with last valid message
	// Check if the device at the index deviceIdx is available and used
	if ( isSerialDeviceRunning( deviceIdx ) )
	{
		try
		{
			double timestamp = ( (Serial *)m_SerialDevices[deviceIdx].m_Serial )->getMessage( &tmpMessage );
			while( timestamp > 0.0 )
			{
				message = tmpMessage;
				timestamp = ( (Serial *)m_SerialDevices[deviceIdx].m_Serial )->getMessage( &tmpMessage );	// this sets 0 message if there is no message to be found, so we need to keep the last valid
			}
		}
		catch ( RtError& error )
		{
			error.printMessage();
		}
	}
	
	return message;
}

void SerialDeviceManager::sendSerialOutMessage( const unsigned int deviceIdx, std::vector<unsigned char> message )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Check if the device at the index deviceIdx is available and used
	if ( isSerialOutDeviceRunning( deviceIdx ) )
	{
		try
		{
			// Send the message via the SerialOut port defined by deviceIdx
			( (SerialOut *)m_SerialOutDevicesInUse[deviceIdx].m_Serial )->sendMessage( &message );
		}
		catch ( RtError& error )
		{
			error.printMessage();
		}
	}
}

void SerialDeviceManager::initSerialDeviceList()
{
	try
	{
		// Store the number of available Serial ports
		Serial tmpSerial;
		m_iNumSerialDevices = tmpSerial.getPortCount();

		// Clear the old Serial list before initializing the new one
		clearSerialList();

		// Initialize the new Serial list according to the number of available Serial ports
		for ( unsigned int i = 0; i < m_iNumSerialDevices; i++ )
		{
			m_SerialDevices.push_back( DeviceItem( tmpSerial.getPortName( i ), false ) );
			m_SerialDevices[i].m_Serial = new Serial();
		}
	}
	catch ( RtError& error )
	{
		error.printMessage();
	}
}

void SerialDeviceManager::initSerialOutDeviceList()
{
	try
	{
		// Store the number of available SerialOut ports
		SerialOut tmpSerialOut;
		m_iNumSerialOutDevices = tmpSerialOut.getPortCount();

		// Clear the old SerialOut list before initializing the new one
		clearSerialOutList();

		// Initialize the new SerialOut list according to the number of available SerialOut ports
		for ( unsigned int i = 0; i < m_iNumSerialOutDevices; i++ )
		{
			m_SerialOutDevicesInUse.push_back( DeviceItem( tmpSerialOut.getPortName( i ), false ) );
			m_SerialOutDevicesInUse[i].m_Serial = new SerialOut();
		}
	}
	catch ( RtError& error )
	{
		error.printMessage();
	}
}

void SerialDeviceManager::rescanSerialPorts()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Get the number of currently available Serial ports
	Serial tmpSerial;
	unsigned int numDevices = tmpSerial.getPortCount();

	// If the number of available Serial ports differs from the number that was detected in the last call
	// --> close all ports and re-initialize the Serial list
	if ( numDevices != m_iNumSerialDevices )
	{
		for ( unsigned int i = 0; i < m_SerialDevices.size(); i++ )
		{
			if ( m_SerialDevices[i].m_bIsUsed )
				m_SerialDevices[i].m_Serial->closePort();
		}

		initSerialDeviceList();

		// Store the new number of available Serial ports
		m_iNumSerialDevices = numDevices;
	}
}

void SerialDeviceManager::rescanSerialOutPorts()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Get the number of currently available SerialOut ports
	SerialOut tmpSerialOut;
	unsigned int numDevices = tmpSerialOut.getPortCount();

	// If the number of available SerialOut ports differs from the number that was detected in the last call
	// --> close all ports and re-initialize the SerialOut list
	if ( numDevices != m_iNumSerialOutDevices )
	{
		for ( unsigned int i = 0; i < m_SerialOutDevicesInUse.size(); i++ )
		{
			if ( m_SerialOutDevicesInUse[i].m_bIsUsed )
				m_SerialOutDevicesInUse[i].m_Serial->closePort();
		}

		initSerialOutDeviceList();

		// Store the new number of available SerialOut ports
		m_iNumSerialOutDevices = numDevices;
	}
}

bool SerialDeviceManager::isSerialDeviceAvailable( const unsigned int deviceIdx ) const
{
	return ( deviceIdx < m_SerialDevices.size() );
}

bool SerialDeviceManager::isSerialOutDeviceAvailable( const unsigned int deviceIdx ) const
{
	return ( deviceIdx < m_SerialOutDevicesInUse.size() );
}

bool SerialDeviceManager::isSerialDeviceFree( const unsigned int deviceIdx ) const
{
	if ( isSerialDeviceAvailable( deviceIdx ) && !m_SerialDevices[deviceIdx].m_bIsUsed )
		return true;
	return false;
}

bool SerialDeviceManager::isSerialOutDeviceFree( const unsigned int deviceIdx ) const
{
	if ( isSerialOutDeviceAvailable( deviceIdx ) && !m_SerialOutDevicesInUse[deviceIdx].m_bIsUsed )
		return true;
	return false;
}
*/