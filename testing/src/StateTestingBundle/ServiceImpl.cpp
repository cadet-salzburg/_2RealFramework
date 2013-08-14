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
using _2Real::Image;

using namespace std;

unsigned int instanceCounter = 0;

Test::Test() : Block(), mCounter( 0 ), mId( ++instanceCounter )
{
}

void Test::setup( BlockHandle &handle )
{
	try
	{
		mInstance.block = handle;
		handle.getAllInletHandles( mInstance.inlets );
		handle.getAllOutletHandles( mInstance.outlets );
		handle.getAllParameterHandles( mInstance.parameters );

		std::cout << ".... setup:" << mId << " ...." << std::endl;
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
		for ( unsigned int i=0; i<mInstance.inlets.size(); ++i )
		{
			std::shared_ptr< const CustomType > data = mInstance.inlets[ i ].getReadableRef();
			if ( !data.get() ) std::cout << "DATA FOR INLET " << i<< " IS MISSING!" << std::endl;
		}

		for ( unsigned int i=0; i<mInstance.parameters.size(); ++i )
		{
			std::shared_ptr< const CustomType > data = mInstance.parameters[ i ].getReadableRef();
			if ( !data.get() ) std::cout << "DATA FOR PARAMETER " << i<< " IS MISSING!" << std::endl;
		}

		{
			OutletHandle o = mInstance.outlets[ 0 ];
			std::shared_ptr< CustomType > data = o.getWriteableRef();
			data->set< unsigned int >( "default", ++mCounter );
		}

		std::cout << ".... update:" << mId << " ...." << std::endl;
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
		std::cout << ".... shutdown:" << mId << " ...." << std::endl;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

Test::~Test()
{
}