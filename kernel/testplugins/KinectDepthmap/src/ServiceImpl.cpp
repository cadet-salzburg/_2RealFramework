#include "ServiceImpl.h"

using namespace std;
using namespace _2Real;

bool KinectService::setup(ConfigMetadataPtr const& _config)
{
	if (m_bIsInitialized)
	{
		return false;
	}

	bool noError = true;

	unsigned int width, height;
	noError &= _config->setupParameter< unsigned int >("image width", width);
	noError &= _config->setupParameter< unsigned int >("image height", height);

	if (!(noError && m_KinectDepthMap.setup(width, height)))
	{
		cout << "kinect depth map service: setup failed" << endl;
		return false;
	}

	m_bIsInitialized = true;

	/*
		register all setup parameters, input & output variables as defined in the service metadata
	*/
	addOutputVariable< ::Image< unsigned short, 2 > >("output image", m_OutputImage);
	
	/*
		call abstract service -> configure
	*/
	if (!configure(_config))
	{
		shutdown();
		return false;
	}
	
	return true;
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
		std::cout << "kinect update begin" << std::endl;
		m_OutputImage = m_KinectDepthMap.depthMap();
		std::cout << "kinect update complete" << std::endl;
	}
}