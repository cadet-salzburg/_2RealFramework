#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"

#include "RandomValues.h"
#include "Image.h"

#include <iostream>

template< typename T >
static _2Real::IService *const createImageScaling(void)
{
	_2Real::IService *service = new ImageScalingService< T >();
	return service;
}

template< typename T >
class ImageScalingService : public _2Real::IService
{

public:

	void shutdown() throw(...) {}
	void update() throw(...);
	void setup(_2Real::ServiceContext *const _context) throw(...);

private:

	::Image< T, 2 >			m_InputImage;			//input image
	::Image< T, 2 >			m_OutputImage;			//output image
	T						m_Scale;				//setup param

};

template< typename T >
void ImageScalingService< T >::setup(_2Real::ServiceContext *const _context)
{
	/*
		register all setup parameters, input & output variables as defined in the service metadata
	*/
	try
	{
		_context->getSetupParameter< T >("scale factor", m_Scale);
		_context->registerInputSlot< ::Image< T, 2 > >("input image", m_InputImage);
		_context->registerOutputSlot< ::Image< T, 2 > >("output image", m_OutputImage);
	}
	catch(Exception &e)
	{
		throw e;
	}

}

template< typename T >
void ImageScalingService< T >::update()
{
	try
	{
		if (m_InputImage.data() != NULL)
		{
			unsigned int width = m_InputImage.resolution().get(0);
			unsigned int height = m_InputImage.resolution().get(1);

			unsigned int sz = width*height;

			T* tmp = new T[sz];
			for (unsigned int y=0; y<height; y++)
			{
				for (unsigned int x=0; x<width; x++)
				{
					unsigned int i = y*width + x;
					tmp[i] = m_Scale*m_InputImage.data()[i];
					//if (x < 1 && y < 1)
					//{
					//	std::cout << "scaling: " << m_Scale << " input " << m_InputImage.data()[i] << "output " << tmp[i] << std::endl;
					//}
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
};