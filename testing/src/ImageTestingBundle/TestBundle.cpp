#include "ServiceImpl.h"

#include "_2RealBundle.h"

using namespace _2Real::bundle;
using _2Real::ImageT;
using _2Real::Exception;

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

		BlockMetainfo imgOut = info.exportBlock< ImageOut, WithoutContext >( "image out" );
		imgOut.setDescription( "testing the context's functionality" );
		imgOut.addOutlet< ImageT< float > >( "image outlet" );

		BlockMetainfo imgInOut = info.exportBlock< ImageInOut, WithoutContext >( "image in - out" );
		imgInOut.setDescription( "testing the context's functionality" );
		imgInOut.addInlet< ImageT< float > >( "image inlet", ImageT< float >() );
		imgInOut.addOutlet< ImageT< float > >( "image outlet" );

		BlockMetainfo imgIn = info.exportBlock< ImageIn, WithoutContext >( "image in" );
		imgIn.setDescription( "testing the context's functionality" );
		imgIn.addInlet< ImageT< float > >( "image inlet", ImageT< float >() );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}