#include <iostream>
#include <string>
#include <sstream>
#include "KinectOpenNIUserSkeletonBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

void KinectOpenNIUserSkeletonBlock::setup( BlockHandle &block )
{
	try
	{
		KinectOpenNIDepthBlock::setup(block);
		setGeneratorType(_2RealKinectWrapper::USERIMAGE);
		m_SkeletonsOutletHandle = block.getOutletHandle("Skeletons");
		m_NrOfUsersOutletHandle = block.getOutletHandle("NrOfUsers");
		m_NrOfSkeletonsOutletHandle = block.getOutletHandle("NrOfSkeletons");
		m_iNrOfUsers = m_iNrOfSkeletons = 0;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void KinectOpenNIUserSkeletonBlock::update()
{
	try
	{
		KinectOpenNIDepthBlock::update();

		// get and set nr of detected users
		int iNrOfUsers = m_OpenNIDeviceManager->getNumberOfUsers(m_iCurrentDevice);
		if(iNrOfUsers != m_iNrOfUsers)
		{
			m_iNrOfUsers = iNrOfUsers;
			m_NrOfUsersOutletHandle.getWriteableRef<int>() = m_iNrOfUsers;
		}
		else
		{
			m_NrOfUsersOutletHandle.discard();
		}
		//get and set nr of skeletons
		int iNrOfSkeletons = m_OpenNIDeviceManager->getNumberOfSkeletons(m_iCurrentDevice);
		if(iNrOfSkeletons != m_iNrOfSkeletons)
		{
			m_iNrOfSkeletons = iNrOfSkeletons;
			m_NrOfSkeletonsOutletHandle.getWriteableRef<int>() = m_iNrOfSkeletons;
		}
		else
		{
			m_NrOfSkeletonsOutletHandle.discard();
		}

		m_SkeletonsOutletHandle.getWriteableRef<_2Real::Skeleton>() = m_OpenNIDeviceManager->getSkeletonScreen(m_iCurrentDevice, 0);

	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
