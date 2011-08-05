#pragma once

#include "_2RealAbstractService.h"

#include "KinectDepthMap.h"
#include "Image.h"

class KinectService : public _2Real::AbstractService
{

public:

	KinectService() : m_bIsInitialized(false) {}

	void shutdown();
	void update();
	bool KinectService::setup(_2Real::ConfigMetadataPtr const& _config);

private:

	bool								m_bIsInitialized;
	KinectDepthMap						m_KinectDepthMap;
	::Image< unsigned short, 2 >		m_OutputImage;

};

/*
	factory function to be registered with the framework
*/
static _2Real::ServicePtr createKinectService()
{
	_2Real::ServicePtr service(new KinectService());
	return service;
}
