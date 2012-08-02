#include "ServiceImpl.h"

#include "_2RealBundle.h"

using namespace _2Real::bundle;
using namespace _2Real;

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

		BlockMetainfo &out = info.exportBlock< Out, WithoutContext >( "out" );
		out.setDescription( "test" );
		out.addInlet< string >( "out_msg", "undefined" );
		out.addOutlet< long >( "out_outlet" );

		BlockMetainfo &inout = info.exportBlock< InOut, WithoutContext >( "in_out" );
		inout.setDescription( "test" );
		inout.addInlet< string >( "inout_msg", "undefined" );
		inout.addInlet< double >( "inout_inlet", 0. );
		inout.addOutlet< double >( "inout_outlet" );

		BlockMetainfo &in = info.exportBlock< In, WithoutContext >( "in" );
		in.setDescription( "test" );
		in.addInlet< string >( "in_msg", "undefined" );
		in.addInlet< Number >( "in_inlet", Number( 0x0000 ) );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}