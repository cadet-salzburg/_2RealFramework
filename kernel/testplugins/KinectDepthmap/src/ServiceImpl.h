#pragma once

#include "_2RealIUserService.h"
#include "_2RealServiceContext.h"

#include "KinectDepthMap.h"
#include "Image.h"

class KinectService : public _2Real::IUserService
{

public:

	KinectService() : m_bIsInitialized(false) {}

	void shutdown();
	void update();
	void KinectService::setup(_2Real::ServiceContext *const _context);
	const bool init();

private:

	bool								m_bIsInitialized;
	KinectDepthMap						m_KinectDepthMap;
	::Image< unsigned short, 2 >		m_OutputImage;
	unsigned int						m_iImageWidth;
	unsigned int						m_iImageHeight;

};

/*
	factory function to be registered with the framework
*/
static _2Real::UserServicePtr createKinectService()
{
	_2Real::UserServicePtr service(new KinectService());
	return service;
}
