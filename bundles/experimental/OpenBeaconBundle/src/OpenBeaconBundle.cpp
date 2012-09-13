#include "_2RealBundle.h"
#include "OpenBeaconBlock.h"

#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "OpenBeaconBundle" );
		info.setDescription( "OpenBeacon life stream data bundle" );
		info.setAuthor( "Michael Platz, Roland Haring" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo openBeacon = info.exportBlock< OpenBeaconBlock, WithoutContext>( "OpenBeaconBlock" );
		openBeacon.setDescription( "OpenBeacon life stream data" );
		openBeacon.setCategory( "Devices" );

		openBeacon.addInlet< int >( "UDPport", 0 );
		openBeacon.addOutlet< std::vector<Point> >( "tags" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}