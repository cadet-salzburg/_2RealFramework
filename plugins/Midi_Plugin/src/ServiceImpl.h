#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"


#include <iostream>
#include "RtMidi.h"

class MidiInputService : public _2Real::Block
{

public:

	MidiInputService() {}
	void shutdown();
	void update();
	void setup(_2Real::FrameworkContext &contex);
	~MidiInputService() {}

private:

	
	_2Real::OutletHandle			m_InputMidi;
	RtMidiIn						*m_midiInput;
};



class MidiOutputService : public _2Real::Block
{

public:

	MidiOutputService() {}
	void shutdown() {}
	void update();
	void setup(_2Real::FrameworkContext &contex);
	~MidiOutputService() {}

private:
		 
	_2Real::OutletHandle			m_OutputMidi;
	RtMidiOut						*m_midiOutput;
};