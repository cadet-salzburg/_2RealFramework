#pragma once

#include "_2RealBlock.h"

#include "Poco/Mutex.h"

class ContextManager : public _2Real::bundle::ContextBlock
{

public:

	void update();
	void setup( _2Real::bundle::BlockHandle &handle );
	void shutdown();

	unsigned long getValue() const;

private:

	mutable Poco::FastMutex			m_Access;
	unsigned long					m_Value;

};

class Out : public _2Real::bundle::Block
{

public:

	Out( _2Real::bundle::ContextBlock &context );

	void update();
	void setup( _2Real::bundle::BlockHandle &handle );
	void shutdown();

private:

	_2Real::bundle::OutletHandle		m_Out;
	_2Real::bundle::OutletHandle		m_Discard;
	ContextManager				&m_Context;
	unsigned int				m_Counter;

};

class In: public _2Real::bundle::Block
{

public:

	In( _2Real::bundle::ContextBlock &context );

	void update();
	void setup( _2Real::bundle::BlockHandle &handle );
	void shutdown();

private:

	_2Real::bundle::InletHandle			m_In;
	_2Real::bundle::ParameterHandle		m_Param;
	ContextManager				&m_Context;
	unsigned int				m_Counter;

};

