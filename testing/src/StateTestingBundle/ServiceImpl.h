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
		std::vector< _2Real::bundle::InletHandle >		inlets;
		std::vector< _2Real::bundle::OutletHandle >		outlets;
		std::vector< _2Real::bundle::ParameterHandle >	parameters;
		_2Real::bundle::BlockHandle block;
	};

	BlockInstance					mInstance;
	unsigned long					mCounter;
	unsigned int					mId;

};