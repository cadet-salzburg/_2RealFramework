#pragma once

#include "_2RealBlock.h"

class Test : public _2Real::bundle::Block
{

public:

	Test();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

};