#include "MidiInputBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

MidiInputBlock::MidiInputBlock( ContextBlock & context ) :
	Block(),
	m_iMidiInCurrentPort( -1 ),
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
			// If there are no Ports available set the current Port index back to -1 and jump out of the method
			if ( m_MidiIn->getPortCount() == 0 )
			{
				m_iMidiInCurrentPort = -1;
				return;
			}

			// Check if the current Port index has changed since the last update call and open the new Port accordingly
			rescanMidiInPorts();

			// Store the MidiIn message in a vector
			vector<unsigned char> midiMessage;
			m_MidiIn->getMessage( &midiMessage );

			// If a message was successfully generated send it via the three Block Outlets
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
	else // Try initializing the RtMidiIn Instance if something should have gone wrong during the setup phase
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
		// Get the handles to all needed In and Outlets
		m_MidiInPortInlet = context.getInletHandle( "MidiInPort" );
		m_MidiInMessage0Outlet = context.getOutletHandle( "MidiInMessage0" );
		m_MidiInMessage1Outlet = context.getOutletHandle( "MidiinMessage1" );
		m_MidiInMessage2Outlet = context.getOutletHandle( "MidiInMessage2" );

		// Initialize the RtMidiIn instance
		initRtMidiIn();
	}
	catch ( RtError& error )
	{
		error.printMessage();
		// Delete the RtMidiIn instance if something went wrong while allocating memory
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
	// Ask the Inlet for the Port that should be opened / used
	unsigned int portToOpen = m_MidiInPortInlet.getReadableRef<unsigned int>();

	// If the Port index in the Inlet is different from the currently used Port index
	// close the old Port and open a new Port according to the new Port index
	if ( m_iMidiInCurrentPort != portToOpen )
	{
		m_MidiIn->closePort();
		m_iMidiInCurrentPort = portToOpen;
		m_MidiIn->openPort( m_iMidiInCurrentPort );
		m_MidiIn->ignoreTypes( false, false, false );
	}
}

void MidiInputBlock::initRtMidiIn()
{
	m_MidiIn = new RtMidiIn();
}
