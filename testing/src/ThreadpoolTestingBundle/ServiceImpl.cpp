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

Test::Test() : Block()
{
}

void Test::setup( BlockHandle &handle )
{
	try
	{
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
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};