#include "CameraDeviceManager.h"

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

void CameraDeviceManager::setup( FrameworkContext &context )
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

void CameraDeviceManager::update()
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