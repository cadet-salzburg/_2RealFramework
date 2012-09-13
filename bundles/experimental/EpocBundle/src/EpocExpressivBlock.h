#pragma once
#include "_2RealBlock.h"
#include "EpocDeviceManager.h"

class EpocExpressivBlock : public _2Real::bundle::Block
{
public:
	EpocExpressivBlock( _2Real::bundle::ContextBlock & context );
	virtual ~EpocExpressivBlock();
	virtual void					shutdown();
	virtual void					update();
	virtual void					setup( _2Real::bundle::BlockHandle &context );

private:
	_2Real::bundle::InletHandle							m_userIdIn;
	_2Real::bundle::OutletHandle						m_blinkOut;
	_2Real::bundle::OutletHandle						m_rightWinkOut;
	_2Real::bundle::OutletHandle						m_leftWinkOut;
	_2Real::bundle::OutletHandle						m_lookRightOut;
	_2Real::bundle::OutletHandle						m_lookLeftOut;
	_2Real::bundle::OutletHandle						m_raiseBrowOut;
	_2Real::bundle::OutletHandle						m_furrowBrowOut;
	_2Real::bundle::OutletHandle						m_smileOut;
	_2Real::bundle::OutletHandle						m_clenchOut;
	_2Real::bundle::OutletHandle						m_smirkRightOut;
	_2Real::bundle::OutletHandle						m_smirkLeftOut;
	_2Real::bundle::OutletHandle						m_laughOut;

	EpocDeviceManager*					m_EpocDeviceManager;
};