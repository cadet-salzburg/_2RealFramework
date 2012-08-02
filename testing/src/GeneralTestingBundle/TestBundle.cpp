#include "ServiceImpl.h"

#include "_2RealBundle.h"

#include <vector>
#include <string>

using _2Real::bundle::BundleMetainfo;
using _2Real::bundle::BlockMetainfo;
using _2Real::bundle::ContextBlockMetainfo;
using _2Real::bundle::WithContext;
using _2Real::Exception;

using std::string;
using std::vector;

void getBundleMetainfo( BundleMetainfo &info )
{
	try
	{
		info.setName( "tralala" );
		info.setDescription( "bla bla bla" );
		info.setAuthor( "help@cadet.at" );
		info.setCategory( "context testing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 0, 0 );

		ContextBlockMetainfo &context = info.exportContextBlock< ContextManager >();

		BlockMetainfo out = info.exportBlock< Out, WithContext >( "out" );
		out.setDescription( "testing the context's functionality" );
		out.addOutlet< unsigned int >( "outlet" );
		out.addOutlet< unsigned int >( "discard" );
		out.addOutlet< vector< string > >( "whatever" );

		BlockMetainfo in = info.exportBlock< In, WithContext >( "in" );
		in.setDescription( "testing the context's functionality" );
		in.addInlet< unsigned int >( "inlet", (unsigned int)10 );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}