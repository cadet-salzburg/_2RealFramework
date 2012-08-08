#pragma once

#include "_2realblock.h"
#include "Poco/Mutex.h"
#include <hash_map>
#include <string>

class ofShapeWrap;

/*
Class ShapeWrapDeviceManager

Manages two lists, one ShapeWrap Input and one ShapeWrap Output list. The lists are initialized and updated automatically
according to the Blocks that are requesting these lists. Furthermore, the manager offers methods to get and send
ShapeWrap messages using a certain port.
*/
class ShapeWrapDeviceManager : public _2Real::bundle::ContextBlock
{
public:
	ShapeWrapDeviceManager();
	~ShapeWrapDeviceManager();

	// Methods that are called via the _2Real framework
	void							setup( _2Real::bundle::BlockHandle &context );
	void							update();
	void							shutdown();

	// Registers a ShapeWrap Block, increases the reference counter and activates the scanning of the ShapeWrap List
	// if called for the first time
	void							registerShapeWrapBlock();
	// Unregisters a ShapeWrap Block, decreases the reference counter and also deactivates the scanning of the
	// ShapeWrap List if the reference counter reaches zero
	void							unregisterShapeWrapBlock();

	// Bind a MidiOut device via an index
	bool							bindShapeWrapDevice( const std::string identifier, unsigned int baudrate );
	// Unbind a MidiIn device via an index
	void							unbindShapeWrapDevice( const std::string identifier );

	// Get the number of available ShapeWrap Ports
	unsigned int					getShapeWrapPortCount();

	// Returns whether the ShapeWrap device with the index deviceIdx is currently is use or not
	bool							isShapeWrapDeviceRunning( const std::string identifier );
	
	// Reads the ShapeWrap message from the ShapeWrap device and returns the message as a std::vector
	std::vector<unsigned char>		getShapeWrapMessage( const std::string identifier );

	// Sends the message to a ShapeWrap device and returns the message as a std::vector
	void							sendShapeWrapMessage( const std::string identifier, std::vector<unsigned char> &message );

private:
	// A struct to define a simple DeviceItem, which is stored in lists
	// Each DeviceItem is defined by description string, a flag to determine whether it is currently being used
	// or not and a pointer to an ShapeWrap instance
	class DeviceItem
	{
		friend class ShapeWrapDeviceManager;

		DeviceItem( std::string strDescription, bool bIsUsed = false);
		~DeviceItem();
		//ShapeWrap						*m_pShapeWrap;
		std::string						m_strDescription;
		bool							m_bIsUsed;
	};

	// Initialize the ShapeWrap List according to how many ShapeWrap ports are available
	void							initShapeWrapDeviceList();
	// Checks whether the ShapeWrap device at the index deviceIdx is available or not
	bool							isShapeWrapDeviceAvailable(std::string identifier);
	// Checks whether the ShapeWrap device at the index deviceIdx is available and not used yet
	bool							isShapeWrapDeviceFree(std::string identifier);
	// Clears the ShapeWrap list and deletes allocated memory
	void							clearShapeWrapList();
	// returns a device item
	DeviceItem						*getDeviceItem(const std::string identifier);

	// A reference counter that describes how many ShapeWrap Blocks hold a reference to this Device manager
	// --> needed to both initialize and clear the ShapeWrap list
	int								m_iShapeWrapReferenceCount;

	// The list of available ShapeWrap Devices
	stdext::hash_map< std::string, DeviceItem* > m_ShapeWrapDevices;
	
	// Mutex needed for several locks in the Device manager's code
	Poco::Mutex						m_Mutex;
};
