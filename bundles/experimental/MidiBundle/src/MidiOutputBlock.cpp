#include "MidiOutputBlock.h"

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

#include <vector>
#include <iostream>

MidiOutputBlock::MidiOutputBlock( ContextBlock & context ) :
	Block(),
	m_iMidiOutCurrentPort( -1 ),
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
			// If there are no Ports available set the current Port index back to -1 and jump out of the method
			if ( m_MidiOut->getPortCount() == 0 )
			{
				m_iMidiOutCurrentPort = -1;
				return;
			}

			// Check if the current Port index has changed since the last update call and open the new Port accordingly
			rescanMidiOutPorts();

			// Put the received messages in a vector and send them via the RtMidiOut instance
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
	else // Try initializing the RtMidiOut Instance if something should have gone wrong during the setup phase
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
		// Get the handles to all needed Inlets
		m_MidiOutPortInlet = context.getInletHandle( "midioutport" );
		m_MidiOutMessage0Inlet = context.getInletHandle( "midioutmessage0" );
		m_MidiOutMessage1Inlet = context.getInletHandle( "midioutmessage1" );
		m_MidiOutMessage2Inlet = context.getInletHandle( "midioutmessage2" );

		// Initialize the RtMidiOut instance
		initRtMidiOut();
	}
	catch ( RtError& error )
	{
		error.printMessage();
		// Delete the RtMidiOut instance if something went wrong while allocating memory
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
	// Ask the Inlet for the Port that should be opened / used
	unsigned int portToOpen = m_MidiOutPortInlet.getReadableRef<unsigned int>();

	// If the Port index in the Inlet is different from the currently used Port index
	// close the old Port and open a new Port according to the new Port index
	if ( m_iMidiOutCurrentPort != portToOpen )
	{
		m_MidiOut->closePort();
		m_iMidiOutCurrentPort = portToOpen;
		m_MidiOut->openPort( m_iMidiOutCurrentPort );
	}
}

void MidiOutputBlock::initRtMidiOut()
{
	m_MidiOut = new RtMidiOut();
}
