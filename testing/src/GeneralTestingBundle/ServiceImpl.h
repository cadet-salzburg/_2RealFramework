#pragma once

#include "_2RealBlock.h"

#include "Poco/Mutex.h"

class ContextManager : public _2Real::ContextBlock
{

public:

	void update();
	void setup( _2Real::FrameworkContext &context );
	void shutdown();

	unsigned long getValue() const;

private:

	mutable Poco::FastMutex			m_Access;
	unsigned long					m_Value;

};

class TestBlock : public _2Real::Block
{

public:

	TestBlock( _2Real::ContextBlock &context );

	void update();
	void setup( _2Real::FrameworkContext &context );
	void shutdown();

private:

	ContextManager				&m_Context;
	unsigned int				m_Counter;

};
