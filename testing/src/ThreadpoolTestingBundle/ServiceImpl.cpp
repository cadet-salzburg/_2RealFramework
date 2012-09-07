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
		++m_Out.getWriteableRef< long >();
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
		std::cout << "--MULTIIN SETUP--" << std::endl;
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
		std::cout << "--MULTIIN UPDATE--" << std::endl;
		ostringstream msg;
		msg << m_Msg.getReadableRef< string >() << " size is " << m_In.getSize() << endl;
		cout << msg.str();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};