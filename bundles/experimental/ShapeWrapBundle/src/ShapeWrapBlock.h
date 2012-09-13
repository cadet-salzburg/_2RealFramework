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

	// The Block Inlets 
	_2Real::bundle::InletHandle	m_UDPPort;
	_2Real::bundle::InletHandle	m_ActorId;

	// The Block Outlets
	_2Real::bundle::OutletHandle m_Skeleton;

	FBSimpleNetworkClient* m_pNetworkClient;
	_2Real::TimestampedData m_LastUpdate;
};
