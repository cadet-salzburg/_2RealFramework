#include "ServiceImpl.h"

using namespace std;
using namespace _2Real;

const bool KinectService::setup(ServiceContext *const _context)
{
	if (m_bIsInitialized)
	{
		return false;
	}

	bool success = true;

	success &= _context->getSetupParameter< unsigned int >("image width", m_iImageWidth);
	success &= _context->getSetupParameter< unsigned int >("image height", m_iImageHeight);

	if (!success || !(m_KinectDepthMap.setup(m_iImageWidth, m_iImageHeight)))
	{
		cout << "kinect depth map service: setup failed" << endl;
		return false;
	}

	success &= _context->registerOutputVariable< ::Image< unsigned short, 2 > >("output image", m_OutputImage);

	if (success)
	{
		m_bIsInitialized = true;
	}

	return m_bIsInitialized;
}

void KinectService::shutdown()
{
	if (m_bIsInitialized)
	{
		m_KinectDepthMap.shutdown();
	}
}

void KinectService::update()
{
	if (m_bIsInitialized && m_KinectDepthMap.update())
	{
		m_OutputImage = m_KinectDepthMap.depthMap();
	}
}