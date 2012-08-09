#include "ServiceImpl.h"

#include <iostream>

using _2Real::bundle::BlockHandle;
using _2Real::bundle::ContextBlock;
using _2Real::Exception;

using std::cout;
using std::endl;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ContextManager::setup( BlockHandle &handle )
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

Out::Out( ContextBlock &context ) :
	Block(),
	m_Context( dynamic_cast< ContextManager & >( context ) )
{
}

void Out::setup( BlockHandle &handle )
{
	try
	{
		std::cout << "SETUP OUT" << std::endl;
		m_Out = handle.getOutletHandle( "outlet" );
		m_Discard = handle.getOutletHandle( "discard" );
		m_Out.getWriteableRef< unsigned int >() = 0;
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void Out::update()
{
	try
	{
		++m_Out.getWriteableRef< unsigned int >();
		m_Discard.discard();
		std::cout << "OUT " << m_Out.getWriteableRef< unsigned int >() << std::endl;
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void Out::shutdown()
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

In::In( ContextBlock &context ) :
	Block(),
	m_Context( dynamic_cast< ContextManager & >( context ) )
{
}

void In::setup( BlockHandle &handle )
{
	try
	{
		std::cout << "SETUP IN" << std::endl;

		m_Block = handle;

		m_In = m_Block.getInletHandle( "inlet" );
		m_Counter = 0;

		if ( m_In.hasUpdated() )
		{
			cout << "UPDATED (SETUP)" << endl;
		}

		if ( m_In.hasChanged() )
		{
			cout << "CHANGED (SETUP)" << endl;
		}
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void In::update()
{
	try
	{
		if ( m_In.hasUpdated() )
		{
			cout << "UPDATED (UPDATE)" << endl;
		}

		if ( m_In.hasChanged() )
		{
			cout << "CHANGED (UPDATE)" << endl;
		}

		cout << m_Block.getAllInletHandles().size() << std::endl;
		cout << m_Block.getAllOutletHandles().size() << std::endl;
		std::cout << "IN inlet" << m_In.getReadableRef< unsigned int >() << std::endl;
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}

void In::shutdown()
{
	try
	{
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}