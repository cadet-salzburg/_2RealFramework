#include "ServiceImpl.h"
#include "_2RealDatatypes.h"

#include "bundle/_2RealBlockHandle.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using _2Real::bundle::BlockHandle;
using _2Real::bundle::ContextBlock;
using _2Real::Exception;

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;

void Out::setup( BlockHandle &handle )
{
	try
	{
		m_Msg = handle.getInletHandle( "out_msg" );
		m_Out = handle.getOutletHandle( "out_outlet" );
		m_Out.getWriteableRef< long >() = 100;
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
		ostringstream msg;
		msg << m_Msg.getReadableRef< string >() << " " << ++m_Out.getWriteableRef< long >() << endl;
		cout << msg.str();
		//m_Out.getWriteableRef< long >();
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
		m_In = handle.getInletHandle( "inout_inlet" );
		m_Msg = handle.getInletHandle( "inout_msg" );
		m_Out = handle.getOutletHandle( "inout_outlet" );
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
		ostringstream msg;
		msg << m_Msg.getReadableRef< string >() << " " << m_In.getReadableRef< double >() << endl;
		cout << msg.str();
		m_Out.getWriteableRef< double >() = m_In.getReadableRef< double >();
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
		m_In = handle.getInletHandle( "in_inlet" );
		m_InOptions = handle.getInletHandle( "in_inlet_options" );
		m_Msg = handle.getInletHandle( "in_msg" );
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
		ostringstream msg;
		msg << m_Msg.getReadableRef< string >() << " " << m_In.getReadableRef< int >() << endl;
		cout << msg.str();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void MultiIn::setup( BlockHandle &handle )
{
	try
	{
		m_In = handle.getInletHandle( "multiin_inlet" );
		m_InOptions = handle.getInletHandle( "multiin_inlet_options" );
		m_Msg = handle.getInletHandle( "multiin_msg" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void MultiIn::update()
{
	try
	{
		unsigned int sz = m_In.getSize();

		int sum = 0;
		for ( unsigned int i = 0; i<sz; ++i )
		{
			sum += m_In.getReadableRef< int >();
		}

		ostringstream msg;
		msg << m_Msg.getReadableRef< string >() << " nr of items is " << sz << endl;
		msg << m_Msg.getReadableRef< string >() << " sum of items is " << sum << endl;
		cout << msg.str();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};