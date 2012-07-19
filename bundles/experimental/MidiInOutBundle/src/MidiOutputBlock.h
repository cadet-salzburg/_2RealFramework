#pragma once

#include "_2realblock.h"
#include "RtMidi.h"

/*
class MidiOutputBlock

Defines the MidiOutBlock --> A Midi Output Port is opened and receives messages via three Inlets.
The class automatically checks for available Ports and opens Ports accordingly to availability.
*/

class MidiOutputBlock : public _2Real::bundle::Block
{
public:
	MidiOutputBlock( _2Real::bundle::ContextBlock & context );
	~MidiOutputBlock(void);

	// Virtual methods
	virtual void update();
	virtual void setup( _2Real::bundle::BlockHandle &context );
	virtual void shutdown();

private:
	// Gets the new Port index from the Block Inlet and checks, whether the Port has changed since the last call
	// --> If so, the old Port is closed and the new one opened
	void							rescanMidiOutPorts();
	// Initializes the RtMidiOut Pointer
	void							initRtMidiOut();

	// Te current MidiOut Port index
	int								m_iMidiOutCurrentPort;

	// The Block Outle for the MidiOut Port index
	_2Real::bundle::InletHandle		m_MidiOutPortInlet;

	// The three Block Inlets that describe the incoming Midi messages
	_2Real::bundle::InletHandle		m_MidiOutMessage0Inlet;
	_2Real::bundle::InletHandle		m_MidiOutMessage1Inlet;
	_2Real::bundle::InletHandle		m_MidiOutMessage2Inlet;

	// The RtMidiOut instance
	RtMidiOut*						m_MidiOut;
};

