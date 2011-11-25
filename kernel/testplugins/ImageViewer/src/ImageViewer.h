#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealImageBuffer.h"
#include "Poco\Mutex.h"
#include <iostream>

//name of plugin "ImageViewer"
//name of service = "displaywindow"

_2Real::IService *const createservice_displaywindow();

class displaywindow : public _2Real::IService
{

public:

	displaywindow() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~displaywindow();

private:

	_2Real::InputHandle					m_InputImage;
	_2Real::OutputHandle				m_Output;
	//_2Real::Image2D_float 			*m_Image;

};
