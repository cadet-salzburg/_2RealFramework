#pragma once

#include "_2realblock.h"

/*
class MovingHeadBlock

Defines the MovingHeadput Block --> Via a MovingHead Input Port the MovingHead Messages are sent through three Outlets.
*/
class MovingHeadBlock : public _2Real::bundle::Block
{
public:
	MovingHeadBlock( _2Real::bundle::ContextBlock & context );
	~MovingHeadBlock(void);

	// Virtual Methods
	virtual void					update();
	virtual void					setup( _2Real::bundle::BlockHandle &context );
	virtual void					shutdown();

private:
	void							discardAllOutlets();

	// The Block Inlets 
	_2Real::bundle::InletHandle	m_MotorIDInlet;
	_2Real::bundle::InletHandle m_CommandInlet;
	_2Real::bundle::InletHandle m_ValueIntel;

	// The Block Outlets
	_2Real::bundle::OutletHandle m_SerialOutlet;

	// current active MovingHead identifier (empty string means none)
	std::string m_MovingHeadIdentifer;
};
