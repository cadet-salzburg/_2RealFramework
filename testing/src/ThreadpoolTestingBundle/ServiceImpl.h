#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"

#include "Poco/Mutex.h"

class TestContext : public _2Real::ContextBlock
{

public:

	TestContext();

	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );

	unsigned int getCurrentValue();
	unsigned int getCounterValue();

private:

	Poco::FastMutex		m_Access;
	unsigned int		m_Counter;
	unsigned int		m_Val;

};

class Out : public _2Real::Block
{

public:

	Out( _2Real::ContextBlock &context ) : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );

private:

	int								m_Counter;
	_2Real::OutletHandle			m_Out;

};

class InOut : public _2Real::Block
{

public:

	InOut( _2Real::ContextBlock &context ) : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );

private:

	int								m_Counter;
	_2Real::InletHandle				m_In;
	_2Real::OutletHandle			m_Out;

};

class In : public _2Real::Block
{

public:

	In( _2Real::ContextBlock &context ) : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );

private:

	int								m_Counter;
	_2Real::InletHandle				m_In;

};