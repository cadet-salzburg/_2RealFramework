#include "ServiceImpl.h"

#include "_2RealFrameworkContext.h"
#include "_2RealException.h"

#include <iostream>
#include <vector>
#include <string>

using _2Real::FrameworkContext;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

void Out::setup( FrameworkContext &context )
{
	try
	{
		m_Counter = -1;
		m_Out = context.getOutletHandle( "outlet" );
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
		m_Out.data< unsigned int >() = ++m_Counter;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void InOut::setup( FrameworkContext &context )
{
	try
	{
		m_Counter = -1;
		m_In = context.getInletHandle( "inlet" );
		m_Out = context.getOutletHandle( "outlet" );
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
		++m_Counter;
		m_Out.data< unsigned int>() = m_In.data< unsigned int >();
	}
	catch ( Exception &e)
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void In::setup( FrameworkContext &context )
{
	try
	{
		m_Counter = -1;
		m_In = context.getInletHandle( "inlet" );
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
		++m_Counter;
		//if ( m_Counter == 10 )
		//{
			cout << m_In.data< unsigned int >() << endl;
		//	m_Counter = 0;
		//}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};