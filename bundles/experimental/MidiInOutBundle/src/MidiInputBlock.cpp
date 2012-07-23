#include "MidiInputBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

MidiInputBlock::MidiInputBlock( ContextBlock & context ) :
	Block(),
	m_iMidiInCurrentPort( -1 )
{
	m_MidiInOutDeviceManager = static_cast<MidiInOutDeviceManager*>( &context );
	m_MidiInOutDeviceManager->registerMidiInBlock();
}

MidiInputBlock::~MidiInputBlock()
{

}

void MidiInputBlock::update()
{
	try
	{
		if ( m_MidiInOutDeviceManager->getMidiInPortCount() == 0 )
		{
			m_iMidiInCurrentPort = -1;
			return;
		}

		unsigned int portIndex = m_MidiInPortInlet.getReadableRef<unsigned int>();

		if ( portIndex != m_iMidiInCurrentPort )
		{
			if ( m_iMidiInCurrentPort >= 0 )
			{
				m_MidiInOutDeviceManager->unsbindMidiInDevice( m_iMidiInCurrentPort );
				m_iMidiInCurrentPort = -1;
			}

			if ( m_MidiInOutDeviceManager->bindMidiInDevice( portIndex ) )
				m_iMidiInCurrentPort = portIndex;
		}

		if ( m_MidiInOutDeviceManager->isMidiInDeviceRunning( m_iMidiInCurrentPort ) )
		{
			vector<unsigned char> midiMessage = m_MidiInOutDeviceManager->getMidiInMessage( m_iMidiInCurrentPort );
			
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
		else
			m_iMidiInCurrentPort = -1;

	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void MidiInputBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed In and Outlets
		m_MidiInPortInlet = context.getInletHandle( "MidiInPort" );
		m_MidiInMessage0Outlet = context.getOutletHandle( "MidiInMessage0" );
		m_MidiInMessage1Outlet = context.getOutletHandle( "MidiinMessage1" );
		m_MidiInMessage2Outlet = context.getOutletHandle( "MidiInMessage2" );
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void MidiInputBlock::shutdown()
{
	m_MidiInOutDeviceManager->unsbindMidiInDevice( m_iMidiInCurrentPort );
	m_MidiInOutDeviceManager->unregisterMidiInBlock();
}