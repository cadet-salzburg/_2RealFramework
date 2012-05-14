#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"

#include <iostream>
#include "RtMidi.h"

class Service1 : public _2Real::IService
{

public:

	Service1() {}
	void shutdown();
	void update();
	void setup(_2Real::ServiceContext &context);
	~Service1() {}

private:

	int								m_CurrentCount;
	_2Real::OutputHandle			m_CounterValue;
	_2Real::OutputHandle			m_Test;
	RtMidiIn						*m_midiInput;
};



class Service2 : public _2Real::IService
{

public:

	Service2() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~Service2() {}

private:
		 
	_2Real::OutputHandle			m_OutputMidi;
	RtMidiOut						*m_midiOutput;
};