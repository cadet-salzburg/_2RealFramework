#include "ServiceImpl.h"
#include "_2RealDatatypes.h"

#include "bundle/_2RealBlockHandle.h"
#include "datatypes/_2RealCustomData.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace _2Real::bundle;
using _2Real::Exception;
using _2Real::CustomType;
using _2Real::Image;

using namespace std;

Test::Test() : Block(), mCounter( 0 )
{
}

void Test::setup( BlockHandle &handle )
{
	try
	{
		mInstance.block = handle;
		handle.getAllInletHandles( mInstance.inlets );
		handle.getAllOutletHandles( mInstance.outlets );
		handle.getAllParameterHandles( mInstance.parameters );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void Test::update()
{
	try
	{
		for ( unsigned int i=0; i<mInstance.inlets.size(); ++i )
		{
			std::shared_ptr< const CustomType > data = mInstance.inlets[ i ].getReadableRef();
			if ( !data.get() ) std::cout << "DATA FOR INLET " << i<< " IS MISSING!" << std::endl;
		}

		for ( unsigned int i=0; i<mInstance.parameters.size(); ++i )
		{
			std::shared_ptr< const CustomType > data = mInstance.parameters[ i ].getReadableRef();
			if ( !data.get() ) std::cout << "DATA FOR PARAMETER " << i<< " IS MISSING!" << std::endl;
		}

		//InletHandle i2 = mInstance.inlets[ 2 ];
		//{
		//	std::shared_ptr< const CustomType > d = i2.getReadableRef();
		//	std::cout << "test int:\t\t\t" << ( int ) *( d->get< int >( "test int" ).get() ) << std::endl;
		//	std::cout << "test string:\t\t\t" << *( d->get< std::string >( "test string" ).get() ) << std::endl;
		//	std::vector< float > const& vf = *( d->get< std::vector< float > >( "test float vector" ).get() );
		//	std::cout << "test float vector:\t\t" << "size " << vf.size() << std::endl;
		//	std::shared_ptr< const CustomType > i = d->get< CustomType >( "test image" );
		//	std::shared_ptr< const Image > img = Image::asImage( i );
		//	std::cout << "test image:" << std::endl;
		//	std::cout << "\twidth:\t\t\t" << img->getWidth() << std::endl;
		//	std::cout << "\theight:\t\t\t" << img->getHeight() << std::endl;
		//	std::cout << "\tchannel order:\t\t" << ( std::string ) img->getChannelOrder() << std::endl;
		//	std::cout << "\tformat:\t\t\t" << ( std::string ) img->getDatatype() << std::endl;
		//	std::shared_ptr< const CustomType > b = d->get< CustomType >( "test basetype" );
		//	std::cout << "test basetype:" << std::endl;
		//	std::cout << "\tchar:\t\t\t" << ( int ) *( b->get< char >( "char" ).get() ) << std::endl;
		//	std::cout << "\tuchar:\t\t\t" << ( int ) *( b->get< unsigned char >( "uchar" ).get() ) << std::endl;
		//	std::cout << "\tint:\t\t\t" << ( int ) *( b->get< int >( "int" ).get() ) << std::endl;
		//	std::cout << "\tuint:\t\t\t" << ( int ) *( b->get< unsigned int >( "uint" ).get() ) << std::endl;
		//	std::cout << "\tfloat:\t\t\t" << *( b->get< float >( "float" ).get() ) << std::endl;
		//	std::cout << "\tdouble:\t\t\t" << *( b->get< double >( "double" ).get() ) << std::endl;
		//	std::cout << "\tstring:\t\t\t" << *( b->get< std::string >( "string" ).get() ) << std::endl;
		//	std::cout << "\tbool:\t\t\t" << std::boolalpha << *( b->get< bool >( "bool" ).get() ) << std::endl;
		//	std::vector< int > const& vi = *( b->get< std::vector< int > >( "int vector" ).get() );
		//	std::cout << "\tint vector:\t\t" << "size " << vi.size() << std::endl;
		//	std::vector< std::vector< int > > const& vvi = *( b->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
		//	std::cout << "\tint vector vector:\t" << "size " << vvi.size() << std::endl;
		//	std::shared_ptr< const CustomType > s = d->get< CustomType >( "test simpletype" );
		//	std::cout << "test simpletype:" << std::endl;
		//	std::cout << "\tint:\t\t\t" << ( int ) *( s->get< int >( "int" ).get() ) << std::endl;
		//	std::cout << "\tfloat:\t\t\t" << ( int ) *( s->get< float >( "float" ).get() ) << std::endl;
		//}

		{
			// basetype
			OutletHandle o = mInstance.outlets[ 0 ];
			std::shared_ptr< CustomType > data = o.getWriteableRef();
			data->set< int >( "--B-- int", ++mCounter );
			data->set< std::vector< std::vector< int > > >( "--B-- int vector vector", std::vector< std::vector< int > >( 10, std::vector< int >( 5, 5 ) ) );
		}

		{
			// complextype
			OutletHandle o = mInstance.outlets[ 1 ];
			std::shared_ptr< CustomType > data = o.getWriteableRef();
			std::shared_ptr< Image > img = Image::asImage( data->get< CustomType >( "--B-- image" ) );
			img->createImagedata( 10, 10, Image::ChannelOrder::RGB, Image::Datatype::UINT8 );
		}

		{
			// simpletype
			OutletHandle o = mInstance.outlets[ 2 ];
			std::shared_ptr< CustomType > data = o.getWriteableRef();
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void Test::shutdown()
{
	try
	{
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

Test::~Test()
{
}