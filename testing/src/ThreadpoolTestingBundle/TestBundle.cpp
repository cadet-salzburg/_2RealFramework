#include "ServiceImpl.h"

#include "_2RealBundle.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"

using _2Real::BundleMetainfo;
using _2Real::BlockMetainfo;
using _2Real::Exception;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo info )
{
	try
	{
		info.setDescription( "bla bla bla" );
		info.setAuthor( "help@cadet.at" );
		info.setCategory( "testing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 0, 0 );

		BlockMetainfo out = info.exportBlock< Out >( "out" );
		out.setDescription( "test" );
		out.addOutlet< unsigned int >( "outlet", (unsigned int)0 );

		BlockMetainfo inout = info.exportBlock< InOut >( "in - out" );
		inout.setDescription( "test" );
		inout.addInlet< unsigned int >( "inlet", (unsigned int)0 );
		inout.addOutlet< unsigned int >( "outlet", (unsigned int)0 );

		BlockMetainfo in = info.exportBlock< In >( "in" );
		in.setDescription( "test" );
		in.addInlet< unsigned int >( "inlet", (unsigned int)0 );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}