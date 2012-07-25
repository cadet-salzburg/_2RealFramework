#pragma once
#include "KinectOpenNIDepthBlock.h"

using namespace _2Real::bundle;

class KinectOpenNIUserSkeletonBlock : public KinectOpenNIDepthBlock
{
public:
	KinectOpenNIUserSkeletonBlock( ContextBlock & context ) : KinectOpenNIDepthBlock(context)
	{
	};
	
	void					setup( BlockHandle &context );
	void					update();

private:
	OutletHandle			m_NrOfUsersOutletHandle;
	OutletHandle			m_NrOfSkeletonsOutletHandle;
	int						m_iNrOfUsers;
	int						m_iNrOfSkeletons;
};