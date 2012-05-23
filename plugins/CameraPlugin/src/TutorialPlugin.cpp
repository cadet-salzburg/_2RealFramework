#include "ServiceImpl.h"

#include "_2RealFrameworkContext.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"
#include "_2RealBundle.h"

#include "_2RealImageT.h"

#include <iostream>

using _2Real::BundleMetainfo;
using _2Real::BlockMetainfo;
using _2Real::Exception;
using _2Real::ImageT;
using _2Real::ImageChannelOrder;

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

		//ImageT< unsigned char > img( 640, 480, ImageChannelOrder::RGB );
		//ImageT< unsigned char >::iterator it = img.iter();
		//while( it.nextLine() )
		//{
		//	while( it.nextPixel() )
		//	{
		//		it.r() = 0;
		//		it.g() = 0;
		//		it.b() = 0;
		//	}
		//}

		BlockMetainfo camera = info.exportBlock< CameraService >( "CameraService" );
		camera.setDescription( "count from up from 0" );
		camera.addOutlet< ImageT< unsigned char > >( "s1 outlet", ImageT< unsigned char >() );
	}
	catch ( _2Real::Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

 