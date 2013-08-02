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

		info.setName( "--B--" );
		info.setCategory( "testing" );
		info.setVersion( 0, 0, 0 );

		TypeMetainfo allType = info.exportCustomType( "--B-- basetype" );
		allType.addField< char >( "--B-- char" );
		allType.addField< unsigned char >( "--B-- uchar" );
		allType.addField< int >( "--B-- int" );
		allType.addField< unsigned int >( "--B-- uint" );
		allType.addField< double >( "--B-- double" );
		allType.addField< float >( "--B-- float" );
		allType.addField< std::string >( "--B-- string" );
		allType.addField< bool >( "--B-- bool" );
		allType.addField< std::vector< int > >( "--B-- int vector" );
		allType.addField< std::vector< std::vector< int > > >( "--B-- int vector vector" );

		TypeMetainfo simpleType = info.exportCustomType( "--B-- simpletype" );
		simpleType.addField< int >( "--B-- int" );
		simpleType.addField< float >( "--B-- float" );

		TypeMetainfo complexType = info.exportCustomType( "--B-- complextype" );
		complexType.addField< int >( "--B-- int" );
		complexType.addField< std::string >( "--B-- string" );
		complexType.addField< std::vector< float > >( "--B-- float vector" );
		complexType.addCustomTypeField( "--B-- image", "image" );
		complexType.addCustomTypeField( "--B-- simpletype", "--B-- simpletype" );
		complexType.addCustomTypeField( "--B-- basetype", "--B-- basetype" );

		//TypeMetainfo &recursiveType = info.exportCustomType( "recursivetype" );
		//recursiveType.addCustomTypeField( "simpletype", "simpletype" );
		//recursiveType.addCustomTypeField( "recursivetype", "recursivetype" );

		std::shared_ptr< CustomType > allInit = allType.makeData();
		allInit->set< char >( "--B-- char", 100 );
		allInit->set< unsigned char >( "--B-- uchar", 100U );
		allInit->set< int >( "--B-- int", 100 );
		allInit->set< unsigned int >( "--B-- uint", 100U );
		allInit->set< double >( "--B-- double", 100.0 );
		allInit->set< float >( "--B-- float", 100.f );
		allInit->set< std::string >( "--B-- string", "100" );
		allInit->set< bool >( "--B-- bool", true );
		allInit->set< std::vector< int > >( "--B-- int vector", std::vector< int >( 100, 100 ) );
		allInit->set< std::vector< std::vector< int > > >( "--B-- int vector vector", std::vector< std::vector< int > >( 100, std::vector< int >( 100, 100 ) ) );

		std::shared_ptr< CustomType > simpleInit = simpleType.makeData();
		simpleInit->set< int >( "--B-- int", 555 );
		simpleInit->set< float >( "--B-- float", 555.f );

		std::shared_ptr< CustomType > complexInit = complexType.makeData();
		complexInit->set< int >( "--B-- int", 222 );
		complexInit->set< std::string >( "--B-- string", "222" );
		complexInit->set< std::vector< float > >( "--B-- float vector", std::vector< float >( 222, 222.f ) );
		complexInit->set< CustomType >( "--B-- image", *( imgInit.toCustomType().get() ) );
		complexInit->set< CustomType >( "--B-- simpletype", *( simpleInit.get() ) );
		complexInit->set< CustomType >( "--B-- basetype", *( allInit.get() ) );

		//std::shared_ptr< CustomType > recursiveInit( new CustomType( recursiveType ) );
		//recursiveInit->set< CustomType >( "simpletype", *( simpleInit.get() ) );
		//recursiveInit->set< CustomType >( "recursivetype", *( recursiveInit.get() ) );

		FunctionBlockMetainfo testBlock = info.exportFunctionBlock< Test, WithoutContext >( "TypeTestingBlock" );
		testBlock.setDescription( "type testing" );

		testBlock.addCustomTypeInlet( "i0", "--B-- basetype", allInit );
		testBlock.addCustomTypeInlet( "i1", "--B-- basetype", std::shared_ptr< CustomType >() ); // no initializer!
		testBlock.addCustomTypeInlet( "i2", "--B-- complextype", complexInit );
		testBlock.addCustomTypeInlet( "i3", "--B-- complextype", std::shared_ptr< CustomType >() );
		testBlock.addCustomTypeInlet( "i4", "--B-- simpletype", simpleInit );
		testBlock.addCustomTypeInlet( "i5", "--B-- simpletype", std::shared_ptr< CustomType >() );
		testBlock.addInlet< std::vector< int > >( "i6", std::vector< int >( 100, 100 ) );
		testBlock.addInlet< std::vector< int > >( "i7" );
		testBlock.addInlet< std::vector< std::vector< int > > >( "i8", std::vector< std::vector< int > >( 100, std::vector< int >( 100, 100 ) ) );
		testBlock.addInlet< std::vector< std::vector< int > > >( "i9" );

		testBlock.addCustomTypeOutlet( "o0", "--B-- basetype" );
		testBlock.addCustomTypeOutlet( "o1", "--B-- complextype" );
		testBlock.addCustomTypeOutlet( "o2", "--B-- simpletype" );
		testBlock.addOutlet< std::vector< int > >( "o3" );
		testBlock.addOutlet< std::vector< std::vector< int > > >( "o4" );

		testBlock.addCustomTypeParameter( "p0", "--B-- basetype", allInit );
		testBlock.addCustomTypeParameter( "p1", "--B-- basetype", std::shared_ptr< CustomType >() );
		testBlock.addCustomTypeParameter( "p2", "--B-- complextype", complexInit );
		testBlock.addCustomTypeParameter( "p3", "--B-- complextype", std::shared_ptr< CustomType >() );
		testBlock.addCustomTypeParameter( "p4", "--B-- simpletype", simpleInit );
		testBlock.addCustomTypeParameter( "p5", "--B-- simpletype", std::shared_ptr< CustomType >() );
		testBlock.addParameter< std::vector< int > >( "p6", std::vector< int >( 50, 50 )  );
		testBlock.addParameter< std::vector< int > >( "p7" );
		testBlock.addParameter< std::vector< std::vector< int > > >( "p8", std::vector< std::vector< int > >( 10, std::vector< int >( 10, 10 ) ) );
		testBlock.addParameter< std::vector< std::vector< int > > >( "p9" );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}