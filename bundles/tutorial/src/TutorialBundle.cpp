#include "MultiplierBlock.h"

#include "_2RealBundle.h"
#include <sstream>
#include <iostream>

using namespace _2Real;

using namespace _2Real::bundle;
using			_2Real::Exception;
using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setDescription( "Tutorial Bundle" );
		info.setAuthor( "Robert Praxmarer" );
		info.setCategory( "Tutorial" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo multiplierBlock = info.exportBlock< MultiplierBlock, WithoutContext >( "MultiplierBlock" );

		multiplierBlock.addInlet<int>( "NumberA", 0 );
		multiplierBlock.addInlet<int>( "NumberB", 0 );
		multiplierBlock.addOutlet<int>( "Result" );

		multiplierBlock.setDescription( "Multiplier Block" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}