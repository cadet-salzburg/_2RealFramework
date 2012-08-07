#include "MidiInOutDeviceManager.h"

#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;


MidiInOutDeviceManager::MidiInOutDeviceManager(void) : 
	ContextBlock(),
	m_bMidiInListIsUsed( false ),
	m_bMidiOutListIsUsed( false ),
	m_iMidiInReferenceCount( 0 ),
	m_iMidiOutReferenceCount( 0 ),
	m_iNumMidiInDevices( 0 ),
	m_iNumMidiOutDevices( 0 ),
	m_MidiIn( 0 ),
	m_MidiOut( 0 )
{
	
}


MidiInOutDeviceManager::~MidiInOutDeviceManager(void)
{
	clearMidiInList();
	clearMidiOutList();
}

void MidiInOutDeviceManager::setup( _2Real::bundle::BlockHandle &context )
{
	
}

void MidiInOutDeviceManager::update()
{
	try
	{
		// Only initialize and update the respective Midi lists if they are actually requested by
		// either MidiIn or Out Blocks
		if ( m_bMidiInListIsUsed )
			rescanMidiInPorts();
		if ( m_bMidiOutListIsUsed )
			rescanMidiOutPorts();
	}
	catch ( RtError& error )
	{
		error.printMessage();
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void MidiInOutDeviceManager::shutdown()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	try
	{
		for ( unsigned int i = 0; i < m_MidiInDevicesInUse.size(); i++ )
		{
			if ( m_MidiInDevicesInUse[i].m_bIsUsed )
				m_MidiInDevicesInUse[i].m_Midi->closePort();
		}

		for ( unsigned int i = 0; i < m_MidiOutDevicesInUse.size(); i++ )
		{
			if ( m_MidiOutDevicesInUse[i].m_bIsUsed )
				m_MidiOutDevicesInUse[i].m_Midi->closePort();
		}
	}
	catch ( RtError& error )
	{
		error.printMessage();
	}
}

void MidiInOutDeviceManager::registerMidiInBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// If called for the first time, set the MidiInIsUsed flag to true to make sure that the MidiIn list is
	// initialized and updated from now on
	if ( m_iMidiInReferenceCount == 0 )
		m_bMidiInListIsUsed = true;
	// Increase reference counter
	m_iMidiInReferenceCount++;
}

void MidiInOutDeviceManager::unregisterMidiInBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Decrease reference counter
	m_iMidiInReferenceCount--;

	// If the reference counter reaches zero clear the MidiIn list and reset all important values
	if ( m_iMidiInReferenceCount <= 0 )
	{
		m_iMidiInReferenceCount = 0;
		m_iNumMidiInDevices = 0;
		m_bMidiInListIsUsed = false;
		clearMidiInList();
	}
}

void MidiInOutDeviceManager::registerMidiOutBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// If called for the first time, set the MidiOutIsUsed flag to true to make sure that the MidiOut list is
	// initialized and updated from now on
	if ( m_iMidiOutReferenceCount == 0 )
		m_bMidiOutListIsUsed = true;
	// Increase reference counter
	m_iMidiOutReferenceCount++;
}

void MidiInOutDeviceManager::unregisterMidiOutBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Decrease reference counter
	m_iMidiOutReferenceCount--;

	// If the reference counter reaches zero clear the MidiOut list and reset all important values
	if ( m_iMidiOutReferenceCount <= 0 )
	{
		m_iMidiOutReferenceCount = 0;
		m_iNumMidiOutDevices = 0;
		m_bMidiOutListIsUsed = false;
		clearMidiOutList();
	}
}

bool MidiInOutDeviceManager::bindMidiInDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Only bind the device if the device is actually available and not used yet
	if ( isMidiInDeviceFree( deviceIdx ) )
	{
		try
		{
			// Open the MidiIn port
			m_MidiInDevicesInUse[deviceIdx].m_Midi->openPort( deviceIdx );
			// Set the isUsed flag of this device to true
			m_MidiInDevicesInUse[deviceIdx].m_bIsUsed = true;
			return true;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			// Set the isUsed flag of that device back to false if something went wrong while opening the port
			m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed = false;
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool MidiInOutDeviceManager::bindMidiOutDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Only bind the device if the device is actually available and not used yet
	if ( isMidiOutDeviceFree( deviceIdx ) )
	{
		try
		{
			// Open the MidiOut port
			m_MidiOutDevicesInUse[deviceIdx].m_Midi->openPort( deviceIdx );
			// Set the isUsed flag of this device to true
			m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed = true;
			return true;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			// Set the isUsed flag of that device back to false if something went wrong while opening the port
			m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed = false;
			return false;
		}
	}
	else
	{
		return false;
	}
}

void MidiInOutDeviceManager::unbindMidiInDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Unbind the device if it is available and running
	if ( isMidiInDeviceRunning( deviceIdx ) )
	{
		try
		{
			// Close the MidiIn port
			m_MidiInDevicesInUse[deviceIdx].m_Midi->closePort();
			// Set the isUsed flag to false
			m_MidiInDevicesInUse[deviceIdx].m_bIsUsed = false;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			// Set the isUsed flag to false if something went wrong while closing the port
			m_MidiInDevicesInUse[deviceIdx].m_bIsUsed = false;
		}
	}
}

void MidiInOutDeviceManager::unbindMidiOutDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Unbind the device if it is available and running
	if ( isMidiOutDeviceRunning( deviceIdx ) )
	{
		try
		{
			// Close the MidiOut port
			m_MidiOutDevicesInUse[deviceIdx].m_Midi->closePort();
			// Set the isUsed flag to false
			m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed = false;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			// Set the isUsed flag to false if something went wrong while closing the port
			m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed = false;
		}
	}
}

unsigned int MidiInOutDeviceManager::getMidiInPortCount()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	return m_iNumMidiInDevices;
}

unsigned int MidiInOutDeviceManager::getMidiOutPortCount()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );
	return m_iNumMidiOutDevices;
}

bool MidiInOutDeviceManager::isMidiInDeviceRunning( const unsigned int deviceIdx )
{
	return isMidiInDeviceAvailable( deviceIdx ) && m_MidiInDevicesInUse[deviceIdx].m_bIsUsed;
}

bool MidiInOutDeviceManager::isMidiOutDeviceRunning( const unsigned int deviceIdx )
{
	return isMidiOutDeviceAvailable( deviceIdx ) && m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed;
}

vector<unsigned char> MidiInOutDeviceManager::getMidiInMessage( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Read message from MidiIn with the device index deviceIdx and write the message into
	// an std::vector<unsigned char>
	vector<unsigned char> message;
	vector<unsigned char> tmpMessage;	// needed for keeping up with last valid message
	// Check if the device at the index deviceIdx is available and used
	if ( isMidiInDeviceRunning( deviceIdx ) )
	{
		try
		{
			double timestamp = ( (RtMidiIn *)m_MidiInDevicesInUse[deviceIdx].m_Midi )->getMessage( &tmpMessage );
			while( timestamp > 0.0 )
			{
				message = tmpMessage;
				timestamp = ( (RtMidiIn *)m_MidiInDevicesInUse[deviceIdx].m_Midi )->getMessage( &tmpMessage );	// this sets 0 message if there is no message to be found, so we need to keep the last valid
			}
		}
		catch ( RtError& error )
		{
			error.printMessage();
		}
	}
	
	return message;
}

void MidiInOutDeviceManager::sendMidiOutMessage( const unsigned int deviceIdx, std::vector<unsigned char> message )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Check if the device at the index deviceIdx is available and used
	if ( isMidiOutDeviceRunning( deviceIdx ) )
	{
		try
		{
			// Send the message via the MidiOut port defined by deviceIdx
			( (RtMidiOut *)m_MidiOutDevicesInUse[deviceIdx].m_Midi )->sendMessage( &message );
		}
		catch ( RtError& error )
		{
			error.printMessage();
		}
	}
}

void MidiInOutDeviceManager::initMidiInDeviceList()
{
	try
	{
		// Store the number of available MidiIn ports
		RtMidiIn tmpMidiIn;
		m_iNumMidiInDevices = tmpMidiIn.getPortCount();

		// Clear the old MidiIn list before initializing the new one
		clearMidiInList();

		// Initialize the new MidiIn list according to the number of available MidiIn ports
		for ( unsigned int i = 0; i < m_iNumMidiInDevices; i++ )
		{
			m_MidiInDevicesInUse.push_back( DeviceItem( tmpMidiIn.getPortName( i ), false ) );
			m_MidiInDevicesInUse[i].m_Midi = new RtMidiIn();
		}
	}
	catch ( RtError& error )
	{
		error.printMessage();
	}
}

void MidiInOutDeviceManager::initMidiOutDeviceList()
{
	try
	{
		// Store the number of available MidiOut ports
		RtMidiOut tmpMidiOut;
		m_iNumMidiOutDevices = tmpMidiOut.getPortCount();

		// Clear the old MidiOut list before initializing the new one
		clearMidiOutList();

		// Initialize the new MidiOut list according to the number of available MidiOut ports
		for ( unsigned int i = 0; i < m_iNumMidiOutDevices; i++ )
		{
			m_MidiOutDevicesInUse.push_back( DeviceItem( tmpMidiOut.getPortName( i ), false ) );
			m_MidiOutDevicesInUse[i].m_Midi = new RtMidiOut();
		}
	}
	catch ( RtError& error )
	{
		error.printMessage();
	}
}

void MidiInOutDeviceManager::rescanMidiInPorts()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Get the number of currently available MidiIn ports
	RtMidiIn tmpMidiIn;
	unsigned int numDevices = tmpMidiIn.getPortCount();

	// If the number of available MidiIn ports differs from the number that was detected in the last call
	// --> close all ports and re-initialize the MidiIn list
	if ( numDevices != m_iNumMidiInDevices )
	{
		for ( unsigned int i = 0; i < m_MidiInDevicesInUse.size(); i++ )
		{
			if ( m_MidiInDevicesInUse[i].m_bIsUsed )
				m_MidiInDevicesInUse[i].m_Midi->closePort();
		}

		initMidiInDeviceList();

		// Store the new number of available MidiIn ports
		m_iNumMidiInDevices = numDevices;
	}
}

void MidiInOutDeviceManager::rescanMidiOutPorts()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	// Get the number of currently available MidiOut ports
	RtMidiOut tmpMidiOut;
	unsigned int numDevices = tmpMidiOut.getPortCount();

	// If the number of available MidiOut ports differs from the number that was detected in the last call
	// --> close all ports and re-initialize the MidiOut list
	if ( numDevices != m_iNumMidiOutDevices )
	{
		for ( unsigned int i = 0; i < m_MidiOutDevicesInUse.size(); i++ )
		{
			if ( m_MidiOutDevicesInUse[i].m_bIsUsed )
				m_MidiOutDevicesInUse[i].m_Midi->closePort();
		}

		initMidiOutDeviceList();

		// Store the new number of available MidiOut ports
		m_iNumMidiOutDevices = numDevices;
	}
}

bool MidiInOutDeviceManager::isMidiInDeviceAvailable( const unsigned int deviceIdx ) const
{
	return ( deviceIdx < m_MidiInDevicesInUse.size() );
}

bool MidiInOutDeviceManager::isMidiOutDeviceAvailable( const unsigned int deviceIdx ) const
{
	return ( deviceIdx < m_MidiOutDevicesInUse.size() );
}

bool MidiInOutDeviceManager::isMidiInDeviceFree( const unsigned int deviceIdx ) const
{
	if ( isMidiInDeviceAvailable( deviceIdx ) && !m_MidiInDevicesInUse[deviceIdx].m_bIsUsed )
		return true;
	return false;
}

bool MidiInOutDeviceManager::isMidiOutDeviceFree( const unsigned int deviceIdx ) const
{
	if ( isMidiOutDeviceAvailable( deviceIdx ) && !m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed )
		return true;
	return false;
}

void MidiInOutDeviceManager::clearMidiInList()
{
	for ( unsigned int i = 0; i < m_MidiInDevicesInUse.size(); i++ )
	{
		delete (RtMidiIn *)m_MidiInDevicesInUse[i].m_Midi;
	}
	m_MidiInDevicesInUse.clear();
}

void MidiInOutDeviceManager::clearMidiOutList()
{
	for ( unsigned int i = 0; i < m_MidiOutDevicesInUse.size(); i++ )
	{
		delete (RtMidiOut *)m_MidiOutDevicesInUse[i].m_Midi;
	}
	m_MidiOutDevicesInUse.clear();
}
