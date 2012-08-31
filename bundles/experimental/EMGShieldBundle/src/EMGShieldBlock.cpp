#include "EMGShieldBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

EMGShieldBlock::EMGShieldBlock( ContextBlock & context ) 
: Block()
{

}

EMGShieldBlock::~EMGShieldBlock()
{
}

void EMGShieldBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed In and Outlets
		m_Channel0Value = context.getOutletHandle("Channel0Value");
		m_Channel1Value = context.getOutletHandle("Channel1Value");
		m_SerialByteStream = context.getInletHandle("SerialByteStream");
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EMGShieldBlock::shutdown()
{
}

void EMGShieldBlock::discardAllOutlets()
{
	m_Channel0Value.discard();
	m_Channel1Value.discard();
}

void EMGShieldBlock::update()
{
	try
	{
		// there is only something happening when a new value is provided!
		if (m_SerialByteStream.hasChanged())
		{
			// see EMG_raw_protocol.png for protocol description

			vector<unsigned char> data = m_SerialByteStream.getReadableRef<vector<unsigned char>>();
			for (vector<unsigned char>::iterator it = data.begin(); it != data.end(); it++)
			{
				unsigned char val = *it;
				int channel = (val & 0x80) >> 7;
				int value = val & 0x1F;

				switch (channel)
				{
				case 1:
					m_Channel0Value.getWriteableRef<unsigned int>() = value;
					break;
				case 2:
					m_Channel1Value.getWriteableRef<unsigned int>() = value;
					break;
				default:
					cout << "EMG Bundle: unkown channel " << channel << "!" << endl;
					break;
				}
			}
		}
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
