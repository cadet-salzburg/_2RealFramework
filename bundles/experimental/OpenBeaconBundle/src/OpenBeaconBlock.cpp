#include "_2RealDatatypes.h"

#include "OpenBeaconBlock.h"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;



OpenBeaconBlock::OpenBeaconBlock()
{}

OpenBeaconBlock::~OpenBeaconBlock()
{}

void OpenBeaconBlock::setup( BlockHandle &block )
{
	try
	{
		std::cout << "SETUP openbeacon block" << std::endl;

		m_Block = block;

		m_dummyIn = m_Block.getInletHandle( "dummy_in" );
		m_dummyOut = m_Block.getOutletHandle( "dummy_out" );

		std::cout << "initial value: " << m_dummyIn.getReadableRef<int>() << std::endl;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void OpenBeaconBlock::update()
{
	try
	{
		std::cout << "UPDATE openbeacon block" << std::endl;

		if( m_dummyIn.hasChanged() )
		{
			int dummy = m_dummyIn.getReadableRef<int>();

			std::cout << "setting outlet to " << dummy << std::endl;

			m_dummyOut.getWriteableRef<int>() = dummy;
		}
		else
			m_dummyOut.discard();
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void OpenBeaconBlock::shutdown()
{
	std::cout << "SHUTDOWN openbeacon block" << std::endl;
}