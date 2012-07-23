#pragma once

#include "_2realblock.h"

#include "Poco/Mutex.h"
#include "RtMidi.h"

#include <vector>
#include <string>

/*
Class MidiInOutDeviceManager

Manages two lists, one Midi Input and one Midi Output list. The lists are initialized and updated automatically
according to the Blocks that are requesting these lists. Furthermore, the manager offers methods to get and send
Midi messages using a certain port.
*/
class MidiInOutDeviceManager : public _2Real::bundle::ContextBlock
{
public:
	MidiInOutDeviceManager();
	~MidiInOutDeviceManager();

	// Methods that are called via the _2Real framework
	void							setup( _2Real::bundle::BlockHandle &context );
	void							update();
	void							shutdown();

	// Registers a MidiIn Block, increases the reference counter and activates the scanning of the MidiIn List
	// if called for the first time
	void							registerMidiInBlock();
	// Unregisters a MidiIn Block, decreases the reference counter and also deactivates the scanning of the
	// MidiIn List if the reference counter reaches zero
	void							unregisterMidiInBlock();

	// Registers a MidiOut Block, increases the reference counter and activates the scanning of the MidiOut List
	// if called for the first time
	void							registerMidiOutBlock();
	// Unregisters a MidiOut Block, decreases the reference counter and also deactivates the scanning of the
	// MidiOut List if the reference counter reaches zero
	void							unregisterMidiOutBlock();

	// Bind a MidiIn device via an index
	bool							bindMidiInDevice( const unsigned int deviceIdx );
	// Bind a MidiOut device via an index
	bool							bindMidiOutDevice( const unsigned int deviceIdx );
	// Unbind a MidiIn device via an index
	void							unsbindMidiInDevice( const unsigned int deviceIdx );
	// Unbind a MidiOut device via an index
	void							unsbindMidiOutDevice( const unsigned int deviceIdx );

	// Get the number of available MidiIn Ports
	unsigned int					getMidiInPortCount();
	// Get the number of available MidiOut Ports
	unsigned int					getMidiOutPortCount();
	// Returns whether the MidiIn device with the index deviceIdx is currently is use or not
	bool							isMidiInDeviceRunning( const unsigned int deviceIdx );
	// Returns whether the MidiOut device with the index deviceIdx is currently is use or not
	bool							isMidiOutDeviceRunning( const unsigned int deviceIdx );
	
	// Reads the MidiIn message from the MidiIn device at the index deviceIdx and returns the message as a std::vector
	std::vector<unsigned char>		getMidiInMessage( const unsigned int deviceIdx );
	// Sends a message via the MidiOut device at the index deviceIdx
	void							sendMidiOutMessage( const unsigned int deviceIdx, std::vector<unsigned char> message );

private:
	// Initialize the MidiIn List according to how many MidiIn ports are available
	void							initMidiInDeviceList();
	// Initialize the MidiOut List according to how many MidiOut ports are available
	void							initMidiOutDeviceList();
	// Rescan the MidiIn ports and check, whether the available number of MidiIn ports has changed during runtime
	// --> If so, re-initialize the MidiIn list
	void							rescanMidiInPorts();
	// Rescan the MidiOut ports and check, whether the available number of MidiOut ports has changed during runtime
	// --> If so, re-initialize the MidiOut list
	void							rescanMidiOutPorts();
	// Checks whether the MidiIn device at the index deviceIdx is available or not
	bool							isMidiInDeviceAvailable( const unsigned int deviceIdx ) const;
	// Checks whether the MidiOut device at the index deviceIdx is available or not
	bool							isMidiOutDeviceAvailable( const unsigned int deviceIdx ) const;
	// Checks whether the MidiIn device at the index deviceIdx is available and not used yet
	bool							isMidiInDeviceFree( const unsigned int deviceIdx ) const;
	// Checks whether the MidiOut device at the index deviceIdx is available and not used yet
	bool							isMidiOutDeviceFree( const unsigned int deviceIdx ) const;
	// Clears the MidiIn list and deletes allocated memory
	void							clearMidiInList();
	// Clears the MidiOut list and deletes allocated memory
	void							clearMidiOutList();

	// A struct to define a simple DeviceItem, which is stored in lists
	// Each DeviceItem is defined by description string, a flag to determine whether it is currently being used
	// or not and a pointer to an RtMidi instance, which will be casted to either a RtMidiIn or RtMidiOut
	// instance when needed
	struct DeviceItem
	{
		DeviceItem::DeviceItem( std::string strDescription, bool bIsUsed ) :
			m_Midi ( 0 ),
			m_strDescription ( strDescription ),
			m_bIsUsed ( bIsUsed )
		{
			
		}
		
		RtMidi*							m_Midi;
		std::string						m_strDescription;
		bool							m_bIsUsed;
	};

	// Describes whether the MidiIn list should be used
	bool								m_bMidiInListIsUsed;
	// Describes whether the MidiOut list should be used
	bool								m_bMidiOutListIsUsed;

	// A reference counter that describes how many MidiIn Blocks hold a reference to this Device manager
	// --> needed to both initialize and clear the MidiIn list
	int									m_iMidiInReferenceCount;
	// A reference counter that describes how many MidiOut Blocks hold a reference to this Device manager
	// --> needed to both initialize and clear the MidiOut list
	int									m_iMidiOutReferenceCount;

	// Describes the number of available MidiIn Ports
	unsigned int						m_iNumMidiInDevices;
	// Describes the number of available MidiOut Ports
	unsigned int						m_iNumMidiOutDevices;

	// The list of available MidiIn Devices
	std::vector< DeviceItem >			m_MidiInDevicesInUse;
	// The list of available MidiOut Devices
	std::vector< DeviceItem >			m_MidiOutDevicesInUse;
	
	RtMidiIn*							m_MidiIn;
	RtMidiOut*							m_MidiOut;

	// Mutex needed for several locks in the Device manager's code
	Poco::Mutex							m_Mutex;
};

