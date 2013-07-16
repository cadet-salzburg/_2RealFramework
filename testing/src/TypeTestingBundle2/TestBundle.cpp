#include "ServiceImpl.h"

#include "_2RealBundle.h"

using namespace _2Real::bundle;
using namespace _2Real;

using std::string;

void getBundleMetainfo( BundleMetainfo &info )
{
	try
	{
		Image imgInit;
		unsigned int imgW = 16;
		unsigned int imgH = 16;
		unsigned int imgC = 4;
		std::vector< unsigned char > init( imgW*imgH*imgC, 0 );
		imgInit.setImagedata( &init[ 0 ], imgW, imgH, Image::ChannelOrder::RGBA, Image::Datatype::UINT8 );

		info.setName( "datatypes test" );
		info.setDescription( "bla bla bla" );
		info.setAuthor( "help@cadet.at" );
		info.setCategory( "testing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 0, 0 );
		//info.finalizeBundleInfo();

		// TODO: initial value & range for all fields

		// base types only
		TypeMetainfo &allType = info.exportCustomType( "basetype" );
		allType.addField< char >( "char" );
		allType.addField< unsigned char >( "uchar" );
		allType.addField< int >( "int" );
		allType.addField< unsigned int >( "uint" );
		allType.addField< double >( "double" );
		allType.addField< float >( "float" );
		allType.addField< std::string >( "string" );
		allType.addField< bool >( "bool" );
		allType.addField< std::vector< int > >( "int vector" );
		allType.addField< std::vector< std::vector< int > > >( "int vector vector" );

		// simple custom type
		TypeMetainfo &simpleType = info.exportCustomType( "simpletype" );
		simpleType.addField< int >( "int" );
		simpleType.addField< float >( "float" );

		// base fields, framework defined fields, user defined fields
		TypeMetainfo &complexType = info.exportCustomType( "complextype" );
		complexType.addField< int >( "int" );
		complexType.addField< std::string >( "string" );
		complexType.addField< std::vector< float > >( "float vector" );
		complexType.addCustomTypeField( "image", "image" );
		complexType.addCustomTypeField( "simpletype", "simpletype" );
		complexType.addCustomTypeField( "basetype", "basetype" );

		// oooooh, a type that contains itsself
		//TypeMetainfo &recursiveType = info.exportCustomType( "recursivetype" );
		//recursiveType.addCustomTypeField( "simpletype", "simpletype" );
		//recursiveType.addCustomTypeField( "recursivetype", "recursivetype" );

		std::shared_ptr< CustomType > allInit( new CustomType( allType ) );
		allInit->set< char >( "char", 100 );
		allInit->set< unsigned char >( "uchar", 100U );
		allInit->set< int >( "int", 100 );
		allInit->set< unsigned int >( "uint", 100U );
		allInit->set< double >( "double", 100.0 );
		allInit->set< float >( "float", 100.f );
		allInit->set< std::string >( "string", "100" );
		allInit->set< bool >( "bool", true );
		allInit->set< std::vector< int > >( "int vector", std::vector< int >( 100, 100 ) );
		allInit->set< std::vector< std::vector< int > > >( "int vector vector", std::vector< std::vector< int > >( 100, std::vector< int >( 100, 100 ) ) );

		std::shared_ptr< CustomType > simpleInit( new CustomType( simpleType ) );
		simpleInit->set< int >( "int", 555 );
		simpleInit->set< float >( "float", 555.f );

		std::shared_ptr< CustomType > complexInit( new CustomType( complexType ) );
		complexInit->set< int >( "int", 222 );
		complexInit->set< std::string >( "string", "222" );
		complexInit->set< std::vector< float > >( "float vector", std::vector< float >( 222, 222.f ) );
		complexInit->set< CustomType >( "image", *( imgInit.toCustomType().get() ) );
		complexInit->set< CustomType >( "simpletype", *( simpleInit.get() ) );
		complexInit->set< CustomType >( "basetype", *( allInit.get() ) );

		//std::shared_ptr< CustomType > recursiveInit( new CustomType( recursiveType ) );
		//recursiveInit->set< CustomType >( "simpletype", *( simpleInit.get() ) );
		//recursiveInit->set< CustomType >( "recursivetype", *( recursiveInit.get() ) );

		BlockMetainfo &testBlock = info.exportBlock< Test, WithoutContext >( "TypeTestingBlock" );
		testBlock.setDescription( "block for custom type testing" );
		testBlock.addCustomTypeInlet( "i0", "basetype", allInit );
		testBlock.addCustomTypeInlet( "i1", "basetype", std::shared_ptr< CustomType >() ); // no initializer!
		testBlock.addCustomTypeInlet( "i2", "complextype", complexInit );
		testBlock.addCustomTypeInlet( "i3", "complextype", std::shared_ptr< CustomType >() );
		testBlock.addCustomTypeInlet( "i4", "simpletype", simpleInit );
		testBlock.addCustomTypeInlet( "i5", "simpletype", std::shared_ptr< CustomType >() );
		testBlock.addInlet< std::vector< int > >( "i6", std::vector< int >( 100, 100 ) );
		testBlock.addInlet< std::vector< int > >( "i7" );
		testBlock.addInlet< std::vector< std::vector< int > > >( "i8", std::vector< std::vector< int > >( 100, std::vector< int >( 100, 100 ) ) );
		testBlock.addInlet< std::vector< std::vector< int > > >( "i9" );
		//testBlock.addCustomTypeInlet( "i8", "recursivetype", recursiveInit );
		//testBlock.addCustomTypeInlet( "i9", "recursivetype", std::shared_ptr< CustomType >() );
		testBlock.addCustomTypeOutlet( "o0", "basetype" );
		testBlock.addCustomTypeOutlet( "o1", "complextype" );
		testBlock.addCustomTypeOutlet( "o2", "simpletype" );
		testBlock.addOutlet< std::vector< int > >( "o3" );
		testBlock.addOutlet< std::vector< std::vector< int > > >( "o4" );
		/*testBlock.addCustomTypeOutlet( "o5", "recursivetype" );*/
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}