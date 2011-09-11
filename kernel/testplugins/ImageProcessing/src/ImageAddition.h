#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"

#include "Image.h"

#include <iostream>

template< typename T >
static _2Real::IService *const createImageAddition(void)
{
	_2Real::IService *service = new ImageAdditionService< T >();
	return service;
}

template< typename T >
class ImageAdditionService : public _2Real::IService
{

public:

	void shutdown() throw(...) {}
	void update() throw(...);
	void setup(_2Real::ServiceContext *const _context) throw(...);
	~ImageAdditionService() {}

private:

	T										m_ScaleFactor1;			//scale factor for input image 1
	T										m_ScaleFactor2;			//scale factor for input image 2
	::Image< T, 2 >							m_InputImage1;			//stores input image 1, input slot
	::Image< T, 2 >							m_InputImage2;			//stores input image 2, input slot
	::Image< T, 2 >							m_OutputImage;			//the resulting image, output slot
};

template< typename T >
void ImageAdditionService< T >::setup(_2Real::ServiceContext *const _context)
{
	//register all setup parameters, input & output variables as defined in the service metadata
	try
	{
		_context->getSetupParameter< T >("scale factor 1", m_ScaleFactor1);
		_context->getSetupParameter< T >("scale factor 2", m_ScaleFactor2);
		_context->registerInputSlot< ::Image< T, 2> >("input image 1", m_InputImage1);
		_context->registerInputSlot< ::Image< T, 2> >("input image 2", m_InputImage2);
		_context->registerOutputSlot< ::Image< T, 2> >("output image", m_OutputImage);
	}
	catch (Exception &e)
	{
		throw e;
	}
};

template< typename T >
void ImageAdditionService< T >::update()
{
	try
	{
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
			throw Exception("service update: invalid input data");
		}
	}
	catch (Exception &e)
	{
		throw e;
	}
	catch (...)
	{
		throw Exception("error on image addition update");
	}
};