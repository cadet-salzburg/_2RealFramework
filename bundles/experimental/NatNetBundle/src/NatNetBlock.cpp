#include "_2RealDatatypes.h"
#include "NatNetBlock.h"

#include "NatNetTypes.h"
#include "NatNetClient.h"

#include <iostream>
#include <vector>
#include <string>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;


class NatNetBlockImpl
{
public:
	NatNetBlockImpl() : 
		m_isShutDown( false ),
		m_Client( NULL ),
		oldData( 0 )
	{}

	~NatNetBlockImpl()
	{
		if( !m_isShutDown )
			this->shutdown();
	}

	void setup(string cIP, string sIP, bool isU)
	{
		clientIP = cIP;
		serverIP = sIP;
		isUnicast = isU;

			// release previous server
		if(m_Client)
		{
			m_Client->Uninitialize();
			delete m_Client;
		}

		// create NatNet client
		m_Client = new NatNetClient(isUnicast);


		// Init Client and connect to NatNet server
		int retCode = m_Client->Initialize( (char*) clientIP.data(), (char*) serverIP.data());
		if (retCode != ErrorCode_OK)
		{
			throw _2Real::Exception("Unable to connect to server.");
		}
		else
		{
			// print server info
			sServerDescription ServerDescription;
			memset(&ServerDescription, 0, sizeof(ServerDescription));
			m_Client->GetServerDescription(&ServerDescription);
			if(!ServerDescription.HostPresent)
			{
				throw _2Real::Exception("Unable to connect to server. Host not present. Exiting.");
			}
			else
			{
				unsigned char ver[4];
				m_Client->NatNetVersion(ver);
				printf("Client initialized and ready (NatNet ver. %d.%d.%d.%d.)\n", ver[0], ver[1], ver[2], ver[3]);
				printf("Server application: %s (ver. %d.%d.%d.%d)\n", ServerDescription.szHostApp, ServerDescription.HostAppVersion[0],
				ServerDescription.HostAppVersion[1],ServerDescription.HostAppVersion[2],ServerDescription.HostAppVersion[3]);
				printf("NatNet Version: %d.%d.%d.%d\n", ServerDescription.NatNetVersion[0], ServerDescription.NatNetVersion[1],
				ServerDescription.NatNetVersion[2], ServerDescription.NatNetVersion[3]);
				printf("Client IP: %s\n", (char*) clientIP.data());
				printf("Server IP: %s\n", (char*) clientIP.data());
				printf("Server Name: %s\n\n", ServerDescription.szHostComputerName);
			}
		}
	}

	bool update( std::vector < Point > &otherMarkers, std::vector < RigidBody > &rigidBodies, std::vector < Skeleton > &skeletons )
	{
		otherMarkers.clear();
		rigidBodies.clear();
		skeletons.clear();

		std::vector < Point > markerSets;

		sFrameOfMocapData* data = m_Client->GetLastFrameOfData();
		
		if(!data) return false;
		
		if(data->iFrame == oldData) return false;

		oldData = data->iFrame;	

		int i =data->iFrame;
		char frameNr [33];
		itoa (i, frameNr, 10);
		string label = "Frame ";
		label.append(frameNr);


		// Other Markers
		for(int i=0; i < data->nOtherMarkers; i++)
		{	
			otherMarkers.push_back(_2Real::Point(data->OtherMarkers[i][0], data->OtherMarkers[i][1], data->OtherMarkers[i][2], label, i));
		}

		// Rigid Bodies
		for(int i=0; i < data->nRigidBodies; i++)
		{
			markerSets.clear();
			_2Real::Point position(data->RigidBodies[i].x, data->RigidBodies[i].y, 	data->RigidBodies[i].z);
			_2Real::Quaternion orientation(data->RigidBodies[i].qw, data->RigidBodies[i].qx, data->RigidBodies[i].qy, data->RigidBodies[i].qz);

			for(int iMarker=0; iMarker < data->RigidBodies[i].nMarkers; iMarker++)
			{
			 if(data->RigidBodies[i].MarkerIDs && data->RigidBodies[i].Markers)
				 markerSets.push_back(_2Real::Point(data->RigidBodies[i].Markers[iMarker][0],
					data->RigidBodies[i].Markers[iMarker][1],
					data->RigidBodies[i].Markers[iMarker][2],
					label,
					data->RigidBodies[i].MarkerIDs[iMarker]));
			}
			rigidBodies.push_back( _2Real::RigidBody(markerSets, label, data->RigidBodies[i].ID, 0, position, orientation) );
		}

		 // Skeletons
		for(int i=0; i < data->nSkeletons; i++)
		{
			std::vector<_2Real::RigidBody> rigidBodies;
			sSkeletonData skData = data->Skeletons[i];
			for(int j=0; j< skData.nRigidBodies; j++)
			{
				sRigidBodyData rbData = skData.RigidBodyData[j];

				_2Real::Point position(rbData.x, rbData.y, rbData.z);
				_2Real::Quaternion orientation(rbData.qw, rbData.qx, rbData.qy, rbData.qz);

				markerSets.clear();
											
			for(int iMarker=0; iMarker < data->RigidBodies[i].nMarkers; iMarker++)
			{
			  markerSets.push_back(_2Real::Point(data->RigidBodies[i].Markers[iMarker][0],
					data->RigidBodies[i].Markers[iMarker][1],
					data->RigidBodies[i].Markers[iMarker][2],
					label,
					data->RigidBodies[i].MarkerIDs[iMarker]));
			}
			rigidBodies.push_back( _2Real::RigidBody(markerSets, label, skData.RigidBodyData[j].ID, skData.skeletonID, position, orientation ) );
		}
			skeletons.push_back( _2Real::Skeleton( rigidBodies, label, skData.skeletonID, true ) );
		}		

		return true;
	}

	void shutdown()
	{
		m_Client->Uninitialize();

		m_isShutDown = true;
	}

private:
	bool	m_isShutDown;
	NatNetClient* m_Client;
	string clientIP;
	string serverIP;
	bool isUnicast;
	int oldData;
};

NatNetBlock::NatNetBlock() : 
Block(),
	m_blockImpl( new NatNetBlockImpl() )
{
	std::cout << "natnetblock .ctor" << std::endl;
}

NatNetBlock::~NatNetBlock()
{
	std::cout << "natnetblock .dtor" << std::endl;

	if( m_blockImpl )
	{
		delete m_blockImpl;
		m_blockImpl = NULL;
	}
}

void NatNetBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;

		m_serverIPIn = m_Block.getInletHandle("server_ip");
		m_clientIPIn = m_Block.getInletHandle("client_ip");
		m_isUnicastIn = m_Block.getInletHandle("isUnicast");

		m_otherMarkerOut = m_Block.getOutletHandle("other_marker");
		m_rigidBodyOut = m_Block.getOutletHandle("rigid_body");
		m_skeletonOut = m_Block.getOutletHandle("skeleton");

		string clientIP = m_clientIPIn.getReadableRef<string>();
		string serverIP = m_serverIPIn.getReadableRef<string>();
		bool isUnicast = m_isUnicastIn.getReadableRef<bool>();
		
		if( m_blockImpl )
		m_blockImpl->setup( clientIP, serverIP, isUnicast );
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

void NatNetBlock::update()
{
	try
	{
		if( m_blockImpl )
		{
			std::vector < Point > & otherMarkers = m_otherMarkerOut.getWriteableRef< std::vector < Point > >();
			std::vector < RigidBody > &rigidBodies = m_rigidBodyOut.getWriteableRef< std::vector < RigidBody > >();
			std::vector < Skeleton > &skeletons = m_skeletonOut.getWriteableRef< std::vector < Skeleton > >();

			if( !m_blockImpl->update( otherMarkers, rigidBodies, skeletons ) )
			{
				m_otherMarkerOut.discard();
				m_rigidBodyOut.discard();
				m_skeletonOut.discard();
			}
		}
		else
		{
			m_otherMarkerOut.discard();
			m_rigidBodyOut.discard();
			m_skeletonOut.discard();
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

void NatNetBlock::shutdown() {
	if( m_blockImpl )
		m_blockImpl->shutdown();
}