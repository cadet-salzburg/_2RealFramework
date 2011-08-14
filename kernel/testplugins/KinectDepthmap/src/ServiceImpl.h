#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"

#include "KinectDepthMap.h"
#include "Image.h"

class KinectService : public _2Real::IService
{

public:

	KinectService() : m_bIsInitialized(false) {}

	void shutdown() throw(...);
	void update() throw(...);
	void KinectService::setup(_2Real::ServiceContext *const _context) throw(...);

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
static _2Real::IService *const createKinectService()
{
	_2Real::IService *service = new KinectService();
	return service;
}
