#pragma once

#include "_2realblock.h"
#include "RtMidi.h"

#include "MidiInOutDeviceManager.h"

/*
class MidiOutputBlock

Defines the MidiOutBlock --> A Midi Output Port is opened and receives messages via three Inlets.
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
	// The current MidiOut Port index
	int								m_iMidiOutCurrentPort;

	// The Block Outlet for the MidiOut Port index
	_2Real::bundle::InletHandle		m_MidiOutPortInlet;

	// The three Block Inlets that describe the incoming Midi messages
	_2Real::bundle::InletHandle		m_MidiOutMessage0Inlet;
	_2Real::bundle::InletHandle		m_MidiOutMessage1Inlet;
	_2Real::bundle::InletHandle		m_MidiOutMessage2Inlet;

	// The MidiInOutDeviceManager
	MidiInOutDeviceManager*			m_MidiInOutDeviceManager;
};

