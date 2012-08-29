#include <_2RealDatatypes.h>

#include <windows.h>
#include <iostream>

using namespace _2Real;

int main( int argc, char *argv[] )
{
	try
	{
	}
	catch( std::exception &e )
	{
		std::cerr << "[ERROR] caught exception: " << e.what() << std::endl;
	}
	getchar();
}