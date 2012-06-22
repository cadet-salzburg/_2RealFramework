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
		m_DoubleInletHandle = context.getInletHandle( "doubleInlet" );
		m_DoubleOutletHandle = context.getOutletHandle( "doubleOutlet" );
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
		m_DoubleOutletHandle.getWriteableRef<double>()+=m_DoubleInletHandle.getReadableRef<double>();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};