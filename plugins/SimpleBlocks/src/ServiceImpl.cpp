#include "ServiceImpl.h"

#include "_2RealFrameworkContext.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

#include <iostream>
#include <vector>
#include <string>

using _2Real::FrameworkContext;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::vector;
using std::string;

void BlockManager::setup( FrameworkContext &context )
{
	try
	{
		m_BundleEnum = context.getOutletHandle( "bundle enum" );
		m_BundleVec = context.getOutletHandle( "config text" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void BlockManager::update()
{
	try
	{
		m_BundleVec.data< vector< string > >().push_back( "yay" );
		m_BundleVec.data< vector< string > >().push_back( "updated" );
		m_BundleVec.data< vector< string > >().push_back( "again" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void Counter::setup( FrameworkContext &context )
{
	try
	{
		m_CurrentCount = 1;
		m_CounterValue = context.getOutletHandle( "counter outlet" );
		m_TestEnum = context.getInletHandle( "bundle enum" );
		m_TestVec = context.getInletHandle( "config text" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void Counter::update()
{
	try
	{
		m_CounterValue.data< unsigned int >() = ++m_CurrentCount;

		vector< string > const& vec = m_TestVec.data< vector < string > >();

		for ( vector< string >::const_iterator it = vec.begin(); it != vec.end(); ++it )
		{
			cout << *it << endl;
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void Doubler::setup( FrameworkContext &context )
{
	try
	{
		m_InputValue = context.getInletHandle( "doubler inlet" );
		m_OutputValue = context.getOutletHandle( "doubler outlet" );
		m_TestEnum = context.getInletHandle( "bundle enum" );
		m_TestVec = context.getInletHandle( "config text" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void Doubler::update()
{
	try
	{
		m_OutputValue.data< unsigned int>() = 2 * m_InputValue.data< unsigned int >();
	}
	catch ( Exception &e)
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void PrintOut::setup( FrameworkContext &context )
{
	try
	{
		m_InputValue = context.getInletHandle( "printout inlet" );
		m_TestEnum = context.getInletHandle( "bundle enum" );
		m_TestVec = context.getInletHandle( "config text" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void PrintOut::update()
{
	try
	{
		cout << "PrintOut: " << m_InputValue.data< unsigned int >() << endl;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};