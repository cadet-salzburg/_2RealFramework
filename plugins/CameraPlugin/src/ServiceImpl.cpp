#include "ServiceImpl.h"

#include "_2RealFrameworkContext.h"
#include "_2RealException.h"
#include "_2RealImageT.h"

#include "opencv2/opencv.hpp"

#include <iostream>
#include <string>

using _2Real::FrameworkContext;
using _2Real::Exception;
using _2Real::ImageT;
using _2Real::ImageChannelOrder;

using std::cout;
using std::endl;
using std::string;

using cv::Mat;
using cv::VideoCapture;

void CameraService::setup( FrameworkContext &context )
{
	try
	{
		m_CameraData = context.getOutletHandle( "s1 outlet" );
		m_Capture = new VideoCapture(0);
		if(!m_Capture->isOpened())
		{
			throw Exception( "could not open device" );
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void CameraService::update()
{
	try
	{
		Mat frame;
		*m_Capture >> frame;

		const unsigned int width = frame.size().width;
		const unsigned int height = frame.size().height;
		const unsigned int area = frame.size().area();
		const unsigned int channels = frame.channels();

		const unsigned int sz = area * channels;
		unsigned char *copy = new unsigned char[ sz ];
		memcpy( copy, frame.data, sz*sizeof( unsigned char ) );

		ImageT< unsigned char > frameImg( copy, true, width, height, ImageChannelOrder::RGB );
		m_CameraData.data< ImageT< unsigned char > >() = frameImg;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void CameraService::shutdown()
{
	m_Capture->release();
	delete m_Capture;
}
