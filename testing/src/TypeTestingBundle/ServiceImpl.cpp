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

unsigned int creationCount = 0;

Test::Test() : Block(), mCounter( 0 ), mNumber( ++ creationCount )
{
	cout << "CTOR" << endl;
}

void Test::setup( BlockHandle &handle )
{
	try
	{
		cout << "SETUP " <<  mNumber << endl;
		mHandle = handle;
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
		InletHandle in0 = mHandle.getInletHandle( "customInlet0" );
		InletHandle in1 = mHandle.getInletHandle( "customInlet1" );
		std::shared_ptr< const CustomType > i0 = in0.getReadableRef();
		std::shared_ptr< const CustomType > i1 = in1.getReadableRef();

		if ( !i0.get() ) std::cout << "ARGH " << mNumber << std::endl;
		if ( !i1.get() ) std::cout << "ARGH " << mNumber << std::endl;
		else
		{
			std::shared_ptr< const Image > img0 = Image::asImage( i0->get< CustomType >( "test image" ) );
			//std::cout << img0->getWidth() << " " << img0->getHeight() << std::endl;
		}


		std::shared_ptr< const int > i = i0->get< int >( "test int" );
		int num = *( i.get() ) + ( ++mCounter );

		OutletHandle out0 = mHandle.getOutletHandle( "customOutlet0" );
		std::shared_ptr< CustomType > o = out0.getWriteableRef();
		o->set< int >( "test int", num );
		std::shared_ptr< Image > outImg = Image::asImage( o->get< CustomType >( "test image" ) );
		outImg->createImagedata( 10, 10, Image::ChannelOrder::RGB, Image::Datatype::UINT8 );
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
		cout << "SHUTDOWN " << mNumber << endl;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

Test::~Test()
{
	cout << "DTOR " << mNumber << endl;
}