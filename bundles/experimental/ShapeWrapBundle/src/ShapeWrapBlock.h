#pragma once

#include "_2realblock.h"
#include "engine//_2RealTimestampedData.h"

class FBSimpleNetworkClient;

/*
class ShapeWrapBlock

Defines the ShapeWrapput Block --> Via a ShapeWrap Input Port the ShapeWrap Messages are sent through three Outlets.
*/
class ShapeWrapBlock : public _2Real::bundle::Block
{
public:
	ShapeWrapBlock( _2Real::bundle::ContextBlock & context );
	virtual ~ShapeWrapBlock(void);

	// Virtual Methods
	virtual void					update();
	virtual void					setup( _2Real::bundle::BlockHandle &context );
	virtual void					shutdown();

private:
	virtual void					discardAllOutlets();
	virtual _2Real::Quaternion		rot2Quat(float x, float y, float z);
	virtual _2Real::RigidBody		toRigidBody(const std::string &l, int boneID, int parentID, int markerID);
	virtual _2Real::RigidBody		toRigidBody(const std::string &l, int boneID, int parentID, float* quat);

	// The Block Inlets 
	_2Real::bundle::InletHandle	m_ClientPort;
	_2Real::bundle::InletHandle m_ClientAddress;
	_2Real::bundle::InletHandle	m_ServerPort;
	_2Real::bundle::InletHandle m_ServerAddress;

	// The Block Outlets
	_2Real::bundle::OutletHandle m_SkeletonHandle;
	_2Real::Skeleton m_SkeletonData;

	FBSimpleNetworkClient* m_pNetworkClient;
	_2Real::TimestampedData m_LastUpdate;
	int m_ActorID;
};
