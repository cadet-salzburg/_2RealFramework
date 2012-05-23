#include "ServiceImpl.h"

#include "_2RealFrameworkContext.h"
#include "_2RealException.h"
#include "_2RealEnum.h"
#include "_2RealImageT.h"

#include <iostream>
#include <vector>
#include <string>





using _2Real::FrameworkContext;
using _2Real::Exception;
using _2Real::ImageT;

using std::cout;
using std::endl;
using std::vector;
using std::string;

using namespace cv;

void CameraService::setup( FrameworkContext &context )
{
	try
	{
		 
		m_CameraData = context.getOutletHandle( "s1 outlet" );
		m_Capture = new VideoCapture(0); // open the default camera
		if(!m_Capture->isOpened())  // check if we succeeded
			cout<<" Camera is not opened"<<endl;

	}
	catch ( _2Real::Exception &e )
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
		*m_Capture>>frame; // get a new frame from camera
	//	 imshow("frame", frame);

		unsigned char * cam_data = frame.data;

		//cout<<" camera data"<< cam_data[1]<<endl;
		unsigned char *copy = new unsigned char[640 * 480];
		memcpy(copy, cam_data, 640*480*sizeof(unsigned char));
	//	cout<<" copy data= "<< copy[0]<<endl;
		m_CameraData.data< ImageT<unsigned char> >() =ImageT< unsigned char >(copy, true, 640, 480, _2Real::ImageChannelOrder::RGB);
			
	 
	}
	catch ( _2Real::Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};
 