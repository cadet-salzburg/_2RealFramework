#include "ShapeWrapBlock.h"
#include "winsock2.h"
#include <mmsystem.h>
#include "fbsimpletcpip_client.h"

#include <iostream>
#include <vector>

#define DEFAULT_ACTOR_ID	0

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

ShapeWrapBlock::ShapeWrapBlock( ContextBlock & context ) 
: Block()
, m_pNetworkClient(NULL)
, m_ActorID(DEFAULT_ACTOR_ID)
{
}

ShapeWrapBlock::~ShapeWrapBlock()
{
	delete m_pNetworkClient;
}

void ShapeWrapBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed In and Outlets
		m_ClientPort = context.getInletHandle( "ClientPort" );
		m_ClientAddress = context.getInletHandle( "ClientAddress" );
		m_ServerPort = context.getInletHandle( "ServerPort" );
		m_ServerAddress = context.getInletHandle( "ServerAddress" );
		m_Skeleton = context.getOutletHandle( "Skeleton" );
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void ShapeWrapBlock::shutdown()
{
	if (m_pNetworkClient)
	{
		m_pNetworkClient->StopDataStream(m_ActorID);
		m_pNetworkClient->Close(m_ActorID);
	}
}

void ShapeWrapBlock::discardAllOutlets()
{
	m_Skeleton.discard();
}

void ShapeWrapBlock::update()
{
	try
	{
		if( m_ServerPort.hasChanged() || m_ClientPort.hasChanged() || !m_pNetworkClient || m_ClientAddress.hasChanged() || m_ServerAddress.hasChanged() )
		{
			int serverPort = m_ServerPort.getReadableRef<int>();
			std::string serverAddress = m_ServerAddress.getReadableRef<std::string>();
			int clientPort = m_ClientPort.getReadableRef<int>();
			std::string clientAddress = m_ClientAddress.getReadableRef<std::string>();
			delete m_pNetworkClient;
			m_pNetworkClient = new FBSimpleNetworkClient();
			m_pNetworkClient->Initialize(m_ActorID);
			m_pNetworkClient->SetClientPort(clientPort);
			m_pNetworkClient->SetClientAddress((char*)clientAddress.c_str(), m_ActorID);
			m_pNetworkClient->SetServerPort(serverPort);
			m_pNetworkClient->SetServerAddress((char*)serverAddress.c_str(), m_ActorID);
			if (!m_pNetworkClient->Open(m_ActorID))
			{
				cout << "failed to open network port to Shapewrap on port " << clientPort << endl;
			}
			if (!m_pNetworkClient->GetChannels(m_ActorID))
			{
				cout << "failed to retrieve channel information for Shapewrap system " << endl;
			}
			if (!m_pNetworkClient->StartDataStream(m_ActorID))
			{
				cout << "failed to start Shapewrap data stream on port " << clientPort << endl;
			}
		}
		
		if (m_pNetworkClient->IsConnectedToServer(m_ActorID))
		{
			FBTime evaltime;
			if (!m_pNetworkClient->FetchDataPacket(evaltime, m_ActorID))
			{
				cout << "failed to fetch Shapewrap data from recorder!" << endl;
			}
			else
			{
				// check if shapewrap data has been updated
				if (evaltime.Get() > m_LastUpdate.timestamp)
				{
					m_LastUpdate.timestamp = evaltime.Get();
					_2Real::Skeleton skeleton = m_Skeleton.getWriteableRef<_2Real::Skeleton>(); 


					
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
