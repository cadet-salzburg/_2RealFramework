#include "ServiceImpl.h"

#include "_2RealBundle.h"

using namespace _2Real::bundle;
using namespace _2Real;

using std::string;

void getBundleMetainfo( BundleMetainfo &info )
{
	try
	{
		info.setName( "threadpool test" );
		info.setDescription( "bla bla bla" );
		info.setAuthor( "help@cadet.at" );
		info.setCategory( "testing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 0, 0 );

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

		BlockMetainfo &testBlock = info.exportBlock< Test, WithoutContext >( "TypeTestingBlock" );
		testBlock.setDescription( "block for custom type testing" );
		//customTypeBlock.addInlet< int >( "int inlet 0" );										// no default, no range
		//customTypeBlock.addInlet< int >( "int inlet 1", 10 );									// default, no range
		//customTypeBlock.addInlet< int >( "int inlet 2", 10, Range_t< int >( -2, 10 ) );			// default + range
		//customTypeBlock.addInlet( "int inlet 3", "int", customIntType );						// default val & range are stored in 'customIntType'
		testBlock.addInlet( "customInlet0", "testtype", initialValue );									// make the type explicit, to avoid confusion
		testBlock.addInlet( "customInlet1", "testtype", initialValue );					// range & values for all fiels are stored in the initialValue

		testBlock.addOutlet( "customOutlet0", "testtype" );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}