#pragma once

#include "_2RealBlock.h"
#include "_2RealBundle.h"
#include <vector>

class Test : public _2Real::bundle::Block
{

public:

	Test();
	~Test();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &handle );

private:

	struct BlockInstance
	{
		std::vector< _2Real::bundle::InletHandle > inlets;
		std::vector< _2Real::bundle::OutletHandle > outlets;
		_2Real::bundle::BlockHandle block;
	};

	BlockInstance					mInstance;

	// not pretty, but good enough for testing...
	unsigned int					mNumber;
	unsigned int					mCounter;

};