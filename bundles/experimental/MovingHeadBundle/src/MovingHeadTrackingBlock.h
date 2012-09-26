#pragma once

#include "_2realblock.h"

/*
class MovingHeadTrackingBlock

Defines the MovingHeadput Block --> Via a MovingHead Input Port the MovingHead Messages are sent through three Outlets.
*/
class MovingHeadTrackingBlock : public _2Real::bundle::Block
{
public:
	MovingHeadTrackingBlock( _2Real::bundle::ContextBlock & context );
	virtual ~MovingHeadTrackingBlock(void);

	// Virtual Methods
	virtual void					update();
	virtual void					setup( _2Real::bundle::BlockHandle &context );
	virtual void					shutdown();

private:
	void							discardAllOutlets();

	// The Block Inlets 
	_2Real::bundle::InletHandle m_CenterOfMassInlet;
	_2Real::bundle::InletHandle m_UserIDInlet;
	_2Real::bundle::InletHandle m_MotorIDXInlet;
	_2Real::bundle::InletHandle m_MotorIDYInlet;

	_2Real::bundle::OutletHandle m_MotorIDOutlet;
	_2Real::bundle::OutletHandle m_CommandOutlet;
	_2Real::bundle::OutletHandle m_ValueOutlet;

	// current active MovingHead identifier (empty string means none)
	std::string m_MovingHeadIdentifer;

	int m_CurrentPosX;
	int m_CurrentPosY;
};
