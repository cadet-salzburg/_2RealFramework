#pragma once
#include "_2RealBlock.h"

using namespace _2Real::bundle;

class ThroughputBlock : public Block
{
public:
	ThroughputBlock();
	~ThroughputBlock();
	void					shutdown();
	void					update();
	void					setup( BlockHandle &context );

private:

	BlockHandle*		m_pBlockHandle;
};