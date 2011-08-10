#pragma once

#include "_2RealIUserService.h"
#include "_2RealServiceContext.h"

#include "Image.h"

/*
	factory method for image addition service; registered in service factory of framework
*/

template< typename T >
static _2Real::UserServicePtr createImageAddition(void)
{
	_2Real::UserServicePtr service(new ImageAdditionService< T >());
	return service;
}

/*
	image addition service
*/

template< typename T >
class ImageAdditionService : public _2Real::IUserService
{

public:

	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext *const _context);
	const bool init() { return true; }

private:

	T										m_ScaleFactor1;			//scale factor for input image 1
	T										m_ScaleFactor2;			//scale factor for input image 2
	::Image< T, 2 >							m_InputImage1;			//stores input image 1, input param
	::Image< T, 2 >							m_InputImage2;			//stores input image 2, input param
	::Image< T, 2 >							m_OutputImage;			//the resulting image, output param
};

template< typename T >
void ImageAdditionService< T >::setup(_2Real::ServiceContext *const _context)
{
	/*
		register all setup parameters, input & output variables as defined in the service metadata
	*/
	_context->registerSetupParameter< T >("scale factor 1", m_ScaleFactor1);
	_context->registerSetupParameter< T >("scale factor 2", m_ScaleFactor2);
	_context->registerInputVariable< ::Image< T, 2> >("input image 1", m_InputImage1);
	_context->registerInputVariable< ::Image< T, 2> >("input image 2", m_InputImage2);
	_context->registerOutputVariable< ::Image< T, 2> >("output image", m_OutputImage);
};

template< typename T >
void ImageAdditionService< T >::update()
{
	/*
		this function performs the actual service
	*/

	std::cout << "begin of: image addition update" << std::endl;

	if (m_InputImage1.data() != NULL && m_InputImage2.data() != NULL)
	{

		std::cout << "both input images are not empty" << std::endl;
		
		unsigned int width = m_InputImage1.resolution().get(0);
		unsigned int height = m_InputImage1.resolution().get(1);

		unsigned int sz = width*height;
		
		T* tmp = new T[sz];
		for (unsigned int y=0; y<height; y++)
		{
			for (unsigned int x=0; x<width; x++)
			{
				unsigned int i = y*width + x;
				tmp[i] = m_ScaleFactor1*m_InputImage1.data()[i] + m_ScaleFactor2*m_InputImage2.data()[i];
			}
		}

		Image< T, 2 >::Resolution res;
		res.set(0, width);
		res.set(1, height);

		m_OutputImage.setData(tmp);
		m_OutputImage.setResolution(res);
	}
	else
	{
		std::cout << "at least one input image is empty" << std::endl;
	}

	std::cout << "image addition update complete" << std::endl;
};