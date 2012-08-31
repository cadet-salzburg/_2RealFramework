#include "_2RealBundle.h"

#include "MovingHeadBlock.h"
#include "MovingHeadTrackingBlock.h"

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

// Defines the MovingHead Bundle which consists of one MovingHead Block
void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		// Bundle information
		info.setName( "MovingHeadBundle" );
		info.setDescription( "Sending MovingHead Commands from Tracking-Camera to Serial Port" );
		info.setAuthor( "Roland Haring, Veronika Pauser" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		// MovingHeadInput Block information as well as In and Outlet definition
		BlockMetainfo MovingHead = info.exportBlock< MovingHeadBlock, WithContext >( "MovingHeadBlock" );
				
		MovingHead.addInlet<unsigned int>( "MotorID", 0 );
		MovingHead.addInlet<unsigned char>( "Command", 0 );
		MovingHead.addInlet<unsigned int>( "Value", 0 );
		MovingHead.addOutlet<std::vector<unsigned char>>( "SerialByteStream" );

		MovingHead.setDescription( "MovingHead Control" );


		// MovingHeadOutput Block information as well as In and Outlet definition
		BlockMetainfo MovingHeadTracking = info.exportBlock< MovingHeadTrackingBlock, WithContext >( "MovingHeadTrackingBlock" );
				
		MovingHeadTracking.addInlet<_2Real::Point>( "CentorOfMass", _2Real::Point() );
		MovingHeadTracking.addInlet<unsigned int>( "MotorIdX", 0 );
		MovingHeadTracking.addInlet<unsigned int>( "MotorIdY", 0 );
		MovingHeadTracking.addInlet<unsigned int>( "UserID", 0 );
		MovingHeadTracking.addOutlet<unsigned int>( "MotorID" );
		MovingHeadTracking.addOutlet<unsigned char>( "Command" );
		MovingHeadTracking.addOutlet<unsigned int>( "Value" );

		MovingHeadTracking.setDescription( "MovingHead Tracking" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}