#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"

#include "Image.h"

#include <iostream>

/*
	factory method for image addition service; registered in service factory of framework
*/

template< typename T >
static _2Real::IService *const createImageAddition(void)
{
	_2Real::IService *service = new ImageAdditionService< T >();
	return service;
}

/*
	image addition service
*/

template< typename T >
class ImageAdditionService : public _2Real::IService
{

public:

	void shutdown() throw(...) {}
	void update() throw(...);
	void setup(_2Real::ServiceContext *const _context) throw(...);

private:

	T										m_ScaleFactor1;			//scale factor for input image 1
	T										m_ScaleFactor2;			//scale factor for input image 2
	::Image< T, 2 >							m_InputImage1;			//stores input image 1, input param
	::Image< T, 2 >							m_InputImage2;			//stores input image 2, input param
	::Image< T, 2 >							m_OutputImage;			//the resulting image, output param
};

template< typename T >
void ImageAdditionService< T >::setup(_2Real::ServiceContext *const _context) throw(...)
{
	/*
		register all setup parameters, input & output variables as defined in the service metadata
	*/
	try
	{
		//std::cout << "IMAGE ADDITION SETUP" << std::endl;

		_context->getSetupParameter< T >("scale factor 1", m_ScaleFactor1);

		//std::cout << "IMAGE ADDITION SETUP: scale factor 1 = " << m_ScaleFactor1 << std::endl;

		_context->getSetupParameter< T >("scale factor 2", m_ScaleFactor2);

		//std::cout << "IMAGE ADDITION SETUP: scale factor 2 = " << m_ScaleFactor2 << std::endl;

		_context->registerInputSlot< ::Image< T, 2> >("input image 1", m_InputImage1);
		_context->registerInputSlot< ::Image< T, 2> >("input image 2", m_InputImage2);
		_context->registerOutputSlot< ::Image< T, 2> >("output image", m_OutputImage);

		std::cout << "IMAGE ADDITION SETUP: success" << std::endl;
	}
	catch (...)
	{
		std::cout << "IMAGE ADDITION SETUP: error" << std::endl;
		throw;
	}
};

template< typename T >
void ImageAdditionService< T >::update() throw(...)
{
	/*
		this function performs the actual service
	*/
	try
	{
		//std::cout << "IMAGE ADDITION UPDATE" << std::endl;

		if (m_InputImage1.data() != NULL && m_InputImage2.data() != NULL)
		{
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
					//if (x < 1 && y < 1)
					//{
					//	std::cout << m_InputImage1.data()[i] << " " << m_InputImage2.data()[i] << " " << tmp[i] << std::endl;
					//}
				}
			}

			Image< T, 2 >::Resolution res;
			res.set(0, width);
			res.set(1, height);

			m_OutputImage.setData(tmp);
			m_OutputImage.setResolution(res);

			//std::cout << "IMAGE ADDITION UPDATE: success" << std::endl;
		}
		else
		{
			std::cout << "IMAGE ADDITION UPDATE: at least one image is empty" << std::endl;
			throw Exception::failure();
		}
	}
	catch (...)
	{
		std::cout << "IMAGE ADDITION UPDATE: error" << std::endl;
		throw;
	}
};