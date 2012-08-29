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
		info.setDescription( "mmmh, beacon... wait." );
		info.setAuthor( "michael platz" );
		info.setCategory( "" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo openBeacon = info.exportBlock< OpenBeaconBlock, WithoutContext>( "OpenBeaconBlock" );
		openBeacon.setDescription( "creates 3D vertex and normal data for faces find in an RGBD image stream" );
		openBeacon.setCategory( "" );	//TODO: set

		openBeacon.addInlet< int >( "dummy_in", 0 );
		openBeacon.addOutlet< int >( "dummy_out" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}