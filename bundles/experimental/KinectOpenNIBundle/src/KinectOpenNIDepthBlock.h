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
	void					updateImageOutlet();

private:
	InletHandle								m_IsAlignedToColorInletHandle;
	InletHandle								m_Is16BitInletHandle;
	bool									m_bIsAlignedToColor;
	bool									m_bIs16Bit;
	InletHandle								m_IsRealWorldInletHandle;
	bool									m_bIsRealWorld;
};