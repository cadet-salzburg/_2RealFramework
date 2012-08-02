#pragma once

#include "_2RealBlock.h"

class ImageOut : public _2Real::bundle::Block
{

public:

	ImageOut() : Block() {};

	void update();
	void setup( _2Real::bundle::BlockHandle &context );
	void shutdown();

private:

	_2Real::bundle::OutletHandle		m_Out;

};

class ImageInOut : public _2Real::bundle::Block
{

public:

	ImageInOut() : Block() {};

	void update();
	void setup( _2Real::bundle::BlockHandle &context );
	void shutdown();

private:

	_2Real::bundle::InletHandle			m_In;
	_2Real::bundle::OutletHandle		m_Out;

};

class ImageIn : public _2Real::bundle::Block
{

public:

	ImageIn() : Block() {};

	void update();
	void setup( _2Real::bundle::BlockHandle &context );
	void shutdown();

private:

	unsigned int				m_Counter;
	_2Real::bundle::InletHandle			m_In;

};