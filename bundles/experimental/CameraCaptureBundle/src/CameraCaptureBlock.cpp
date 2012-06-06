#include "CameraCaptureBlock.h"

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

void CameraCaptureBlock::setup( FrameworkContext &context )
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

void CameraCaptureBlock::update()
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