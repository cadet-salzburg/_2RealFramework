#include "ShapeWrapBlock.h"
#include "winsock2.h"
#include <mmsystem.h>
#include "fbsimpletcpip_client.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

ShapeWrapBlock::ShapeWrapBlock( ContextBlock & context ) 
: Block()
, m_pNetworkClient(NULL)
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
		m_UDPPort = context.getInletHandle( "UDPport" );
		m_ActorId = context.getInletHandle( "ActorId" );
		m_Skeleton = context.getOutletHandle( "Write" );
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
		int actorId = m_ActorId.getReadableRef<int>();
		if (actorId >= 0 && actorId < MAX_NETWORK_CONNECT)
		{
			m_pNetworkClient->StopDataStream(actorId);
			m_pNetworkClient->Close(actorId);
		}
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
		int actorId = m_ActorId.getReadableRef<int>();
		if (actorId < 0 || actorId >= MAX_NETWORK_CONNECT)
		{
			cout << "invalid actor id!" << endl;
			return;
		}
		if( m_UDPPort.hasChanged() || m_ActorId.hasChanged() || !m_pNetworkClient)
		{
			int udpPort = m_UDPPort.getReadableRef<int>();
			delete m_pNetworkClient;
			m_pNetworkClient = new FBSimpleNetworkClient();
			m_pNetworkClient->Initialize(actorId);
			if (!m_pNetworkClient->Open(actorId) || !m_pNetworkClient->StartDataStream(actorId))
			{
				cout << "failed to start Shapewrap data stream for actor " << actorId << " on port " << udpPort << endl;
			}
		}

		if (m_pNetworkClient->IsConnectedToServer(actorId))
		{
			FBTime evaltime;
			if (!m_pNetworkClient->FetchDataPacket(evaltime, actorId))
			{
				cout << "failed to fetch Shapewrap data from recorder " << actorId << endl;
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
