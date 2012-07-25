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
		// If there are no MidiOIn ports available jump out of the method and set the current
		// MidiIn port index to -1
		if ( m_MidiInOutDeviceManager->getMidiInPortCount() == 0 )
		{
			m_iMidiInCurrentPort = -1;
			discardAllOutlets();
			return;
		}

		// Get the current MidiIn port that is set in the Inlet
		unsigned int portIndex = m_MidiInPortInlet.getReadableRef<unsigned int>();

		// If the current MidiIn port differs from the port in the last update call
		// unbind the last port and bind the new one
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

		// Get the MidiIn message at the current index but only if the desired MidiIn device is available
		// and used
		if ( m_MidiInOutDeviceManager->isMidiInDeviceRunning( m_iMidiInCurrentPort ) )
		{
			// Retreive the MidiIn message and store it into a std::vector<unsigned char>
			vector<unsigned char> midiMessage = m_MidiInOutDeviceManager->getMidiInMessage( m_iMidiInCurrentPort );
			
			// Store the message to the three Outles and print it into the console
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
			else
			{
				discardAllOutlets();
			}
			
		}
		else
		{
			discardAllOutlets();
			m_iMidiInCurrentPort = -1; // Set the current MidiIn index back to -1 if something went wrong
		}

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

void MidiInputBlock::discardAllOutlets()
{
	m_MidiInMessage0Outlet.discard();
	m_MidiInMessage1Outlet.discard();
	m_MidiInMessage2Outlet.discard();
}