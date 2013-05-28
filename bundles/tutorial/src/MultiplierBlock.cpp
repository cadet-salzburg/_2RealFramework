#include <iostream>
#include <string>
#include <sstream>
#include "MultiplierBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

MultiplierBlock::MultiplierBlock( )
	:Block()
{
}

MultiplierBlock::~MultiplierBlock()
{
}

void MultiplierBlock::setup( BlockHandle &block )
{
	//try
	//{
	//	// inlet handles
	//	m_NumberAInletHandle = block.getInletHandle("NumberA");
	//	m_NumberBInletHandle = block.getInletHandle("NumberB");
	//
	//	// outlet handles
	//	m_ResultOutletHandle = block.getOutletHandle("Result" );

	//	// set default for output, when block isn't started yet and no update had happend
	//	m_ResultOutletHandle.getWriteableRef<int>() = 0;
	//}
	//catch ( Exception &e )
	//{
	//	cout << e.message() << endl;
	//	e.rethrow();
	//}
}

void MultiplierBlock::update()
{
	//try
	//{
	//	// multiply the values of two integer inlets and write it to an integer outlet
	//	m_ResultOutletHandle.getWriteableRef<int>() = m_NumberAInletHandle.getReadableRef<int>() * m_NumberBInletHandle.getReadableRef<int>();
	//}
	//catch ( Exception &e )
	//{
	//	cout << e.message() << endl;
	//	e.rethrow();
	//}
}

void MultiplierBlock::shutdown()
{
	// custom cleanup not needed for this simple block
}

