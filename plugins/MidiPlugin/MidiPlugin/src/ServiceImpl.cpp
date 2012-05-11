#include "ServiceImpl.h"
#include "_2RealException.h"

#include "RtMidi.h"

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

void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
	unsigned int nBytes = message->size();
	Service1 *self = static_cast<Service1*>(userData); 

	for ( unsigned int i=0; i<nBytes; i++ )
	{
		//std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
	 	self->m_ReadBytes.data<unsigned int>() = (int)message->at(i);
		 std::cout << "Byte " << i << " = " << self->m_ReadBytes.data<unsigned int>() << ", ";
	}

	if ( nBytes > 0 )
 
	    self->m_TimeStamp.data<unsigned int>()= deltatime;
	    std::cout << "stamp = " <<  self->m_TimeStamp.data<unsigned int>() << std::endl;   
}

bool chooseMidiPort( RtMidiIn *rtmidi )
{
	std::cout << "\nWould you like to open a virtual input port? [y/N] ";

	std::string keyHit;
	std::getline( std::cin, keyHit );
	if ( keyHit == "y" ) {
		rtmidi->openVirtualPort();
		return true;
	}

	std::string portName;
	unsigned int i = 0, nPorts = rtmidi->getPortCount();
	if ( nPorts == 0 ) {
		std::cout << "No input ports available!" << std::endl;
		return false;
	}

	if ( nPorts == 1 ) {
		std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
	}
	else {
		for ( i=0; i<nPorts; i++ ) {
			portName = rtmidi->getPortName(i);
			std::cout << "  Input port #" << i << ": " << portName << '\n';
		}

		do {
			std::cout << "\nChoose a port number: ";
			std::cin >> i;
		} while ( i >= nPorts );
	}

	//std::getline( std::cin, keyHit );  // used to clear out stdin
	rtmidi->openPort( i );

	return true;
}

_2Real::IService *const createService1(void)
{
	_2Real::IService *service = new Service1();
	return service;
}


// Implement the Interface of the first Service ("Counter")
void Service1::setup(_2Real::ServiceContext &context)
{
	RtMidiIn  *midiInput = 0;
	try
	{
		// names of slot have to match the names defined in metadata.addOutputSlot 
		m_InValue = context.getInputHandle("s1 inlet");
		m_ReadBytes = context.getOutputHandle("m_ReadBytes");
		m_TimeStamp = context.getOutputHandle("m_TimeStamp");

		midiInput = new RtMidiIn();
		// Call function to select port.
		if ( chooseMidiPort( midiInput ) == false ) goto cleanup;

		// Set our callback function.  This should be done immediately after
		// opening the port to avoid having incoming messages written to the
		// queue instead of sent to the callback function.
		midiInput->setCallback( &mycallback, (void*)this );

		// Don't ignore sysex, timing, or active sensing messages.
		midiInput->ignoreTypes( false, false, false );

		std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
		char input;
		std::cin.get(input);

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

cleanup:

	delete midiInput;

};

void Service1::update()
{
	try
	{
		//do something
		
		std::cout<<" Bytes Read"<<m_ReadBytes.data<unsigned int>()<<std::endl;
		std::cout<<" Time Stamp"<<m_TimeStamp.data<unsigned int>()<<std::endl;
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
