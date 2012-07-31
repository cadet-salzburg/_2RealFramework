#include <iostream>
#include <string>
#include <sstream>
#include "ThroughputBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

ThroughputBlock::ThroughputBlock( )
	:Block()
{
}

ThroughputBlock::~ThroughputBlock()
{
}

void ThroughputBlock::setup( BlockHandle &block )
{
	try
	{
		m_pBlockHandle = block;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void ThroughputBlock::update()
{
	try
	{
		// throughput inlets to 
		m_pBlockHandle.getOutletHandle("number").getWriteableRef<_2Real::Number>() = m_pBlockHandle.getInletHandle("number").getReadableRef<_2Real::Number>();
		m_pBlockHandle.getOutletHandle("bool").getWriteableRef<bool>() = m_pBlockHandle.getInletHandle("bool").getReadableRef<bool>();
		m_pBlockHandle.getOutletHandle("uchar").getWriteableRef<unsigned char>() = m_pBlockHandle.getInletHandle("uchar").getReadableRef<unsigned char>();
		m_pBlockHandle.getOutletHandle("char").getWriteableRef<char>() = m_pBlockHandle.getInletHandle("char").getReadableRef<char>();
		m_pBlockHandle.getOutletHandle("ushort").getWriteableRef<unsigned short>() = m_pBlockHandle.getInletHandle("ushort").getReadableRef<unsigned short>();
		m_pBlockHandle.getOutletHandle("short").getWriteableRef<short>() = m_pBlockHandle.getInletHandle("short").getReadableRef<short>();
		m_pBlockHandle.getOutletHandle("uint").getWriteableRef<unsigned int>() = m_pBlockHandle.getInletHandle("uint").getReadableRef<unsigned int>();
		m_pBlockHandle.getOutletHandle("int").getWriteableRef<int>() = m_pBlockHandle.getInletHandle("int").getReadableRef<int>();
		m_pBlockHandle.getOutletHandle("ulong").getWriteableRef<unsigned long>() = m_pBlockHandle.getInletHandle("ulong").getReadableRef<unsigned long>();
		m_pBlockHandle.getOutletHandle("long").getWriteableRef<long>() = m_pBlockHandle.getInletHandle("long").getReadableRef<long>();
		m_pBlockHandle.getOutletHandle("float").getWriteableRef<float>() = m_pBlockHandle.getInletHandle("float").getReadableRef<float>();
		m_pBlockHandle.getOutletHandle("double").getWriteableRef<double>() = m_pBlockHandle.getInletHandle("double").getReadableRef<double>();
		m_pBlockHandle.getOutletHandle("point").getWriteableRef<_2Real::Point>() = m_pBlockHandle.getInletHandle("point").getReadableRef<_2Real::Point>();
		m_pBlockHandle.getOutletHandle("string").getWriteableRef<string>() = m_pBlockHandle.getInletHandle("string").getReadableRef<string>();
		m_pBlockHandle.getOutletHandle("vector").getWriteableRef<std::vector<unsigned char>>() = m_pBlockHandle.getInletHandle("vector").getReadableRef<std::vector<unsigned char>>();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void ThroughputBlock::shutdown()
{
	// custom cleanup not needed for this simple block
}

