#pragma once
#include "KinectOpenNIBlockBase.h"

using namespace _2Real::bundle;

class KinectOpenNIUserSkeletonBlock : public KinectOpenNIBlockBase
{
public:
	KinectOpenNIUserSkeletonBlock( ContextBlock & context ) : KinectOpenNIBlockBase(context)
	{
	};
	
	void					setup( BlockHandle &context );
	void					update();

private:
	InletHandle				m_IsAlignedToColorInletHandle;
	InletHandle				m_IsWorldCoordinatesInletHandle;
	OutletHandle			m_SkeletonsOutletHandle;
	OutletHandle			m_NrOfUsersOutletHandle;
	OutletHandle			m_NrOfSkeletonsOutletHandle;
	int						m_iNrOfUsers;
	int						m_iNrOfSkeletons;
	bool					m_bIsAlignedToColor;
	bool					m_bIsWorldCoordinates;
};