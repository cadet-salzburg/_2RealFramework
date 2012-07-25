#pragma once

#include "_2realblock.h"
#include "RtMidi.h"

#include "MidiInOutDeviceManager.h"

/*
class MidiInputBlock

Defines the MidiInput Block --> Via a Midi Input Port the Midi Messages are sent through three Outlets.
*/
class MidiInputBlock : public _2Real::bundle::Block
{
public:
	MidiInputBlock( _2Real::bundle::ContextBlock & context );
	~MidiInputBlock(void);

	// Virtual Methods
	virtual void					update();
	virtual void					setup( _2Real::bundle::BlockHandle &context );
	virtual void					shutdown();

private:
	void							discardAllOutlets();

	// The current MidiIn Port index
	int								m_iMidiInCurrentPort;

	// The Block Inlet for the MidiIn Port index
	_2Real::bundle::InletHandle		m_MidiInPortInlet;

	// The three Block Outlets where the messages that are generated from the RtMidiIn Instance are sent
	_2Real::bundle::OutletHandle	m_MidiInMessage0Outlet;
	_2Real::bundle::OutletHandle	m_MidiInMessage1Outlet;
	_2Real::bundle::OutletHandle	m_MidiInMessage2Outlet;

	// The MidiInOutDevice Manager
	MidiInOutDeviceManager*			m_MidiInOutDeviceManager;
};

