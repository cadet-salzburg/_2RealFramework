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

		info.setName( "--A--" );
		info.setCategory( "testing" );
		info.setVersion( 0, 0, 0 );

		TypeMetainfo allType = info.exportCustomType( "--A-- basetype" );
		allType.addField< char >( "--A-- char" );
		allType.addField< unsigned char >( "--A-- uchar" );
		allType.addField< int >( "--A-- int" );
		allType.addField< unsigned int >( "--A-- uint" );
		allType.addField< double >( "--A-- double" );
		allType.addField< float >( "--A-- float" );
		allType.addField< std::string >( "--A-- string" );
		allType.addField< bool >( "--A-- bool" );
		allType.addField< std::vector< int > >( "--A-- int vector" );
		allType.addField< std::vector< std::vector< int > > >( "--A-- int vector vector" );

		TypeMetainfo simpleType = info.exportCustomType( "--A-- simpletype" );
		simpleType.addField< int >( "--A-- int" );
		simpleType.addField< float >( "--A-- float" );

		TypeMetainfo complexType = info.exportCustomType( "--A-- complextype" );
		complexType.addField< int >( "--A-- int" );
		complexType.addField< std::string >( "--A-- string" );
		complexType.addField< std::vector< float > >( "--A-- float vector" );
		complexType.addCustomTypeField( "--A-- image", "image" );
		complexType.addCustomTypeField( "--A-- simpletype", "--A-- simpletype" );
		complexType.addCustomTypeField( "--A-- basetype", "--A-- basetype" );

		std::shared_ptr< CustomType > allInit = allType.makeData();
		allInit->set< char >( "--A-- char", 100 );
		allInit->set< unsigned char >( "--A-- uchar", 100U );
		allInit->set< int >( "--A-- int", 100 );
		allInit->set< unsigned int >( "--A-- uint", 100U );
		allInit->set< double >( "--A-- double", 100.0 );
		allInit->set< float >( "--A-- float", 100.f );
		allInit->set< std::string >( "--A-- string", "100" );
		allInit->set< bool >( "--A-- bool", true );
		allInit->set< std::vector< int > >( "--A-- int vector", std::vector< int >( 100, 100 ) );
		allInit->set< std::vector< std::vector< int > > >( "--A-- int vector vector", std::vector< std::vector< int > >( 100, std::vector< int >( 100, 100 ) ) );

		std::shared_ptr< CustomType > simpleInit = simpleType.makeData();
		simpleInit->set< int >( "--A-- int", 555 );
		simpleInit->set< float >( "--A-- float", 555.f );

		std::shared_ptr< CustomType > complexInit = complexType.makeData();
		complexInit->set< int >( "--A-- int", 222 );
		complexInit->set< std::string >( "--A-- string", "222" );
		complexInit->set< std::vector< float > >( "--A-- float vector", std::vector< float >( 222, 222.f ) );
		complexInit->set< CustomType >( "--A-- image", *( imgInit.toCustomType().get() ) );
		complexInit->set< CustomType >( "--A-- simpletype", *( simpleInit.get() ) );
		complexInit->set< CustomType >( "--A-- basetype", *( allInit.get() ) );

		FunctionBlockMetainfo testBlock = info.exportFunctionBlock< Test, WithoutContext >( "TypeTestingBlock" );
		testBlock.setDescription( "type testing" );
		testBlock.addCustomTypeInlet( "i0", "--A-- basetype", allInit );
		testBlock.addCustomTypeInlet( "i1", "--A-- basetype", std::shared_ptr< CustomType >() );
		testBlock.addCustomTypeInlet( "i2", "--A-- complextype", complexInit );
		testBlock.addCustomTypeInlet( "i3", "--A-- complextype", std::shared_ptr< CustomType >() );
		testBlock.addCustomTypeInlet( "i4", "--A-- simpletype", simpleInit );
		testBlock.addCustomTypeInlet( "i5", "--A-- simpletype", std::shared_ptr< CustomType >() );
		testBlock.addInlet< std::vector< int > >( "i6", std::vector< int >( 100, 100 ) );
		testBlock.addInlet< std::vector< int > >( "i7" );
		testBlock.addInlet< std::vector< std::vector< int > > >( "i8", std::vector< std::vector< int > >( 100, std::vector< int >( 100, 100 ) ) );
		testBlock.addInlet< std::vector< std::vector< int > > >( "i9" );

		testBlock.addCustomTypeOutlet( "o0", "--A-- basetype" );
		testBlock.addCustomTypeOutlet( "o1", "--A-- complextype" );
		testBlock.addCustomTypeOutlet( "o2", "--A-- simpletype" );
		testBlock.addOutlet< std::vector< int > >( "o3" );
		testBlock.addOutlet< std::vector< std::vector< int > > >( "o4" );

		testBlock.addCustomTypeParameter( "p0", "--A-- basetype", allInit );
		testBlock.addCustomTypeParameter( "p1", "--A-- basetype", std::shared_ptr< CustomType >() );
		testBlock.addCustomTypeParameter( "p2", "--A-- complextype", complexInit );
		testBlock.addCustomTypeParameter( "p3", "--A-- complextype", std::shared_ptr< CustomType >() );
		testBlock.addCustomTypeParameter( "p4", "--A-- simpletype", simpleInit );
		testBlock.addCustomTypeParameter( "p5", "--A-- simpletype", std::shared_ptr< CustomType >() );
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