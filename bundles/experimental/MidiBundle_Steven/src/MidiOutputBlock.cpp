#include "MidiOutputBlock.h"

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

#include <vector>
#include <iostream>

MidiOutputBlock::MidiOutputBlock( ContextBlock & context ) :
	Block(),
	m_iMidiOutLastPort( -1 ),
	m_MidiOut( 0 )
{

}


MidiOutputBlock::~MidiOutputBlock(void)
{

}

void MidiOutputBlock::update()
{
	if ( m_MidiOut != 0 )
	{
		try
		{
			if ( m_MidiOut->getPortCount() == 0 )
			{
				m_iMidiOutLastPort = -1;
				return;
			}

			rescanMidiOutPorts();

			vector<unsigned char> midiMessage;
			midiMessage.push_back( m_MidiOutMessage0Inlet.getReadableRef<unsigned char>() );
			midiMessage.push_back( m_MidiOutMessage1Inlet.getReadableRef<unsigned char>() );
			midiMessage.push_back( m_MidiOutMessage2Inlet.getReadableRef<unsigned char>() );

			m_MidiOut->sendMessage( &midiMessage );
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
			initRtMidiOut();
		}
		catch ( RtError& error )
		{
			error.printMessage();
			delete m_MidiOut;
			m_MidiOut = 0;
		}
	}
}

void MidiOutputBlock::setup( BlockHandle &context )
{
	try
	{
		m_MidiOutPortInlet = context.getInletHandle( "midioutport" );
		m_MidiOutMessage0Inlet = context.getInletHandle( "midioutmessage0" );
		m_MidiOutMessage1Inlet = context.getInletHandle( "midioutmessage1" );
		m_MidiOutMessage2Inlet = context.getInletHandle( "midioutmessage2" );

		initRtMidiOut();
	}
	catch ( RtError& error )
	{
		error.printMessage();
		delete m_MidiOut;
		m_MidiOut = 0;
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void MidiOutputBlock::shutdown()
{
	if ( m_MidiOut != 0 )
		delete m_MidiOut;
}

void MidiOutputBlock::rescanMidiOutPorts()
{
	unsigned int portToOpen = m_MidiOutPortInlet.getReadableRef<unsigned int>();

	if ( m_iMidiOutLastPort != portToOpen )
	{
		m_MidiOut->closePort();
		m_iMidiOutLastPort = portToOpen;
		m_MidiOut->openPort( m_iMidiOutLastPort );
	}
}

void MidiOutputBlock::initRtMidiOut()
{
	m_MidiOut = new RtMidiOut();
}
