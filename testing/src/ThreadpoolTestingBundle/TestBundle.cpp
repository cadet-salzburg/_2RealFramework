#include "ServiceImpl.h"

#include "_2RealBundle.h"

using namespace _2Real::bundle;
using _2Real::Exception;

using std::string;

void getBundleMetainfo( BundleMetainfo &info )
{
	try
	{
		info.setDescription( "bla bla bla" );
		info.setAuthor( "help@cadet.at" );
		info.setCategory( "testing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 0, 0 );

		ContextBlockMetainfo &context = info.exportContextBlock< TestContext >();

		BlockMetainfo &out = info.exportBlock< Out, WithContext >( "out" );
		out.setDescription( "test" );
		out.addOutlet< unsigned int >( "out outlet" );

		BlockMetainfo &inout = info.exportBlock< InOut, WithContext >( "in - out" );
		inout.setDescription( "test" );
		inout.addInlet< unsigned int >( "inout inlet", (unsigned int)0 );
		inout.addOutlet< unsigned int >( "inout outlet" );

		BlockMetainfo &in = info.exportBlock< In, WithContext >( "in" );
		in.setDescription( "test" );
		in.addInlet< string >( "in msg", "undefined" );
		in.addInlet< unsigned int >( "in inlet", (unsigned int)0 );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}