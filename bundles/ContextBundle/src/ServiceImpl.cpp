#include "ServiceImpl.h"

#include "_2RealFrameworkContext.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

#include <iostream>
#include <string>

using _2Real::FrameworkContext;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

void Context::setup( FrameworkContext &context )
{
	try
	{
		m_ContextEnum = context.getOutletHandle( "context enum" );
		m_ContextNumber = context.getOutletHandle( "context number" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void Context::update()
{
	try
	{
		m_ContextNumber.data< unsigned int >() = rand();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void Test::setup( FrameworkContext &context )
{
	try
	{
		m_ContextEnum = context.getInletHandle( "context enum" );
		m_ContextNumber = context.getInletHandle( "context number" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void Test::update()
{
	try
	{
		cout << "test block: context nr = " << m_ContextNumber.data< unsigned int >() << endl;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};