#pragma once

#include "_2realblock.h"

#include "Poco/Mutex.h"

#include <vector>
#include <string>
#include "ofSerial.h"

/*
Class SerialDeviceManager

Manages two lists, one Serial Input and one Serial Output list. The lists are initialized and updated automatically
according to the Blocks that are requesting these lists. Furthermore, the manager offers methods to get and send
Serial messages using a certain port.
*/
class SerialDeviceManager : public _2Real::bundle::ContextBlock
{
public:
	SerialDeviceManager();
	~SerialDeviceManager();

	// Methods that are called via the _2Real framework
	void							setup( _2Real::bundle::BlockHandle &context );
	void							update();
	void							shutdown();

	// Registers a Serial Block, increases the reference counter and activates the scanning of the Serial List
	// if called for the first time
	void							registerSerialBlock();
	// Unregisters a Serial Block, decreases the reference counter and also deactivates the scanning of the
	// Serial List if the reference counter reaches zero
	void							unregisterSerialBlock();

	// Get the number of available Serial Ports
	unsigned int					getSerialPortCount();

	// Returns whether the Serial device with the index deviceIdx is currently is use or not
	bool							isSerialDeviceRunning( const unsigned int deviceIdx );
	
	// Reads the Serial message from the Serial device at the index deviceIdx and returns the message as a std::vector
	std::vector<unsigned char>		getSerialMessage( const unsigned int deviceIdx );

private:
	// Initialize the Serial List according to how many Serial ports are available
	void							initSerialDeviceList();
	// Rescan the Serial ports and check, whether the available number of Serial ports has changed during runtime
	// --> If so, re-initialize the Serial list
	void							rescanSerialPorts();
	// Checks whether the Serial device at the index deviceIdx is available or not
	bool							isSerialDeviceAvailable( const unsigned int deviceIdx ) const;
	// Checks whether the Serial device at the index deviceIdx is available and not used yet
	bool							isSerialDeviceFree( const unsigned int deviceIdx ) const;
	// Clears the Serial list and deletes allocated memory
	void							clearSerialList();

	// A struct to define a simple DeviceItem, which is stored in lists
	// Each DeviceItem is defined by description string, a flag to determine whether it is currently being used
	// or not and a pointer to an Serial instance
	struct DeviceItem
	{
		DeviceItem::DeviceItem( std::string strDescription, bool bIsUsed ) :
			m_strDescription ( strDescription ),
			m_bIsUsed ( bIsUsed )
		{
		}
		ofSerial						m_Serial;
		std::string						m_strDescription;
		bool							m_bIsUsed;
	};

	// A reference counter that describes how many Serial Blocks hold a reference to this Device manager
	// --> needed to both initialize and clear the Serial list
	int									m_iSerialReferenceCount;

	// Describes the number of available Serial Ports
	unsigned int						m_iNumSerialDevices;

	// The list of available Serial Devices
	std::vector< DeviceItem >			m_SerialDevices;
	
	// Mutex needed for several locks in the Device manager's code
	Poco::Mutex							m_Mutex;
};

