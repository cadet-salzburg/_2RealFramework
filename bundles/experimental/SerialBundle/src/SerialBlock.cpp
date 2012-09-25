#include "SerialBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

SerialBlock::SerialBlock( ContextBlock & context ) 
: Block()
, m_bBound(false)
, m_bBindable(true)
{
	m_SerialDeviceManager = static_cast<SerialDeviceManager*>( &context );
	m_SerialDeviceManager->registerSerialBlock();
}

SerialBlock::~SerialBlock()
{
	m_SerialMessage.clear();
}

void SerialBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed In and Outlets
		m_SerialPortInlet = context.getInletHandle( "SerialPort" );
		m_iBaudRateInlet = context.getInletHandle( "BaudRate" );
		m_DataInIntel = context.getInletHandle( "Write" );
		m_DataOutOutlet = context.getOutletHandle( "Read" );
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
	m_DataOutOutlet.discard();
}

void SerialBlock::update()
{
	try
	{
		// 1. check if we should unbind due to inlet change
		if (m_SerialPortInlet.hasChanged() || m_iBaudRateInlet.hasChanged())
		{
			if (m_bBound)
			{
				m_SerialDeviceManager->unbindSerialDevice(m_SerialIdentifer);
				m_bBound = false;
				m_SerialIdentifer = "";
			}
			m_bBindable = true;
		}

		// 2. check if we should try to bind
		if (!m_bBound && m_bBindable)
		{
			// Get the current Serial port that is set in the Inlet
			std::string port = m_SerialPortInlet.getReadableRef<std::string>();
			unsigned int baudRrate = m_iBaudRateInlet.getReadableRef<unsigned int>();			
			
			if (m_SerialDeviceManager->bindSerialDevice(port, baudRrate))
			{
				m_SerialIdentifer = port;
				m_bBound = true;
				m_bBindable = false;
			}
			else
			{
				m_SerialIdentifer = "";
				m_bBound = false;
				m_bBindable = false;
			}
		}

		// 3. read and write data if bound
		if (m_bBound)
		{
			// 3a. read data
			if (m_SerialDeviceManager->hasDataPending(m_SerialIdentifer) > 0)
			{
				// Retreive the Serial message and store it into a std::vector<unsigned char>
				m_SerialMessage.clear();
				m_SerialMessage = m_SerialDeviceManager->getSerialMessage( m_SerialIdentifer );
				// write it to block's output
				m_DataOutOutlet.getWriteableRef<vector<unsigned char>>() = m_SerialMessage;
			}

			// 3b. write data
			if (m_DataInIntel.hasChanged())
			{
				vector<unsigned char> serialMessage = m_DataInIntel.getReadableRef<vector<unsigned char>>();
				if (serialMessage.size() > 0)
				{
					m_SerialDeviceManager->sendSerialMessage(m_SerialIdentifer, serialMessage);
#if 0
					printf("---\n");
					for (vector<unsigned char>::iterator it = serialMessage.begin(); it != serialMessage.end(); it++)
					{
						printf("%d %c\n", *it, *it);
					}
#endif
				}
			}
		}
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
