#include "_2RealBundle.h"

#include "EMGShieldBlock.h"

#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;

using namespace _2Real::bundle;
using			_2Real::Exception;
using std::string;
using std::cout;
using std::endl;

// Defines the EMGShield Bundle which consists of one EMGShield Block
void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		// Bundle information
		info.setName( "EMGShieldBundle" );
		info.setDescription( "Sending EMGShield Commands to Serial Port" );
		info.setAuthor( "Roland Haring" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		// EMGShieldInput Block information as well as In and Outlet definition
		BlockMetainfo EMGShield = info.exportBlock< EMGShieldBlock, WithContext >( "EMGShieldBlock" );
				
		EMGShield.addInlet<unsigned int>( "MotorID", 0 );
		EMGShield.addInlet<unsigned char>( "Command", 0 );
		EMGShield.addInlet<unsigned int>( "Value", 0 );

		EMGShield.addInlet<std::vector<unsigned char>>( "SerialByteStream",  std::vector<unsigned char>());

		EMGShield.setDescription( "EMGShield Control" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}