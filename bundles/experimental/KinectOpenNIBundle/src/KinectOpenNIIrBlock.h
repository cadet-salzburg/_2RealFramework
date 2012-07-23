#pragma once
#include "KinectOpenNIBlockBase.h"

using namespace _2Real::bundle;

class KinectOpenNIIrBlock : public KinectOpenNIBlockBase
{
public:
	KinectOpenNIIrBlock( ContextBlock & context ) : KinectOpenNIBlockBase(context)
	{};

	void					setup( BlockHandle &context );

private:
};