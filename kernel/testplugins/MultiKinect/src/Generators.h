#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"

#include "ImageBuffer.h"

#include "D:\cadet\trunk\projects\_2RealKinectWrapper\include\_2RealKinect.h"

_2Real::IService *const createImageService();

class ImageService : public _2Real::IService
{

public:

	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext *const context);
	~ImageService() {}

private:

	_2Real::_2RealKinect		*m_2RealKinect;
	_2Real::_2RealGenerator		m_Generator;
	unsigned int				m_DeviceId;
	ImageBuffer					m_Output;

};