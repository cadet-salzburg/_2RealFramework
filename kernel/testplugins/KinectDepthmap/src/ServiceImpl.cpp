#include "ServiceImpl.h"

using namespace std;
using namespace _2Real;

void KinectService::setup(ServiceContext *const _context)
{
	_context->registerSetupParameter< unsigned int >("image width", m_iImageWidth);
	_context->registerSetupParameter< unsigned int >("image height", m_iImageHeight);
	_context->registerOutputVariable< ::Image< unsigned short, 2 > >("output image", m_OutputImage);
}

const bool KinectService::init()
{
	if (m_bIsInitialized)
	{
		return false;
	}

	if (!(m_KinectDepthMap.setup(m_iImageWidth, m_iImageHeight)))
	{
		cout << "kinect depth map service: setup failed" << endl;
		return false;
	}
	else
	{
		m_bIsInitialized = true,
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