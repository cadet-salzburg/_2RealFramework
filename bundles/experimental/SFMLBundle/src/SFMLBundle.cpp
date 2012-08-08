#include "_2RealBundle.h"
#include "WindowBlock.h"
#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "SFMLWindowBundle" );
		info.setDescription( "xxxx" );
		info.setAuthor( "gerlinde emsenhuber" );
		info.setCategory( "xxxx" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo win = info.exportBlock< WindowBlock, WithoutContext >( "SFMLWindowBlock" );
		win.setDescription( "xxxx" );
		win.setCategory( "xxxx" );
		win.addInlet< float >( "bg_color_val_r", 0.5f );
		win.addInlet< float >( "bg_color_val_g", 0.5f );
		win.addInlet< float >( "bg_color_val_b", 0.5f );
		win.addInlet< string >( "window_title", "yay" );

		//BlockMetainfo tex = info.exportBlock< TexBlock, WithoutContext >( "SFMLTextureBlock" );
		//tex.setDescription( "xxxx" );
		//tex.setCategory( "xxxx" );
		//tex.addOutlet< string >( "error_state" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}