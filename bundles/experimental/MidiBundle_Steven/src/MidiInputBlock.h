#pragma once

#include "_2realblock.h"
#include "RtMidi.h"

/*
class MidiInputBlock

Defines the MidiInput Block --> Via a Midi Input Port the Midi Messages are sent through three Outlets.
The class automatically checks for available Ports and opens Ports accordingly to availability.
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
	// Gets the new Port index from the Block Inlet and checks, whether the Port has changed since the last call
	// --> If so, the old Port is closed and the new one opened
	void							rescanMidiInPorts();
	// Initializes the RtMidiIn Pointer
	void							initRtMidiIn();

	// The current MidiIn Port index
	int								m_iMidiInCurrentPort;

	// The Block Inlet for the MidiIn Port index
	_2Real::bundle::InletHandle		m_MidiInPortInlet;

	// The three Block Outlets where the messages that are generated from the RtMidiIn Instance are sent
	_2Real::bundle::OutletHandle	m_MidiInMessage0Outlet;
	_2Real::bundle::OutletHandle	m_MidiInMessage1Outlet;
	_2Real::bundle::OutletHandle	m_MidiInMessage2Outlet;

	// The RtMidiIn Instance
	RtMidiIn*						m_MidiIn;
};

