#include "ServiceImpl.h"
#include "_2RealException.h"
#include "_2RealEnum.h"
#include "Poco/Mutex.h"

using namespace _2Real;
using namespace std;

void Service1::setup(_2Real::ServiceContext &context)
{
	try
	{
		m_CurrentCount = 1;
		m_CounterValue = context.getOutputHandle("counter outlet");
		m_Test = context.getOutputHandle("test");
	
		m_midiInput = new RtMidiIn();
		// Check available ports
		unsigned int port = 0;
		unsigned int nPorts = m_midiInput->getPortCount();
		if ( port >= nPorts ) {
			delete m_midiInput;
			std::cout << "Invalid port specifier!\n";

		}

		try {
			m_midiInput->openPort( port );
		}
		catch ( RtError &error ) {
			error.printMessage();

		}
		// Don't ignore sysex, timing, or active sensing messages.
		m_midiInput->ignoreTypes( false, false, false );

		std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
		 
	}


	catch (_2Real::Exception &e)
	{
		e.rethrow();
	}
	catch (std::exception &e)
	{
		std::ostringstream msg;
		msg << "exception during service setup: " << e.what();
		throw _2Real::ServiceException(msg.str());
	}
};

void Service1::update()
{
	try
	{

		std::vector<unsigned char> message;
		int nBytes, i;
		double stamp;

		stamp = m_midiInput->getMessage( &message );
		nBytes = message.size();
		if(nBytes>0)
		{

			for ( i=0; i<nBytes; i++ ){
				m_Test.data< std::vector < unsigned char  > >() = message;
				std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
			}

		}

	}
	catch (_2Real::Exception &e)
	{
		e.rethrow();
	}
	catch (std::exception &e)
	{
		std::ostringstream msg;
		msg << "exception during service update: " << e.what();
		throw _2Real::ServiceException(msg.str());
	}
};

void Service1::shutdown()
{
	delete m_midiInput;
}

void Service2::setup( _2Real::ServiceContext &context )
{

	m_midiOutput = new RtMidiOut();
	m_OutputMidi = context.getOutputHandle("outputmidi");

	// Check available ports
	unsigned int port = 0;
	unsigned int nPorts = m_midiOutput->getPortCount();
	if ( port >= nPorts ) {
		delete m_midiOutput;
		std::cout << "Invalid port specifier!\n";

	}

	try {
		m_midiOutput->openPort( port );
	}
	catch ( RtError &error ) {
		error.printMessage();

	}
	 

 

}

void Service2::update()
{
	try
	{
	std::vector<unsigned char> message;
	message.push_back( 192 );
	message.push_back( 5 );
	m_midiOutput->sendMessage( &message );
	m_OutputMidi.data< vector<unsigned char> >() = message;
	}

	catch (_2Real::Exception &e)
	{
		e.rethrow();
	}
	catch (std::exception &e)
	{
		std::ostringstream msg;
		msg << "exception during service update: " << e.what();
		throw _2Real::ServiceException(msg.str());
	}

}
