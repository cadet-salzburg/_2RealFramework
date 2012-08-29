#pragma once
#include "_2RealBlock.h"
#include "EpocDeviceManager.h"

class EpocGyroBlock : public _2Real::bundle::Block
{
public:
	EpocGyroBlock( _2Real::bundle::ContextBlock & context );
	~EpocGyroBlock();
	void					shutdown();
	void					update();
	void					setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::InletHandle			m_userIdIn;
	_2Real::bundle::OutletHandle		m_gyroXOut;
	_2Real::bundle::OutletHandle		m_gyroYOut;

	EpocDeviceManager*					m_EpocDeviceManager;
};