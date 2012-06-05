#include "ServiceImpl.h"

#include "_2RealBundle.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

using _2Real::BundleMetainfo;
using _2Real::BlockMetainfo;
using _2Real::Enumeration;
using _2Real::Enums;
using _2Real::Exception;

using std::vector;
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

		BlockMetainfo counter = info.exportBlock< Counter >( "counter" );
		counter.setDescription( "count from up from 0" );
		counter.addOutlet< unsigned int >( "counter outlet", (unsigned int)0 );

		BlockMetainfo doubler = info.exportBlock< Doubler >( "doubler" );
		doubler.setDescription( "double the inlets' value" );
		doubler.addInlet< unsigned int >( "doubler inlet", (unsigned int)0 );
		doubler.addOutlet< unsigned int >( "doubler outlet", (unsigned int)0 );

		BlockMetainfo printOut = info.exportBlock< PrintOut >( "print out" );
		printOut.setDescription( "write the received value to std::out" );
		printOut.addInlet< unsigned int >( "printout inlet", (unsigned int)0 );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}