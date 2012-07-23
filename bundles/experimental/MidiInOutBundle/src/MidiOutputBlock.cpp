#include "MidiOutputBlock.h"

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

#include <vector>
#include <iostream>

MidiOutputBlock::MidiOutputBlock( ContextBlock & context ) :
	Block(),
	m_iMidiOutCurrentPort( -1 )
{
	m_MidiInOutDeviceManager = static_cast<MidiInOutDeviceManager*>( &context );
	m_MidiInOutDeviceManager->registerMidiOutBlock();
}


MidiOutputBlock::~MidiOutputBlock(void)
{

}

void MidiOutputBlock::update()
{
	try
	{
		if ( m_MidiInOutDeviceManager->getMidiOutPortCount() == 0 )
		{
			m_iMidiOutCurrentPort = -1;
			return;
		}

		unsigned int portIndex = m_MidiOutPortInlet.getReadableRef<unsigned int>();

		if ( portIndex != m_iMidiOutCurrentPort )
		{
			if ( m_iMidiOutCurrentPort >= 0 )
			{
				m_MidiInOutDeviceManager->unsbindMidiOutDevice( m_iMidiOutCurrentPort );
				m_iMidiOutCurrentPort = -1;
			}

			if ( m_MidiInOutDeviceManager->bindMidiOutDevice( portIndex ) )
				m_iMidiOutCurrentPort = portIndex;
		}

		if ( m_MidiInOutDeviceManager->isMidiOutDeviceRunning( m_iMidiOutCurrentPort ) )
		{
			// Put the received messages in a vector and send them via the RtMidiOut instance
			vector<unsigned char> midiMessage;
			midiMessage.push_back( m_MidiOutMessage0Inlet.getReadableRef<unsigned char>() );
			midiMessage.push_back( m_MidiOutMessage1Inlet.getReadableRef<unsigned char>() );
			midiMessage.push_back( m_MidiOutMessage2Inlet.getReadableRef<unsigned char>() );

			m_MidiInOutDeviceManager->sendMidiOutMessage( m_iMidiOutCurrentPort, midiMessage );
		}
		else
			m_iMidiOutCurrentPort = -1;
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void MidiOutputBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed Inlets
		m_MidiOutPortInlet = context.getInletHandle( "MidiOutPort" );
		m_MidiOutMessage0Inlet = context.getInletHandle( "MidiOutMessage0" );
		m_MidiOutMessage1Inlet = context.getInletHandle( "MidioutMessage1" );
		m_MidiOutMessage2Inlet = context.getInletHandle( "MidioutMessage2" );
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void MidiOutputBlock::shutdown()
{
	m_MidiInOutDeviceManager->unsbindMidiOutDevice( m_iMidiOutCurrentPort );
	m_MidiInOutDeviceManager->unregisterMidiOutBlock();
}
