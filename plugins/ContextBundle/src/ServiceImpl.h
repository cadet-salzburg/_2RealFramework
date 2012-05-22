#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"

class Context : public _2Real::Block
{

public:

	Context() : Block() {}
	~Context() {}
	void update();
	void shutdown() {}
	void setup( _2Real::FrameworkContext &context );

private:

	_2Real::OutletHandle			m_ContextEnum;
	_2Real::OutletHandle			m_ContextNumber;

};

class Test : public _2Real::Block
{

public:

	Test() : Block() {}
	~Test() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );

private:

	_2Real::InletHandle				m_ContextEnum;
	_2Real::InletHandle				m_ContextNumber;

};