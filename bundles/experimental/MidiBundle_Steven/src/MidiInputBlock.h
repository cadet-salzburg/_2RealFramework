#pragma once

#include "_2realblock.h"
#include "RtMidi.h"


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
	void							rescanMidiInPorts();
	void							initRtMidiIn();

	int								m_iMidiInLastPort;
	_2Real::bundle::InletHandle		m_MidiInPortInlet;
	_2Real::bundle::OutletHandle	m_MidiMessageOutlet;

	// new solution
	_2Real::bundle::OutletHandle	m_MidiInMessage0Outlet;
	_2Real::bundle::OutletHandle	m_MidiInMessage1Outlet;
	_2Real::bundle::OutletHandle	m_MidiInMessage2Outlet;

	RtMidiIn*						m_MidiIn;
};

