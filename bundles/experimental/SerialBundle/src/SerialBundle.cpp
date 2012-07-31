#include "_2RealBundle.h"

#include "SerialBlock.h"

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

// Defines the Serial Bundle which consists of two Blocks, one SerialIn and one SerialOut Block
void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		// Bundle information
		info.setName( "SerialBundle" );
		info.setDescription( "Serial In and Out" );
		info.setAuthor( "Roland Haring" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		// SerialDeviceManager ContextBlock definition
		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< SerialDeviceManager >();

		// SerialInput Block information as well as In and Outlet definition
		BlockMetainfo serial = info.exportBlock< SerialBlock, WithContext >( "SerialInBlock" );
		serial.addInlet<unsigned int>( "SerialPort", 0 );
		serial.addInlet<unsigned int>( "BaudRate", 0 );

		std::vector<unsigned char> empty;
		serial.addInlet<std::vector<unsigned char>>( "Read", empty);
		serial.addOutlet<std::vector<unsigned char>>( "Write" );

		serial.setDescription( "Serial Port" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}