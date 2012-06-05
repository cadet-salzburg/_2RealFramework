#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"

class Out : public _2Real::Block
{

public:

	Out() : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );
	~Out() {}

private:

	int								m_Counter;
	_2Real::OutletHandle			m_Out;

};

class InOut : public _2Real::Block
{

public:

	InOut() : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );
	~InOut() {}

private:

	int								m_Counter;
	_2Real::InletHandle				m_In;
	_2Real::OutletHandle			m_Out;

};

class In : public _2Real::Block
{

public:

	In() : Block() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );
	~In() {}

private:

	int								m_Counter;
	_2Real::InletHandle				m_In;

};