#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"

#include "_2RealImagebuffer.h"

#include <iostream>
#include <vector>
#include <deque>

template< typename ImageData >
static _2Real::IService *const createImageAccumulation(void)
{
	_2Real::IService *service = new ImageAccumulationService< ImageData >();
	return service;
}

template< typename ImageData >
class ImageAccumulationService : public _2Real::IService
{

public:

	ImageAccumulationService();
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~ImageAccumulationService();

private:

	_2Real::InputHandle						m_Input;
	_2Real::OutputHandle					m_Output;
	_2Real::Image2D_float 					*m_Image;
	unsigned char							m_BufferSize;
	float									m_Factor;
	std::deque< _2Real::Image2D_float * >	m_Buffer;

};

template< typename ImageData >
ImageAccumulationService< ImageData >::~ImageAccumulationService()
{
	std::deque< _2Real::Image2D_float * >::iterator it;
	for (it = m_Buffer.begin(); it != m_Buffer.end(); it++)
	{
		delete *it;
	}
}

template< typename ImageData >
ImageAccumulationService< ImageData >::ImageAccumulationService() :
	IService(), m_Image(NULL)
{
}

template< typename ImageData >
void ImageAccumulationService< ImageData >::setup(_2Real::ServiceContext &context)
{
	try
	{
		//cout << "ImageAccumulationService: setup " << endl;

		m_Input = context.getInputHandle("input image");
		m_Output = context.getOutputHandle("output image");

		m_BufferSize = context.getParameterValue< unsigned char >("buffer size");
		if (m_BufferSize < 2)
		{
			throw ServiceException("setup parameter \'buffer size\' should at least be 2");
		}

		m_Factor = 1.0f/float(m_BufferSize);

		//cout << "ImageAccumulationService: setup finished" << endl;
	}
	catch (Exception &e)
	{
		throw e;
	}
	catch (...)
	{
		throw ServiceException("ImageAccumulationService: unexpected error in setup");
	}
};

template< typename ImageData >
void ImageAccumulationService< ImageData >::update()
{
	try
	{
		//cout << "ImageAccumulationService: update " << endl;

		Pixelbuffer< ImageData > const& newImage(m_Input.getNewestData< Pixelbuffer < ImageData > >());
		Pixelbuffer< float > & outImage = m_Output.data< Pixelbuffer < float > >();

		unsigned int width = newImage.width();
		unsigned int height = newImage.height();
		unsigned int channels = newImage.channels();
		unsigned int size = newImage.size();

		if (!m_Image)
		{
			m_Image = new Imagebuffer< float >(width, height, channels);
			m_Image->set(0.0f);
		}
		else
		{
			Imagebuffer< float > *first = new Imagebuffer< float >(width, height, channels);
			m_Buffer.push_front(first);

			Imagebuffer< float > *last = NULL;
			if (m_Buffer.size() < m_BufferSize)
			{
				for (unsigned int i=0; i<width; i++)
				{
					for (unsigned int j=0; j<height; j++)
					{
						for (unsigned int k=0; k<channels; k++)
						{
							float value1 = m_Image->operator()(j, i, k);
							float value2 = m_Factor*(float(newImage(j, i, k))/255.0f);
							first->operator()(j, i, k) = value2;
							float result = value1 + value2;
							m_Image->operator()(j, i, k) = result;
						}
					}
				}
			}
			else
			{
				last = m_Buffer.back();
				for (unsigned int i=0; i<width; i++)
				{
					for (unsigned int j=0; j<height; j++)
					{
						for (unsigned int k=0; k<channels; k++)
						{
							float value1 = m_Image->operator()(j, i, k);
							float value2 = m_Factor*(float(newImage(j, i, k))/255.0f);
							first->operator()(j, i, k) = value2;
							float value3 = last->operator()(j, i, k);
							float result = value1 + value2 - value3;
							m_Image->operator()(j, i, k) = result;
						}
					}
				}
				delete last;
				m_Buffer.pop_back();
			}
		}

		outImage.copy(*m_Image);

		//cout << "ImageAccumulationService: update finished " << endl;
	}
	catch (Exception &e)
	{
		throw ServiceException(e.what());
	}
	catch (...)
	{
		throw ServiceException("RunningAverageService: unexpected error in update");
	}
};