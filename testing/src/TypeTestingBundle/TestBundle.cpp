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
		testType.addCustomTypeField( "test image", "image" );

		std::shared_ptr< CustomType > initialValue( new CustomType( testType ) );
		initialValue->set< int >( "test int", 0 );
		initialValue->set< std::string >( "test string", "NARF" );
		initialValue->set< std::vector< float > >( "test float array", std::vector< float >( 10, 0.5f ) );
		Image image;
		std::vector< unsigned char > init( 16, 0 );
		image.setImagedata( &init[ 0 ], 2, 2, Image::ChannelOrder::RGBA, Image::Datatype::UINT8 );
		initialValue->set< CustomType >( "test image", *( image.toCustomType().get() ) );

		BlockMetainfo &testBlock = info.exportBlock< Test, WithoutContext >( "TypeTestingBlock" );
		testBlock.setDescription( "block for custom type testing" );
		testBlock.addCustomTypeInlet( "customInlet0", "testtype", initialValue );
		testBlock.addCustomTypeInlet( "customInlet1", "testtype", initialValue );
		testBlock.addOutlet( "customOutlet0", "testtype" );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}