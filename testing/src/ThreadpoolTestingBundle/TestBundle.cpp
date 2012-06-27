#include "ServiceImpl.h"

#include "_2RealBundle.h"
#include "_2RealBundleMetainfo.h"
#include "_2RealBlockMetainfo.h"
#include "_2RealContextBlockMetainfo.h"
#include "_2RealException.h"

using _2Real::BundleMetainfo;
using _2Real::BlockMetainfo;
using _2Real::ContextBlockMetainfo;
using _2Real::Exception;
using _2Real::WithContext;
using _2Real::WithoutContext;

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
		in.addParameter< string >( "in msg", "undefined" );
		in.addInlet< unsigned int >( "in inlet", (unsigned int)0 );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}