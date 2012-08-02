#pragma once

#include "_2RealBlock.h"

class Out : public _2Real::bundle::Block
{

public:

	Out() : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

private:

	_2Real::bundle::InletHandle			m_Msg;
	_2Real::bundle::OutletHandle		m_Out;

};

class InOut : public _2Real::bundle::Block
{

public:

	InOut() : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

private:

	_2Real::bundle::InletHandle			m_Msg;
	_2Real::bundle::InletHandle			m_In;
	_2Real::bundle::OutletHandle		m_Out;

};

class In : public _2Real::bundle::Block
{

public:

	In() : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

private:

	int									m_Counter;
	_2Real::bundle::InletHandle			m_In;
	_2Real::bundle::InletHandle			m_Msg;

};