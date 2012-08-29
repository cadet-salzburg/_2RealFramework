#include "_2RealBundle.h"
#include "NatNetBlock.h"
#include <sstream>
#include <iostream>
#include <string>

using namespace _2Real;
using namespace _2Real::bundle;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "NatNetBundle" );
		info.setDescription( "OptiTrack plugin" );
		info.setAuthor( "Veronika Pauser" );
		info.setCategory( "Tracking" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo natNet = info.exportBlock< NatNetBlock, WithoutContext>( "NatNetBlock" );
		natNet.setDescription( "Plugin for accessing data from OptiTrack system." );

		natNet.addInlet< string >( "server_ip", "192.168.5.67" ); //127.0.0.1
		natNet.addInlet< string >( "client_ip", "192.168.5.67" ); //127.0.0.1
		natNet.addInlet< bool >( "isUnicast", false );

		natNet.addOutlet< std::vector < _2Real::Point > >( "marker_set" );
		natNet.addOutlet< std::vector < _2Real::Point > >( "other_marker" );
		natNet.addOutlet< std::vector < _2Real::RigidBody > >( "rigid_body" );
		natNet.addOutlet< std::vector < _2Real::Skeleton > >( "skeleton" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}