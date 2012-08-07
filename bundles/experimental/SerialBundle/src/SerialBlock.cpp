#include "SerialBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

SerialBlock::SerialBlock( ContextBlock & context ) :
	Block()
{
	m_SerialDeviceManager = static_cast<SerialDeviceManager*>( &context );
	m_SerialDeviceManager->registerSerialBlock();
}

SerialBlock::~SerialBlock()
{
}

void SerialBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed In and Outlets
		m_SerialPortInlet = context.getInletHandle( "SerialPort" );
		m_iBaudRateInlet = context.getInletHandle( "BaudRate" );
		m_ReadIntel = context.getInletHandle( "Read" );
		m_WriteOutlet = context.getOutletHandle( "Write" );
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void SerialBlock::shutdown()
{
	m_SerialDeviceManager->unregisterSerialBlock();
}

void SerialBlock::discardAllOutlets()
{
	m_WriteOutlet.discard();
}

void SerialBlock::update()
{
	try
	{
		// Get the current Serial port that is set in the Inlet
		std::string port = m_SerialPortInlet.getReadableRef<std::string>();
		unsigned int bauRrate = m_iBaudRateInlet.getReadableRef<unsigned int>();
/*
		// Retreive the Serial message and store it into a std::vector<unsigned char>
		vector<unsigned char> SerialMessage = m_SerialDeviceManager->getSerialMessage( m_iSerialCurrentPort );
		m_SerialMessage0Outlet.getWriteableRef<unsigned char>() = SerialMessage[0];
*/

	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
