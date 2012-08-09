#include "_2RealBundle.h"
#include "WindowBlock.h"
#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "SFMLBundle" );
		info.setDescription( "xxxx" );
		info.setAuthor( "gerlinde emsenhuber" );
		info.setCategory( "xxxx" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo win = info.exportBlock< WindowBlock, WithoutContext >( "SFMLWindowBlock" );
		win.setDescription( "xxxx" );
		win.setCategory( "xxxx" );

		//win.addInlet< _2Real::Mat4 >( "test_mat4", _2Real::Mat4::Identity() );

		// testing mat4-->scale
		_2Real::Mat4 matScale;
		matScale << 0.5, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 1.0;
		win.addInlet< _2Real::Mat4 >( "scale_mat", matScale );

		// testing vec3-->clear color
		_2Real::Vec3 vecClear;
		vecClear << 0, 0.5, 0.8;
		win.addInlet< Vec3 >( "clear_color", vecClear );

		win.addInlet< std::string >( "window_title", "yay" );
	}
	catch ( Exception &e )
	{
		std::cout << e.message() << std::endl;
		e.rethrow();
	}
}