#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealOutputHandle.h"
#include "_2RealInputHandle.h"

#include "_2RealKinect.h"

#include "_2RealPixelbuffer.h"

_2Real::IService *const createImageService();

class ImageService : public _2Real::IService
{

public:

	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~ImageService() {}

private:

	_2Real::_2RealKinect		*m_2RealKinect;
	_2Real::_2RealGenerator		m_Generator;
	unsigned int				m_DeviceId;
	_2Real::OutputHandle		m_Output;
	unsigned int				m_Channels;
	unsigned int				m_Bpp;

};