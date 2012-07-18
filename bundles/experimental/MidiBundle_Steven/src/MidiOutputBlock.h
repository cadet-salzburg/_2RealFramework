#pragma once

#include "_2realblock.h"
#include "RtMidi.h"

class MidiOutputBlock : public _2Real::bundle::Block
{
public:
	MidiOutputBlock( _2Real::bundle::ContextBlock & context );
	~MidiOutputBlock(void);

	virtual void update();
	virtual void setup( _2Real::bundle::BlockHandle &context );
	virtual void shutdown();

private:
	void							rescanMidiOutPorts();
	void							initRtMidiOut();

	int								m_iMidiOutLastPort;
	_2Real::bundle::InletHandle		m_MidiOutPortInlet;
	_2Real::bundle::InletHandle		m_MidiOutMessageInlet;

	// New solution
	_2Real::bundle::InletHandle		m_MidiOutMessage0Inlet;
	_2Real::bundle::InletHandle		m_MidiOutMessage1Inlet;
	_2Real::bundle::InletHandle		m_MidiOutMessage2Inlet;

	RtMidiOut*						m_MidiOut;
};

