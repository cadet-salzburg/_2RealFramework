#include "_2RealDatatypes.h"

#include "OpenBeaconBlock.h"

#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include <iostream>
#include "rapidjson/document.h"

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

#include <sstream>
template <class T>
inline std::string to_string (const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

OpenBeaconBlock::OpenBeaconBlock()
: m_pSocket(NULL)
{}

OpenBeaconBlock::~OpenBeaconBlock()
{
}

void OpenBeaconBlock::setup( BlockHandle &block )
{
	try
	{
		m_Context = block;

		m_UDPPort = m_Context.getInletHandle( "UDPport" );
		m_Tags = m_Context.getOutletHandle( "tags" );
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void OpenBeaconBlock::update()
{
	try
	{
		if( m_UDPPort.hasChanged() || !m_pSocket)
		{
			int udpPort = m_UDPPort.getReadableRef<int>();
			Poco::Net::SocketAddress sa(Poco::Net::IPAddress(), udpPort);
			delete m_pSocket;
			m_pSocket = new Poco::Net::DatagramSocket(sa);
		}
		
		// read from udp port and update tags
		char buffer[2048];
		Poco::Net::SocketAddress sender;
		int n = m_pSocket->receiveFrom(buffer, sizeof(buffer)-1, sender);
		buffer[n] = '\0';
		
		//std::cout << sender.toString() << ": " << buffer << std::endl;
		
		if (n > 0)
		{
			rapidjson::Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
			// In-situ parsing, decode strings directly in the source string. Source must be string.
			if (document.ParseInsitu<0>(buffer).HasParseError())
			{
				cout << "failed to parse json data in openbeacon block" << endl;
			}
			else
			{
				m_Tags.discard();
				std::vector<_2Real::Point> points = m_Tags.getWriteableRef<std::vector<_2Real::Point>>(); 
				
				const rapidjson::Value& tags = document["tag"];
				assert(tags.IsArray());
				for (rapidjson::SizeType i = 0; i < tags.Size(); i++)
				{
					const rapidjson::Value& tag = tags[i];
					assert(tag["id"].IsInt());
					assert(tag["px"].IsInt());
					assert(tag["py"].IsInt());

					points.push_back(_2Real::Point(tag["px"].GetInt(), tag["py"].GetInt(), 0, to_string<int>(tag["id"].GetInt()), tag["id"].GetInt()));
				}
			}
		}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void OpenBeaconBlock::shutdown()
{
	delete m_pSocket;
}