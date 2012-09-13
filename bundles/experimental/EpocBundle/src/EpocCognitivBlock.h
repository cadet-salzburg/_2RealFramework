#pragma once
#include "_2RealBlock.h"
#include "EpocDeviceManager.h"

class EpocCognitivBlock : public _2Real::bundle::Block
{
public:
	EpocCognitivBlock( _2Real::bundle::ContextBlock & context );
	virtual ~EpocCognitivBlock();
	virtual void					shutdown();
	virtual void					update();
	virtual void					setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::InletHandle							m_userIdIn;
	_2Real::bundle::OutletHandle						m_neutralOut;
	_2Real::bundle::OutletHandle						m_pushOut;
	_2Real::bundle::OutletHandle						m_pullOut;
	_2Real::bundle::OutletHandle						m_liftOut;
	_2Real::bundle::OutletHandle						m_dropOut;
	_2Real::bundle::OutletHandle						m_leftOut;
	_2Real::bundle::OutletHandle						m_rightOut;
	_2Real::bundle::OutletHandle						m_rotateLeftOut;
	_2Real::bundle::OutletHandle						m_rotateRightOut;
	_2Real::bundle::OutletHandle						m_rotateClockwiseOut;
	_2Real::bundle::OutletHandle						m_rotateCounterClockwiseOut;
	_2Real::bundle::OutletHandle						m_rotateForwardsOut;
	_2Real::bundle::OutletHandle						m_rotateReverseOut;
	_2Real::bundle::OutletHandle						m_disappearOut;

	EpocDeviceManager*					m_EpocDeviceManager;
};