#include "MultiplierBlock.h"
#include "ThroughputBlock.h"

#include "_2RealBundle.h"
#include "datatypes\_2RealFieldDescriptor.h"
#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;

using _2Real::Exception;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "TutorialBundle" );
		info.setDescription( "Tutorial Bundle" );
		info.setAuthor( "Robert Praxmarer" );
		info.setCategory( "Tutorial" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		TypeMetainfo &testType = info.exportCustomType( "testtype" );
		testType.addField< int >( "test int" );
		testType.addField< std::string >( "test string" );
		testType.addField< std::vector< float > >( "test float array" );

		// create custom type of the template
		CustomType initialValue( testType );
		initialValue.set< int >( "test int", 0 );
		initialValue.set< std::string >( "test string", "NARF" );

		// this would create a 'custom' type containing only one field, an integer
		// but luckily, there's a shortcut ;)
		//CustomType customIntType( metainfo< int >() );
		//customIntType.set< int >( "default", 0 );
		//customIntType.setRange( "default", Range_t< int >( -1000, -10 ) );

		BlockMetainfo customTypeBlock = info.exportBlock< MultiplierBlock, WithoutContext >( "MultiplierBlock" );
		customTypeBlock.setDescription( "Custom Type Block" );
		//customTypeBlock.addInlet< int >( "int inlet 0" );										// no default, no range
		//customTypeBlock.addInlet< int >( "int inlet 1", 10 );									// default, no range
		//customTypeBlock.addInlet< int >( "int inlet 2", 10, Range_t< int >( -2, 10 ) );			// default + range
		//customTypeBlock.addInlet( "int inlet 3", "int", customIntType );						// default val & range are stored in 'customIntType'
		customTypeBlock.addInlet( "custom inlet", "testtype" );									// make the type explicit, to avoid confusion
		customTypeBlock.addInlet( "custom inlet", "testtype", initialValue );					// range & values for all fiels are stored in the initialValue

		customTypeBlock.addOutlet( "custom outlet", "testtype" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
