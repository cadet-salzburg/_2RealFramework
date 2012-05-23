#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"

#include "opencv2/opencv.hpp"

using namespace cv;
class CameraService : public _2Real::Block
{

public:

	CameraService() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );
	~CameraService() {}

private:

	 
	_2Real::OutletHandle			m_CameraData;
	VideoCapture					*m_Capture; 

};
 