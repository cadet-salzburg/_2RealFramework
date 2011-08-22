#include "ServiceImpl.h"
#include "_2RealServiceContext.h"

using namespace std;
using namespace _2Real;

void KinectService::setup(ServiceContext *const _context) throw(...)
{
	if (m_bIsInitialized)
	{
		//
	}

	try
	{
		_context->getSetupParameter< unsigned int >("image width", m_iImageWidth);
		_context->getSetupParameter< unsigned int >("image height", m_iImageHeight);
	}
	catch (...)
	{
		//
	}

	if (!(m_KinectDepthMap.setup(m_iImageWidth, m_iImageHeight)))
	{
		//
	}

	try
	{
		_context->registerOutputSlot< ::Image< unsigned short, 2 > >("output image", m_OutputImage);
	}
	catch (...)
	{
		//
	}

	m_bIsInitialized = true;
}

void KinectService::shutdown() throw (...)
{
	if (m_bIsInitialized)
	{
		m_KinectDepthMap.shutdown();
	}
}

void KinectService::update() throw(...)
{
	if (m_bIsInitialized && m_KinectDepthMap.update())
	{
		m_OutputImage = m_KinectDepthMap.depthMap();
	}
}