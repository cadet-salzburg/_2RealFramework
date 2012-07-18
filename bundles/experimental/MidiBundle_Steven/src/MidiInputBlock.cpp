#include "MidiInputBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

MidiInputBlock::MidiInputBlock( ContextBlock & context ) :
	Block(),
	m_iMidiInLastPort( -1 ),
	m_MidiIn( 0 )
{

}

MidiInputBlock::~MidiInputBlock()
{

}

void MidiInputBlock::update()
{
	if ( m_MidiIn != 0 )
	{
		try
		{
			if ( m_MidiIn->getPortCount() == 0 )
			{
				m_iMidiInLastPort = -1;
				return;
			}

			rescanMidiInPorts();

			vector<unsigned char> midiMessage;
			m_MidiIn->getMessage( &midiMessage );

			if ( !midiMessage.empty() && midiMessage.size() >= 3 )
			{
				m_MidiInMessage0Outlet.getWriteableRef<unsigned char>() = midiMessage[0];
				m_MidiInMessage1Outlet.getWriteableRef<unsigned char>() = midiMessage[1];
				m_MidiInMessage2Outlet.getWriteableRef<unsigned char>() = midiMessage[2];

				for ( unsigned int i = 0; i < midiMessage.size(); i++ )
				{
					cout << "Byte " << i << " = " << (int)midiMessage[i] << ", ";
				}
				cout << endl;
			}
		}
		catch ( RtError& error )
		{
			error.printMessage();
		}
		catch ( Exception& e )
		{
			cout << e.message() << endl;
			e.rethrow();
		}
	}
	else
	{
		try
		{
			initRtMidiIn();
		}
		catch ( RtError& error )
		{
			error.printMessage();
			delete m_MidiIn;
			m_MidiIn = 0;
		}
	}
}

void MidiInputBlock::setup( BlockHandle &context )
{
	try
	{
		m_MidiInPortInlet = context.getInletHandle( "midiinport" );
		m_MidiInMessage0Outlet = context.getOutletHandle( "midiinmessage0" );
		m_MidiInMessage1Outlet = context.getOutletHandle( "midiinmessage1" );
		m_MidiInMessage2Outlet = context.getOutletHandle( "midiinmessage2" );

		initRtMidiIn();
	}
	catch ( RtError& error )
	{
		error.printMessage();
		delete m_MidiIn;
		m_MidiIn = 0;
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void MidiInputBlock::shutdown()
{
	if ( m_MidiIn != 0 )
		delete m_MidiIn;
}

void MidiInputBlock::rescanMidiInPorts()
{
	unsigned int portToOpen = m_MidiInPortInlet.getReadableRef<unsigned int>();

	if ( m_iMidiInLastPort != portToOpen )
	{
		m_MidiIn->closePort();
		m_iMidiInLastPort = portToOpen;
		m_MidiIn->openPort( m_iMidiInLastPort );
		m_MidiIn->ignoreTypes( false, false, false );
	}
}

void MidiInputBlock::initRtMidiIn()
{
	m_MidiIn = new RtMidiIn();
}
