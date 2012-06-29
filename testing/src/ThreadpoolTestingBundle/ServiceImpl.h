#pragma once

#include "_2RealBlock.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "bundle/_2RealParameterHandle.h"

#include "Poco/Mutex.h"

class TestContext : public _2Real::bundle::ContextBlock
{

public:

	TestContext();

	void shutdown() {}
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

	unsigned int getCurrentValue();
	unsigned int getCounterValue();

private:

	Poco::FastMutex		m_Access;
	unsigned int		m_Counter;
	unsigned int		m_Val;

};

class Out : public _2Real::bundle::Block
{

public:

	Out( _2Real::bundle::ContextBlock &context ) : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

private:

	_2Real::bundle::OutletHandle			m_Out;

};

class InOut : public _2Real::bundle::Block
{

public:

	InOut( _2Real::bundle::ContextBlock &context ) : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

private:

	_2Real::bundle::InletHandle				m_In;
	_2Real::bundle::OutletHandle			m_Out;

};

class In : public _2Real::bundle::Block
{

public:

	In( _2Real::bundle::ContextBlock &context ) : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

private:

	int									m_Counter;
	_2Real::bundle::InletHandle			m_In;
	_2Real::bundle::ParameterHandle		m_Param;

};