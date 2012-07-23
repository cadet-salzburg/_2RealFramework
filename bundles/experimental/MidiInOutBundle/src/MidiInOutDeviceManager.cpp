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

	if ( m_iMidiInReferenceCount == 0 )
		m_bMidiInListIsUsed = true;
	m_iMidiInReferenceCount++;
}

void MidiInOutDeviceManager::unregisterMidiInBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	m_iMidiInReferenceCount--;
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

	if ( m_iMidiOutReferenceCount == 0 )
		m_bMidiOutListIsUsed = true;
	m_iMidiOutReferenceCount++;
}

void MidiInOutDeviceManager::unregisterMidiOutBlock()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	m_iMidiOutReferenceCount--;
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

	if ( isMidiInDeviceFree( deviceIdx ) )
	{
		try
		{
			m_MidiInDevicesInUse[deviceIdx].m_Midi->openPort( deviceIdx );
			m_MidiInDevicesInUse[deviceIdx].m_bIsUsed = true;
			return true;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed = false;
			return false;
		}
	}
}

bool MidiInOutDeviceManager::bindMidiOutDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	if ( isMidiOutDeviceFree( deviceIdx ) )
	{
		try
		{
			m_MidiOutDevicesInUse[deviceIdx].m_Midi->openPort( deviceIdx );
			m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed = true;
			return true;
		}
		catch ( RtError& error )
		{
			error.printMessage();
			m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed = false;
			return false;
		}
	}
}

void MidiInOutDeviceManager::unsbindMidiInDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	try
	{
		if ( isMidiInDeviceRunning( deviceIdx ) )
		{
			m_MidiInDevicesInUse[deviceIdx].m_Midi->closePort();
			m_MidiInDevicesInUse[deviceIdx].m_bIsUsed = false;
		}
	}
	catch ( RtError& error )
	{
		error.printMessage();
	}
}

void MidiInOutDeviceManager::unsbindMidiOutDevice( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	try
	{
		if ( isMidiOutDeviceRunning( deviceIdx ) )
		{
			m_MidiOutDevicesInUse[deviceIdx].m_Midi->closePort();
			m_MidiOutDevicesInUse[deviceIdx].m_bIsUsed = false;
		}
	}
	catch ( RtError& error )
	{
		error.printMessage();
	}
}

int MidiInOutDeviceManager::getFirstFreeMidiInDevice()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	for ( unsigned int i = 0; i < m_MidiInDevicesInUse.size(); i++ )
	{
		if ( !m_MidiInDevicesInUse[i].m_bIsUsed )
			return i;
	}
	return -1;
}

int MidiInOutDeviceManager::getFirstFreeMidiOutDevice()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	for ( unsigned int i = 0; i < m_MidiOutDevicesInUse.size(); i++ )
	{
		if ( !m_MidiOutDevicesInUse[i].m_bIsUsed )
			return i;
	}
	return -1;
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

	vector<unsigned char> message;
	if ( isMidiInDeviceAvailable( deviceIdx ) && isMidiInDeviceRunning( deviceIdx ) )
		( (RtMidiIn *)m_MidiInDevicesInUse[deviceIdx].m_Midi )->getMessage( &message );
	
	return message;
}

void MidiInOutDeviceManager::sendMidiOutMessage( const unsigned int deviceIdx, std::vector<unsigned char> message )
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	if ( isMidiOutDeviceRunning( deviceIdx ) )
	{
		try
		{
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
		RtMidiIn tmpMidiIn;
		m_iNumMidiInDevices = tmpMidiIn.getPortCount();

		clearMidiInList();
		for ( unsigned int i = 0; i < m_iNumMidiInDevices; i++ )
		{
			m_MidiInDevicesInUse.push_back( DeviceItem( tmpMidiIn.getPortName( i ), false ) );
			m_MidiInDevicesInUse[i].m_Midi = new RtMidiIn();
		}

		m_bMidiInListIsUsed = true;
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
		RtMidiOut tmpMidiOut;
		m_iNumMidiOutDevices = tmpMidiOut.getPortCount();

		clearMidiOutList();
		for ( unsigned int i = 0; i < m_iNumMidiOutDevices; i++ )
		{
			m_MidiOutDevicesInUse.push_back( DeviceItem( tmpMidiOut.getPortName( i ), false ) );
			m_MidiOutDevicesInUse[i].m_Midi = new RtMidiOut();
		}

		m_bMidiOutListIsUsed = true;
	}
	catch ( RtError& error )
	{
		error.printMessage();
	}
}

void MidiInOutDeviceManager::rescanMidiInPorts()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	RtMidiIn tmpMidiIn;
	unsigned int numDevices = tmpMidiIn.getPortCount();
	if ( numDevices != m_iNumMidiInDevices )
	{
		for ( unsigned int i = 0; i < m_MidiInDevicesInUse.size(); i++ )
		{
			if ( m_MidiInDevicesInUse[i].m_bIsUsed )
				m_MidiInDevicesInUse[i].m_Midi->closePort();
		}

		initMidiInDeviceList();
		m_iNumMidiInDevices = numDevices;
	}
}

void MidiInOutDeviceManager::rescanMidiOutPorts()
{
	Poco::Mutex::ScopedLock lock( m_Mutex );

	RtMidiOut tmpMidiOut;
	unsigned int numDevices = tmpMidiOut.getPortCount();
	if ( numDevices != m_iNumMidiOutDevices )
	{
		for ( unsigned int i = 0; i < m_MidiOutDevicesInUse.size(); i++ )
		{
			if ( m_MidiOutDevicesInUse[i].m_bIsUsed )
				m_MidiOutDevicesInUse[i].m_Midi->closePort();
		}

		initMidiOutDeviceList();
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
