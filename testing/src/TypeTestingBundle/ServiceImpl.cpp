#include "ServiceImpl.h"
#include "_2RealDatatypes.h"

#include "bundle/_2RealBlockHandle.h"
#include "datatypes/_2RealCustomData.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace _2Real::bundle;
using _2Real::Exception;
using _2Real::CustomType;

using namespace std;

unsigned int creationCount = 0;

Test::Test() : Block(), mCounter( 0 ), mNumber( ++ creationCount )
{
	cout << "CTOR" << endl;
}

void Test::setup( BlockHandle &handle )
{
	try
	{
		cout << "SETUP " <<  mNumber << endl;
		mHandle = handle;
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
		//cout << "UPDATE BEGIN " << mNumber << endl;
		InletHandle in0 = mHandle.getInletHandle( "customInlet0" );
		InletHandle in1 = mHandle.getInletHandle( "customInlet1" );

		CustomType const& i0 = in0.getReadableRef();
		int i = i0.get< int >( "test int" );
		//cout << i << endl;
		int o = i + ( ++mCounter );

		OutletHandle out0 = mHandle.getOutletHandle( "customOutlet0" );
		CustomType &o0 = out0.getWriteableRef();
		o0.set< int >( "test int", o );
		//cout << "UPDATE DONE " << mNumber << endl;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void Test::shutdown()
{
	try
	{
		cout << "SHUTDOWN " << mNumber << endl;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

Test::~Test()
{
	cout << "DTOR " << mNumber << endl;
}