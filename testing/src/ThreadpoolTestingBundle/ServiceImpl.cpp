#include "ServiceImpl.h"

#include "_2RealBlockHandle.h"
#include "_2RealException.h"

#include <iostream>
#include <vector>
#include <string>

using _2Real::bundle::BlockHandle;
using _2Real::bundle::ContextBlock;
using _2Real::Exception;

using std::cout;
using std::endl;
using std::string;

TestContext::TestContext() :
	ContextBlock(),
	m_Counter( 100 )
{
}

void TestContext::setup( BlockHandle &handle )
{
	try
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_Val = 2;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void TestContext::update()
{
	try
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_Val *= 4;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

unsigned int TestContext::getCounterValue()
{
	Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
	return ++m_Counter;
}

unsigned int TestContext::getCurrentValue()
{
	Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
	return m_Val;
}

void Out::setup( BlockHandle &handle )
{
	try
	{
		m_Out = handle.getOutletHandle( "out outlet" );
		m_Out.getWriteableRef< unsigned int >() = 0;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void Out::update()
{
	try
	{
		++m_Out.getWriteableRef< unsigned int >();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void InOut::setup( BlockHandle &handle )
{
	try
	{
		m_In = handle.getInletHandle( "inout inlet" );
		m_Out = handle.getOutletHandle( "inout outlet" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void InOut::update()
{
	try
	{
		cout << m_In.getReadableRef< unsigned int >() << endl;
		m_Out.getWriteableRef< unsigned int>() = m_In.getReadableRef< unsigned int >();
	}
	catch ( Exception &e)
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void In::setup( BlockHandle &handle )
{
	try
	{
		m_In = handle.getInletHandle( "in inlet" );
		m_Param = handle.getParameterHandle( "in msg" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void In::update()
{
	try
	{
		cout << m_Param.getReadableRef< string >() << " " << m_In.getReadableRef< unsigned int >() << endl;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};