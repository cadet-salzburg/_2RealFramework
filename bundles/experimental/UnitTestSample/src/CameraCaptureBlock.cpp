#include "CameraCaptureBlock.h"

#include <iostream>
#include <string>

using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

void CameraCaptureBlock::setup( BlockHandle &context )
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
		m_DoubleOutletHandle.getWriteableRef<double>()=m_DoubleInletHandle.getReadableRef<double>();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};