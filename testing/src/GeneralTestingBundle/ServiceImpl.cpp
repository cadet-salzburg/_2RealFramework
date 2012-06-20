#include "ServiceImpl.h"

#include "_2RealFrameworkContext.h"
#include "_2RealException.h"

#include <iostream>

using _2Real::FrameworkContext;
using _2Real::Exception;
using _2Real::ContextBlock;

using std::cout;
using std::endl;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ContextManager::setup( FrameworkContext &context )
{
	try
	{
		m_Value = 0;
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ContextManager::update()
{
	try
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_Value += 1000;
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void ContextManager::shutdown()
{
	try
	{
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

unsigned long ContextManager::getValue() const
{
	Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
	return m_Value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

TestBlock::TestBlock( ContextBlock &context ) :
	Block(),
	m_Context( dynamic_cast< ContextManager & >( context ) )
{
}

void TestBlock::setup( FrameworkContext &context )
{
	try
	{
		m_Counter = 0;
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void TestBlock::update()
{
	try
	{
		if ( ++m_Counter == 100 )
		{
			cout << m_Context.getValue() + m_Counter << endl;
			m_Counter = 0;
		}
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void TestBlock::shutdown()
{
	try
	{
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////