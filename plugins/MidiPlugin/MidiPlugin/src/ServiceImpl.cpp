#include "ServiceImpl.h"
#include "_2RealException.h"



using namespace _2Real;
using namespace std;



unsigned int ReadBytes;
unsigned int TimeStamp;

void usage( void ) {
	// Error function in case of incorrect command-line
	// argument specifications.
	std::cout << "\nuseage: cmidiin <port>\n";
	std::cout << "    where port = the device to use (default = 0).\n\n";
	exit( 0 );
}


_2Real::IService *const createService1(void)
{
	_2Real::IService *service = new Service1();
	return service;
}


// Implement the Interface of the first Service ("Counter")
void Service1::setup(_2Real::ServiceContext &context)
{

	try
	{
		// names of slot have to match the names defined in metadata.addOutputSlot 
 
		m_ReadBytes = context.getOutputHandle("m_ReadBytes");
	    m_TimeStamp = context.getOutputHandle("m_TimeStamp");

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
	catch ( RtError &error ) {
		error.printMessage();
	}


};

void Service1::update()
{
	double stamp; 
	std::vector<unsigned char> message;
	int nBytes;
	try
	{
		//do something
		stamp  = m_midiInput->getMessage(&message);
		nBytes = message.size();
	
		for (int i=0; i<nBytes; i++ )
			std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
		if ( nBytes > 0 )
			std::cout << "stamp = " << stamp << std::endl;

		
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
