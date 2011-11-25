#include "OpenCVvideoplayer.h"

#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"
#include "_2RealImagebuffer.h"

#include <stdio.h>

#include <Windows.h>


_2Real::IService *const createservice_playvideo()
{
	_2Real::IService *service = new playvideo();
	return service;
}

void playvideo::setup(_2Real::ServiceContext &context) {
    
	std::cout << "starting setup" << std::endl;
	
    m_capture = cvCaptureFromAVI("stuff.avi");
    int fps = ( int )cvGetCaptureProperty( m_capture, CV_CAP_PROP_FPS );
    std::cout << "fps: " << fps << std::endl; 

    //m_Output = context.getOutputHandle("framenumber");   
	m_framenumber = 0;
	m_OutputImage = context.getOutputHandle("output image");

}
void playvideo::update() {

	// get a frame
    m_frame = cvQueryFrame( m_capture );

    if( m_frame ) 
	{
		m_framenumber++;
		/*
		std::cout << m_frame->width << std::endl;
		std::cout << m_frame->height << std::endl;
		std::cout << m_frame->dataOrder << std::endl;
		*/
		m_OutputImage.data< _2Real::Buffer2D_uchar >().assign(( unsigned char* )m_frame->imageData,
												( unsigned int )m_frame->width,
												( unsigned int )m_frame->height,
												3,
												false);

		//m_Output.data<unsigned int>() = m_framenumber;
	}
}


playvideo::~playvideo() {

	   /* free memory */
    cvReleaseCapture( &m_capture );
    
}