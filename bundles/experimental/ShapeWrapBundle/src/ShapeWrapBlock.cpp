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
		m_SkeletonHandle = context.getOutletHandle( "Skeleton" );
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
	m_SkeletonHandle.discard();
}

_2Real::Quaternion ShapeWrapBlock::rot2Quat(float x, float y, float z)
{
	float euler[3] = {x, y, z};
	float quat[4] = {0};
	m_pNetworkClient->FBSimpleNetworkClient::ConvertEulertoQuat(euler, quat);
	return _2Real::Quaternion(quat[0], quat[1], quat[2], quat[3]);
}

_2Real::RigidBody ShapeWrapBlock::toRigidBody(const std::string &l, int boneID, int parentID, int markerID)
{
	 _2Real::Point pos(m_pNetworkClient->GetDataTX(markerID, m_ActorID), m_pNetworkClient->GetDataTY(markerID, m_ActorID), m_pNetworkClient->GetDataTZ(markerID, m_ActorID));

	 _2Real::Quaternion orient = rot2Quat(m_pNetworkClient->GetDataRX(markerID, m_ActorID), m_pNetworkClient->GetDataRY(markerID, m_ActorID), m_pNetworkClient->GetDataRZ(markerID, m_ActorID));

	return _2Real::RigidBody(l, boneID, parentID, pos, orient);
}

_2Real::RigidBody ShapeWrapBlock::toRigidBody(const std::string &l, int boneID, int parentID, float* quat)
{
	_2Real::RigidBody rb;
	rb.setId(boneID);
	rb.setParentId(parentID);
	rb.setLabel(l);
	rb.setOrientation(quat[0], quat[1], quat[2], quat[3]);
	return rb;
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
			if (m_pNetworkClient->FetchDataPacket(evaltime, m_ActorID))
			{
				// check if shapewrap data has been updated
				if (evaltime.Get() > m_LastUpdate.timestamp)
				{
					m_LastUpdate.timestamp = evaltime.Get();
					std::vector<_2Real::RigidBody> rigidBodies;
					
					int hipsID = _2Real::RigidBody::INVALID_ID;
					int chestID = _2Real::RigidBody::INVALID_ID;
					int rightKnuckleID = _2Real::RigidBody::INVALID_ID;
					int leftKnuckleID = _2Real::RigidBody::INVALID_ID;
					int lastID = 0;

					// add hips data as root bone
					if (m_pNetworkClient->IsPresent(HIPS_PRESENT, m_ActorID))
					{
						hipsID = lastID++;
						_2Real::RigidBody rigid = toRigidBody("hips", hipsID, _2Real::RigidBody::INVALID_ID, HIPS);
						rigidBodies.push_back(rigid);
					}

					if (m_pNetworkClient->IsPresent(CHEST_PRESENT, m_ActorID))
					{
						chestID = lastID++;
						_2Real::RigidBody rigid = toRigidBody("chest", chestID, hipsID, CHEST);
						rigidBodies.push_back(rigid);
					}
					else
					{
						// no chest
						chestID = hipsID;
					}

					if (m_pNetworkClient->IsPresent(HEAD_PRESENT, m_ActorID))
					{
						_2Real::RigidBody rigid = toRigidBody("head", lastID++, chestID, HEAD);
						rigidBodies.push_back(rigid);
					}

					if (m_pNetworkClient->IsPresent(RIGHTLEG_PRESENT, m_ActorID))
					{
						int kneeID = lastID++;
						_2Real::RigidBody knee = toRigidBody("right_knee", kneeID, hipsID, RKNEE);
						rigidBodies.push_back(knee);
						int ankleID = lastID++;
						_2Real::RigidBody ankle = toRigidBody("right_ankle", ankleID, kneeID, RANKLE);
						rigidBodies.push_back(ankle);
						_2Real::RigidBody toe = toRigidBody("right_toe", lastID++, ankleID, RTOE);
						rigidBodies.push_back(toe);
					}

					if (m_pNetworkClient->IsPresent(LEFTLEG_PRESENT, m_ActorID))
					{
						int kneeID = lastID++;
						_2Real::RigidBody knee = toRigidBody("left_knee", kneeID, hipsID, LKNEE);
						rigidBodies.push_back(knee);
						int ankleID = lastID++;
						_2Real::RigidBody ankle = toRigidBody("left_ankle", ankleID, kneeID, LANKLE);
						rigidBodies.push_back(ankle);
						_2Real::RigidBody toe = toRigidBody("left_toe", lastID++, ankleID, LTOE);
						rigidBodies.push_back(toe);
					}

					if (m_pNetworkClient->IsPresent(RIGHTARM_PRESENT, m_ActorID))
					{
						int shoulderID = lastID++;
						_2Real::RigidBody shoulder = toRigidBody("right_shoulder", shoulderID, chestID, RSHOULDER);
						rigidBodies.push_back(shoulder);
						int elbowID = lastID++;
						_2Real::RigidBody elbow = toRigidBody("right_elbow", elbowID, shoulderID, RELBOW);
						rigidBodies.push_back(elbow);
						int wristID = lastID++;
						_2Real::RigidBody wrist = toRigidBody("right_wrist", wristID, elbowID, RWRIST);
						rigidBodies.push_back(wrist);
						rightKnuckleID = lastID++;
						_2Real::RigidBody knuckle = toRigidBody("right_knuckle", rightKnuckleID, wristID, RKNUCKLE);
						rigidBodies.push_back(knuckle);
					}
					else
					{
						// no right arm
						rightKnuckleID = chestID;
					}

					if (m_pNetworkClient->IsPresent(LEFTARM_PRESENT, m_ActorID))
					{
						int shoulderID = lastID++;
						_2Real::RigidBody shoulder = toRigidBody("left_shoulder", shoulderID, chestID, LSHOULDER);
						rigidBodies.push_back(shoulder);
						int elbowID = lastID++;
						_2Real::RigidBody elbow = toRigidBody("left_elbow", elbowID, shoulderID, LELBOW);
						rigidBodies.push_back(elbow);
						int wristID = lastID++;
						_2Real::RigidBody wrist = toRigidBody("left_wrist", wristID, elbowID, LWRIST);
						rigidBodies.push_back(wrist);
						leftKnuckleID = lastID++;
						_2Real::RigidBody knuckle = toRigidBody("left_knuckle", leftKnuckleID, wristID, LKNUCKLE);
						rigidBodies.push_back(knuckle);
					}
					else
					{
						// no left arm
						leftKnuckleID = chestID;
					}

					if (m_pNetworkClient->IsPresent(RIGHTHAND_PRESENT, m_ActorID))
					{
						SHAPEHAND_DATA handData;
						if (m_pNetworkClient->GetShapeHandData(evaltime, &handData, false, m_ActorID))
						{
							int handID = lastID++;
							_2Real::RigidBody hand = toRigidBody("right_hand", handID, rightKnuckleID, handData.hand);
							rigidBodies.push_back(hand);

							int thumb0ID = lastID++;
							_2Real::RigidBody thumb0 = toRigidBody("right_thumb0", thumb0ID, handID, handData.thumb[0]);
							rigidBodies.push_back(thumb0);
							int thumb1ID = lastID++;
							_2Real::RigidBody thumb1 = toRigidBody("right_thumb1", thumb1ID, thumb0ID, handData.thumb[1]);
							rigidBodies.push_back(thumb1);
							int thumb2ID = lastID++;
							_2Real::RigidBody thumb2 = toRigidBody("right_thumb2", thumb2ID, thumb1ID, handData.thumb[2]);
							rigidBodies.push_back(thumb2);

							int index0ID = lastID++;
							_2Real::RigidBody index0 = toRigidBody("right_index0", index0ID, handID, handData.index[0]);
							rigidBodies.push_back(index0);
							int index1ID = lastID++;
							_2Real::RigidBody index1 = toRigidBody("right_index1", index1ID, index0ID, handData.index[1]);
							rigidBodies.push_back(index1);
							int index2ID = lastID++;
							_2Real::RigidBody index2 = toRigidBody("right_index2", index2ID, index1ID, handData.index[2]);
							rigidBodies.push_back(index2);

							int middle0ID = lastID++;
							_2Real::RigidBody middle0 = toRigidBody("right_middle0", middle0ID, handID, handData.middle[0]);
							rigidBodies.push_back(middle0);
							int middle1ID = lastID++;
							_2Real::RigidBody middle1 = toRigidBody("right_middle1", middle1ID, middle0ID, handData.middle[1]);
							rigidBodies.push_back(middle1);
							int middle2ID = lastID++;
							_2Real::RigidBody middle2 = toRigidBody("right_middle2", middle2ID, middle1ID, handData.middle[2]);
							rigidBodies.push_back(middle2);

							int ring0ID = lastID++;
							_2Real::RigidBody ring0 = toRigidBody("right_ring0", ring0ID, handID, handData.ring[0]);
							rigidBodies.push_back(ring0);
							int ring1ID = lastID++;
							_2Real::RigidBody ring1 = toRigidBody("right_ring1", ring1ID, ring0ID, handData.ring[1]);
							rigidBodies.push_back(ring1);
							int ring2ID = lastID++;
							_2Real::RigidBody ring2 = toRigidBody("right_ring2", ring2ID, ring1ID, handData.ring[2]);
							rigidBodies.push_back(ring2);

							int small0ID = lastID++;
							_2Real::RigidBody small0 = toRigidBody("right_small0", small0ID, handID, handData.small_[0]);
							rigidBodies.push_back(small0);
							int small1ID = lastID++;
							_2Real::RigidBody small1 = toRigidBody("right_small1", small1ID, small0ID, handData.small_[1]);
							rigidBodies.push_back(small1);
							int small2ID = lastID++;
							_2Real::RigidBody small2 = toRigidBody("right_small2", small2ID, small1ID, handData.small_[2]);
							rigidBodies.push_back(small2);
						}
						else
						{
							cout << "could not read right hand data, although it should be present" << endl;
						}
					}
					
					if (m_pNetworkClient->IsPresent(LEFTHAND_PRESENT, m_ActorID))
					{
						SHAPEHAND_DATA handData;
						if (m_pNetworkClient->GetShapeHandData(evaltime, &handData, true, m_ActorID))
						{
							int handID = lastID++;
							_2Real::RigidBody hand = toRigidBody("left_hand", handID, leftKnuckleID, handData.hand);
							rigidBodies.push_back(hand);

							int thumb0ID = lastID++;
							_2Real::RigidBody thumb0 = toRigidBody("left_thumb0", thumb0ID, handID, handData.thumb[0]);
							rigidBodies.push_back(thumb0);
							int thumb1ID = lastID++;
							_2Real::RigidBody thumb1 = toRigidBody("left_thumb1", thumb1ID, thumb0ID, handData.thumb[1]);
							rigidBodies.push_back(thumb1);
							int thumb2ID = lastID++;
							_2Real::RigidBody thumb2 = toRigidBody("left_thumb2", thumb2ID, thumb1ID, handData.thumb[2]);
							rigidBodies.push_back(thumb2);

							int index0ID = lastID++;
							_2Real::RigidBody index0 = toRigidBody("left_index0", index0ID, handID, handData.index[0]);
							rigidBodies.push_back(index0);
							int index1ID = lastID++;
							_2Real::RigidBody index1 = toRigidBody("left_index1", index1ID, index0ID, handData.index[1]);
							rigidBodies.push_back(index1);
							int index2ID = lastID++;
							_2Real::RigidBody index2 = toRigidBody("left_index2", index2ID, index1ID, handData.index[2]);
							rigidBodies.push_back(index2);

							int middle0ID = lastID++;
							_2Real::RigidBody middle0 = toRigidBody("left_middle0", middle0ID, handID, handData.middle[0]);
							rigidBodies.push_back(middle0);
							int middle1ID = lastID++;
							_2Real::RigidBody middle1 = toRigidBody("left_middle1", middle1ID, middle0ID, handData.middle[1]);
							rigidBodies.push_back(middle1);
							int middle2ID = lastID++;
							_2Real::RigidBody middle2 = toRigidBody("left_middle2", middle2ID, middle1ID, handData.middle[2]);
							rigidBodies.push_back(middle2);

							int ring0ID = lastID++;
							_2Real::RigidBody ring0 = toRigidBody("left_ring0", ring0ID, handID, handData.ring[0]);
							rigidBodies.push_back(ring0);
							int ring1ID = lastID++;
							_2Real::RigidBody ring1 = toRigidBody("left_ring1", ring1ID, ring0ID, handData.ring[1]);
							rigidBodies.push_back(ring1);
							int ring2ID = lastID++;
							_2Real::RigidBody ring2 = toRigidBody("left_ring2", ring2ID, ring1ID, handData.ring[2]);
							rigidBodies.push_back(ring2);

							int small0ID = lastID++;
							_2Real::RigidBody small0 = toRigidBody("left_small0", small0ID, handID, handData.small_[0]);
							rigidBodies.push_back(small0);
							int small1ID = lastID++;
							_2Real::RigidBody small1 = toRigidBody("left_small1", small1ID, small0ID, handData.small_[1]);
							rigidBodies.push_back(small1);
							int small2ID = lastID++;
							_2Real::RigidBody small2 = toRigidBody("left_small2", small2ID, small1ID, handData.small_[2]);
							rigidBodies.push_back(small2);
						}
						else
						{
							cout << "could not read left hand data, although it should be present" << endl;
						}
					}

					// update skeleton data
					m_SkeletonData.setGlobal(true);
					m_SkeletonData.setRigidBodies(rigidBodies);
					m_SkeletonData.updateBoundingBox();
					m_SkeletonData.setLabel("shapewrap_skeleton");
					// TODO make some unique ID for different concurrent skeletons ...
					m_SkeletonData.setId(m_ActorID);
					m_SkeletonHandle.getWriteableRef<_2Real::Skeleton>() = m_SkeletonData;
				}
				else
				{
					m_SkeletonHandle.discard();
				}
			}
			else
			{
				cout << "failed to fetch Shapewrap data from recorder!" << endl;
				m_SkeletonHandle.discard();
			}
		}
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
