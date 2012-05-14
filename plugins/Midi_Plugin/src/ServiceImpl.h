#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"

#include <iostream>
#include "RtMidi.h"

class MidiInputService : public _2Real::IService
{

public:

	MidiInputService() {}
	void shutdown();
	void update();
	void setup(_2Real::ServiceContext &context);
	~MidiInputService() {}

private:

	int								m_CurrentCount;
	_2Real::OutputHandle			m_CounterValue;
	_2Real::OutputHandle			m_Test;
	RtMidiIn						*m_midiInput;
};



class MidiOutputService : public _2Real::IService
{

public:

	MidiOutputService() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~MidiOutputService() {}

private:
		 
	_2Real::OutputHandle			m_OutputMidi;
	RtMidiOut						*m_midiOutput;
};