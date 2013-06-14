#pragma once

#include "_2RealBlock.h"

class Test : public _2Real::bundle::Block
{

public:

	Test();
	~Test();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

private:

	_2Real::bundle::BlockHandle		mHandle;
	unsigned int					mNumber;
	unsigned int					mCounter;

};