#pragma once
#include "_2RealBlock.h"

using namespace _2Real::bundle;

class MultiplierBlock : public Block
{
public:
	MultiplierBlock();
	~MultiplierBlock();
	void					shutdown();
	void					update();
	void					setup( BlockHandle &context );

//private:
//
//	InletHandle							m_NumberAInletHandle;
//	InletHandle							m_NumberBInletHandle;
//	OutletHandle						m_ResultOutletHandle;
};