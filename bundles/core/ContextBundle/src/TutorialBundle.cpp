#include "BlocksImpl.h"

#include "_2RealBundle.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

using _2Real::BundleMetainfo;
using _2Real::BlockMetainfo;
using _2Real::Enumeration;
using _2Real::Enums;
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

		info.exportBundleContext< Context >();

		info.addContextParameter< unsigned int >( "context number", (unsigned int)0 );
		info.addContextParameter< Enumeration >( "context enum", Enumeration( Enums( "enum 1", "value 1" )("enum 2", "value 2"), "no value" ) );

		BlockMetainfo counter = info.exportBlock< Test >( "test" );
		counter.setDescription( "test block" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}