#include <_2RealDatatypes.h>

#include <windows.h>
#include <iostream>

using namespace _2Real;

int main( int argc, char *argv[] )
{
	try
	{
		Space3D space1( Vec3( 1.0 ), Vec3( 2.0f ) );
		std::cout << space1.extends() << std::endl;
		std::cout << space1.size() << std::endl;

		Space3D space2( Vec3( 10.0, 20.0f, 15.0f ), Vec3( 2.0f, 1.0f, 5.0f ) );
		std::cout << space2.extends() << std::endl;
		std::cout << space2.size() << std::endl;
	}
	catch( std::exception &e )
	{
		std::cerr << "[ERROR] caught exception: " << e.what() << std::endl;
	}
	getchar();
}