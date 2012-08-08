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

		// ShapeWrapDeviceManager ContextBlock definition
		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< ShapeWrapDeviceManager >();

		// ShapeWrapInput Block information as well as In and Outlet definition
		BlockMetainfo ShapeWrap = info.exportBlock< ShapeWrapBlock, WithContext >( "ShapeWrapInBlock" );
		ShapeWrap.addInlet<std::string>( "ShapeWrapPort", "" );
		ShapeWrap.addInlet<unsigned int>( "BaudRate", 0 );

		ShapeWrap.addInlet<std::vector<unsigned char>>( "Read", std::vector<unsigned char>());
		ShapeWrap.addOutlet<std::vector<unsigned char>>( "Write" );

		ShapeWrap.setDescription( "ShapeWrap Port" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}