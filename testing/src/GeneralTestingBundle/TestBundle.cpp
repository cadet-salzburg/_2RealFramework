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

using std::string;

void getBundleMetainfo( BundleMetainfo &info )
{
	try
	{
		info.setDescription( "bla bla bla" );
		info.setAuthor( "help@cadet.at" );
		info.setCategory( "context testing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 0, 0 );

		ContextBlockMetainfo &context = info.exportContextBlock< ContextManager >();

		BlockMetainfo test = info.exportBlock< TestBlock, WithContext >( "test block" );
		test.setDescription( "testing the context's functionality" );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}