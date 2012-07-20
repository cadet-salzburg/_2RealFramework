#include "ServiceImpl.h"

#include "_2RealBundle.h"

using namespace _2Real::bundle;
using _2Real::Exception;

using std::string;

void getBundleMetainfo( BundleMetainfo &info )
{
	try
	{
		info.setName( "threadpool test" );
		info.setDescription( "bla bla bla" );
		info.setAuthor( "help@cadet.at" );
		info.setCategory( "testing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 0, 0 );

		ContextBlockMetainfo &context = info.exportContextBlock< TestContext >();

		BlockMetainfo &out = info.exportBlock< Out, WithContext >( "out" );
		out.setDescription( "test" );
		out.addInlet< string >( "out_msg", "undefined" );
		out.addOutlet< unsigned int >( "out_outlet" );

		BlockMetainfo &inout = info.exportBlock< InOut, WithContext >( "in_out" );
		inout.setDescription( "test" );
		inout.addInlet< string >( "inout_msg", "undefined" );
		inout.addInlet< unsigned int >( "inout_inlet", (unsigned int)0 );
		inout.addOutlet< unsigned int >( "inout_outlet" );

		BlockMetainfo &in = info.exportBlock< In, WithContext >( "in" );
		in.setDescription( "test" );
		in.addInlet< string >( "in_msg", "undefined" );
		in.addInlet< unsigned int >( "in_inlet", (unsigned int)20 );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}