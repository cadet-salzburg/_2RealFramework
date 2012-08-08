#include "ServiceImpl.h"

#include "_2RealBundle.h"

using namespace _2Real::bundle;
using _2Real::ImageT;
using _2Real::Image;
using _2Real::Vec2;
using _2Real::Vec3;
using _2Real::Exception;

using std::string;

void getBundleMetainfo( BundleMetainfo &info )
{
	try
	{
		info.setName( "imagetest" );
		info.setDescription( "bla bla bla" );
		info.setAuthor( "help@cadet.at" );
		info.setCategory( "context testing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 0, 0 );

		BlockMetainfo imgOut = info.exportBlock< ImageOut, WithoutContext >( "image_out" );
		imgOut.setDescription( "testing the context's functionality" );
		imgOut.addOutlet< ImageT< float > >( "image_outlet" );

		_2Real::Options< int > o = _2Real::Options< int >( 0, "constant" )( 1, "replicate" )( 2, "reflect" )( 4, "reflect_101" );

		BlockMetainfo imgInOut = info.exportBlock< ImageInOut, WithoutContext >( "image_in_out" );
		imgInOut.setDescription( "testing the context's functionality" );
		imgInOut.addInlet< Image >( "image_inlet", Image() );
		imgInOut.addInlet< int >( "image_options", 0, o );
		imgInOut.addOutlet< Image >( "image_outlet" );

		BlockMetainfo imgIn = info.exportBlock< ImageIn, WithoutContext >( "image_in" );
		imgIn.setDescription( "testing the context's functionality" );
		imgIn.addInlet< ImageT< float > >( "image_inlet", ImageT< float >() );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}