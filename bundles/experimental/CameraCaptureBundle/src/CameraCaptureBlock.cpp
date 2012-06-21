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
		m_IntOutletHandle = context.getOutletHandle( "intValue" );
		m_FloatOutletHandle = context.getOutletHandle( "floatValue" );
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
		static int i=0;
		static float f=0;
		m_IntOutletHandle.getDataRef<int>()++;
		m_FloatOutletHandle.getDataRef<float>() = f+=0.01;


	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};