#include "_2RealBundle.h"

#include "ShapeWrapBlock.h"

#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;

using namespace _2Real::bundle;
using			_2Real::Exception;
using std::string;
using std::cout;
using std::endl;

// Defines the ShapeWrap Bundle which consists of two Blocks, one ShapeWrapIn and one ShapeWrapOut Block
void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		// Bundle information
		info.setName( "ShapeWrapBundle" );
		info.setDescription( "ShapeWrap In and Out" );
		info.setAuthor( "Roland Haring" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		// ShapeWrapInput Block information as well as In and Outlet definition
		BlockMetainfo ShapeWrap = info.exportBlock< ShapeWrapBlock, WithContext >( "ShapeWrapInBlock" );
		ShapeWrap.setDescription( "ShapeWrap In and Out" );
		ShapeWrap.setCategory( "Devices" );
		
		ShapeWrap.addInlet<int>("ClientPort", 3102);
		ShapeWrap.addInlet<std::string>("ClientAddress", "127.0.0.1");
		ShapeWrap.addInlet<int>("ServerPort", 3100);
		ShapeWrap.addInlet<std::string>("ServerAddress", "127.0.0.1");
		ShapeWrap.addOutlet<_2Real::Skeleton>("Skeleton");
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}