#pragma once
#include "_2RealBlock.h"
#include "EpocDeviceManager.h"

class EpocAffectivBlock : public _2Real::bundle::Block
{
public:
	EpocAffectivBlock( _2Real::bundle::ContextBlock & context );
	~EpocAffectivBlock();
	void					shutdown();
	void					update();
	void					setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::InletHandle			m_userIdIn;
	_2Real::bundle::OutletHandle		m_engagemementOut;
	_2Real::bundle::OutletHandle		m_frustrationOut;
	_2Real::bundle::OutletHandle		m_meditationOut;
	_2Real::bundle::OutletHandle		m_excitementOut;

	EpocDeviceManager*					m_EpocDeviceManager;
};