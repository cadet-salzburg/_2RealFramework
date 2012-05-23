#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"

namespace cv
{
	class VideoCapture;
}

class CameraService : public _2Real::Block
{

public:

	CameraService() {}
	void shutdown();
	void update();
	void setup( _2Real::FrameworkContext &context );
	~CameraService() {}

private:

	_2Real::OutletHandle			m_CameraData;
	cv::VideoCapture				*m_Capture;

};
 