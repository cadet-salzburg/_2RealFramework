#pragma once
#include "KinectOpenNIBlockBase.h"

using namespace _2Real::bundle;

class KinectOpenNIRgbBlock : public KinectOpenNIBlockBase
{
public:
	KinectOpenNIRgbBlock( ContextBlock & context ) : KinectOpenNIBlockBase(context)
	{};

	void					setup( BlockHandle &context );

private:
};