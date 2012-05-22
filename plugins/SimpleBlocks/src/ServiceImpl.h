#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"

class Counter : public _2Real::Block
{

public:

	Counter() : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );
	~Counter() {}

private:

	int								m_CurrentCount;
	_2Real::OutletHandle			m_CounterValue;

};

class Doubler : public _2Real::Block
{

public:

	Doubler() : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );
	~Doubler() {}

private:

	_2Real::InletHandle				m_InputValue;
	_2Real::OutletHandle			m_OutputValue;

};

class PrintOut : public _2Real::Block
{

public:

	PrintOut() : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );
	~PrintOut() {}

private:

	_2Real::InletHandle				m_InputValue;

};