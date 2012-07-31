#pragma once

#include "_2realblock.h"

#include "SerialDeviceManager.h"

/*
class SerialBlock

Defines the Serialput Block --> Via a Serial Input Port the Serial Messages are sent through three Outlets.
*/
class SerialBlock : public _2Real::bundle::Block
{
public:
	SerialBlock( _2Real::bundle::ContextBlock & context );
	~SerialBlock(void);

	// Virtual Methods
	virtual void					update();
	virtual void					setup( _2Real::bundle::BlockHandle &context );
	virtual void					shutdown();

private:
	void							discardAllOutlets();

	// The Block Inlet for the Serial Port index
	_2Real::bundle::InletHandle		m_SerialPortInlet;

	// The Block Inlets Outlets
	_2Real::bundle::InletHandle m_iPortInlet;
	_2Real::bundle::InletHandle m_iBaudRateInlet;
	_2Real::bundle::InletHandle m_ReadIntel;
	_2Real::bundle::OutletHandle m_WriteOutlet;
	
	// The SerialDevice Manager
	SerialDeviceManager*			m_SerialDeviceManager;
};
