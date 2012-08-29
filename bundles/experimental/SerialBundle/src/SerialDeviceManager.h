#pragma once

#include "_2realblock.h"
#include "Poco/Mutex.h"
#include <hash_map>
#include <string>

class ofSerial;

/*
Class SerialDeviceManager

Manages two lists, one Serial Input and one Serial Output list. The lists are initialized and updated automatically
according to the Blocks that are requesting these lists. Furthermore, the manager offers methods to get and send
Serial messages using a certain port.

TODO right now there is only one mutex that is mutexing the whole class at more or less every operation. this means that this is a very bad thing for concurrent operations accessing the hardware.
Read and write operations should be always possible asynchronously without mutexing one shared context.
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

	// Bind a MidiOut device via an index
	bool							bindSerialDevice( const std::string identifier, unsigned int baudrate );
	// Unbind a MidiIn device via an index
	void							unbindSerialDevice( const std::string identifier );

	// Get the number of available Serial Ports
	unsigned int					getSerialPortCount();

	// Returns whether the Serial device with the index deviceIdx is currently is use or not
	bool							isSerialDeviceRunning( const std::string identifier );
	
	// Reads the Serial message from the Serial device and returns the message as a std::vector
	std::vector<unsigned char>		getSerialMessage( const std::string identifier );

	// Sends the message to a serial device and returns the message as a std::vector
	void							sendSerialMessage( const std::string identifier, std::vector<unsigned char> &message );

	// if data is read to be read it returns the number of bytes pending
	int							hasDataPending( const std::string identifier );


private:
	// A struct to define a simple DeviceItem, which is stored in lists
	// Each DeviceItem is defined by description string, a flag to determine whether it is currently being used
	// or not and a pointer to an Serial instance
	class DeviceItem
	{
		friend class SerialDeviceManager;

		DeviceItem( std::string strDescription, bool bIsUsed = false);
		~DeviceItem();
		ofSerial						*m_pSerial;
		std::string						m_strDescription;
		bool							m_bIsUsed;
	};

	// Initialize the Serial List according to how many Serial ports are available
	void							initSerialDeviceList();
	// Checks whether the Serial device at the index deviceIdx is available or not
	bool							isSerialDeviceAvailable(std::string identifier);
	// Checks whether the Serial device at the index deviceIdx is available and not used yet
	bool							isSerialDeviceFree(std::string identifier);
	// Clears the Serial list and deletes allocated memory
	void							clearSerialList();
	// returns a device item
	DeviceItem						*getDeviceItem(const std::string identifier);

	// A reference counter that describes how many Serial Blocks hold a reference to this Device manager
	// --> needed to both initialize and clear the Serial list
	int								m_iSerialReferenceCount;

	// The list of available Serial Devices
	stdext::hash_map< std::string, DeviceItem* > m_SerialDevices;
	
	// Mutex needed for several locks in the Device manager's code
	Poco::Mutex						m_Mutex;
};
