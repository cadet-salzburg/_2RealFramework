#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealImageBuffer.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>

//name of plugin "OpenCVvideoplayer"
//name of service = "playvideo"
_2Real::IService *const createservice_playvideo();

class playvideo : public _2Real::IService
{

public:

	playvideo() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~playvideo();

private:

	//_2Real::InputHandle					m_Input;
	//_2Real::OutputHandle				m_Output;
	_2Real::OutputHandle				m_OutputImage;
	//_2Real::Image2D_float 			*m_Image;
	unsigned int						m_framenumber;
	IplImage							*m_frame;
	CvCapture				*m_capture;
	int									m_Window;

};
