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
		info.setDescription( "WebCam bundle" );
		info.setAuthor( "ahmed tolba" );
		info.setCategory( "video" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 0, 0 );

		BlockMetainfo camera = info.exportBlock< CameraService >( "CameraService" );
		camera.setDescription( "grabs data from a webcam" );
		camera.addOutlet< ImageT< unsigned char > >( "camera image", ImageT< unsigned char >() );
		camera.addInlet< bool >( "fast assign", false );
	}
	catch ( _2Real::Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

 