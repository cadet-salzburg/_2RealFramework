#pragma once

#include "_2realblock.h"

/*
class EMGShieldBlock

Defines the EMGShieldput Block --> Via a EMGShield Input Port the EMGShield Messages are sent through three Outlets.
*/
class EMGShieldBlock : public _2Real::bundle::Block
{
public:
	EMGShieldBlock( _2Real::bundle::ContextBlock & context );
	~EMGShieldBlock(void);

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

	// current active EMGShield identifier (empty string means none)
	std::string m_EMGShieldIdentifer;
};
