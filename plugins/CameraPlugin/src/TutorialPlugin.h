#pragma once

#include "_2RealBlock.h"
#include "_2RealOutletHandle.h"

namespace _2Real
{
	class FrameworkContext;
}

class BlockManager : public _2Real::Block
{

public:

	BlockManager() : Block() {}
	~BlockManager() {}
	void update();
	void setup( _2Real::FrameworkContext &context );
	void shutdown();

private:

	_2Real::OutletHandle	m_BundleEnum;
	_2Real::OutletHandle	m_BundleVec;

};