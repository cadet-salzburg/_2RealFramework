#pragma once

#include "_2RealBlock.h"

class OpenBeaconBlock : public _2Real::bundle::Block
{
public:

	OpenBeaconBlock();
	~OpenBeaconBlock();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::BlockHandle		m_Block;

	_2Real::bundle::InletHandle		m_dummyIn;
	_2Real::bundle::OutletHandle	m_dummyOut;
};