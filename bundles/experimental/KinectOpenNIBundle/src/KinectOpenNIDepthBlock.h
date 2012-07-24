#pragma once
#include "KinectOpenNIBlockBase.h"

using namespace _2Real::bundle;

class KinectOpenNIDepthBlock : public KinectOpenNIBlockBase
{
public:
	KinectOpenNIDepthBlock( ContextBlock & context ) : KinectOpenNIBlockBase(context)
	{
	};
	
	void					setup( BlockHandle &context );
	void					update();
	virtual void			getGeneratorData();

private:
	InletHandle								m_IsAlignedToColorInletHandle;
	bool									m_bIsAlignedToColor;
};