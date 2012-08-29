#include "MovingHeadBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

MovingHeadBlock::MovingHeadBlock( ContextBlock & context ) 
: Block()
{

}

MovingHeadBlock::~MovingHeadBlock()
{
}

void MovingHeadBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed In and Outlets
		m_MotorIDInlet = context.getInletHandle( "MotorID" );
		m_CommandInlet = context.getInletHandle( "Command" );
		m_ValueIntel = context.getInletHandle( "Value" );
		m_SerialOutlet = context.getOutletHandle( "SerialByteStream" );
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void MovingHeadBlock::shutdown()
{
}

void MovingHeadBlock::discardAllOutlets()
{
	m_SerialOutlet.discard();
}

void MovingHeadBlock::update()
{
	try
	{
		// there is only something happening when a new value is provided!
		if (m_ValueIntel.hasChanged())
		{
			// make a proper command

			// Command Syntax
			// Byte
			// 0 1 2         3         4          5          6          7
			// X X ['0'|'1'|'2'] ['S'|'P'|'B'] ['0'..'9'] ['0'..'9'] ['0'..'9'] ['0'..'9']
			//
			// Byte 0 and 1 are snyc bytes
			// Byte 2 is the id of the motor resp. LED
			// Byte 3 is the command (either Speed or Position or Brightness (LED))
			// Byte 4-7 is the parameter value as ascii string for the command. needs to be a number (like '0' '1' '0' '0' for 100)
			// for Position the range is 0 (0 degree) to 1023 (300 degrees)
			// for Speed the range is 1 to 1023 (114 rpm). 0 means maximim speed
			// for LED Brightness the range is 0 (0V) to 1023 (5V)

			std::vector<unsigned char> byteStream;
			byteStream.push_back('X');
			byteStream.push_back('X');
			byteStream.push_back('0' + m_MotorIDInlet.getReadableRef<unsigned int>());
			byteStream.push_back(m_CommandInlet.getReadableRef<unsigned char>());
			unsigned int value = m_ValueIntel.getReadableRef<unsigned int>();
			byteStream.push_back('0' + (value % 10000) / 1000);
			byteStream.push_back('0' + (value % 1000) / 100);
			byteStream.push_back('0' + (value % 100) / 10);
			byteStream.push_back('0' + (value % 10));

			m_SerialOutlet.getWriteableRef<vector<unsigned char>>() = byteStream;
		}
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
