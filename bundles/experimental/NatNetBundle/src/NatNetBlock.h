#pragma once

#include "_2RealBlock.h"

class NatNetBlockImpl;

class NatNetBlock : public _2Real::bundle::Block
{

public:

	NatNetBlock();
	virtual ~NatNetBlock();
	virtual void shutdown();
	virtual void update();
	virtual void setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::BlockHandle			m_Block;

	_2Real::bundle::InletHandle			m_serverIPIn;
	_2Real::bundle::InletHandle			m_clientIPIn;
	_2Real::bundle::InletHandle			m_isUnicastIn;

	_2Real::bundle::OutletHandle		m_markerSetOut;
	_2Real::bundle::OutletHandle		m_otherMarkerOut;
	_2Real::bundle::OutletHandle		m_rigidBodyOut;
	_2Real::bundle::OutletHandle		m_skeletonOut;
	
	NatNetBlockImpl	*m_blockImpl;
};