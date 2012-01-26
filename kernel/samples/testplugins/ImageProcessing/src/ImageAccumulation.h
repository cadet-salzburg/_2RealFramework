#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"

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
	float				 					*m_Image;
	unsigned int							m_BufferSize;
	float									m_Factor;
	std::deque< float * >					m_Buffer;

	_2Real::InputHandle						m_Test;

};

template< typename ImageData >
ImageAccumulationService< ImageData >::~ImageAccumulationService()
{
	std::deque< float * >::iterator it;
	for (it = m_Buffer.begin(); it != m_Buffer.end(); it++)
	{
		delete [] *it;
	}

	delete [] m_Image;
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
		m_Input = context.getInputHandle("input image");
		m_Test = context.getInputHandle("test input");
		m_Output = context.getOutputHandle("output image");

		m_BufferSize = context.getParameterValue< unsigned int >("buffer size");
		if (m_BufferSize < 2)
		{
			throw ServiceException("setup parameter \'buffer size\' should at least be 2");
		}
		m_Factor = 1.0f/float(m_BufferSize);
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
		Pixelbuffer< ImageData > const& newImage = m_Input.data< Pixelbuffer < ImageData > >();
		//the output is always in floating point format
		Pixelbuffer< float > & outImage = m_Output.data< Pixelbuffer < float > >();

		unsigned int width = newImage.width();
		unsigned int height = newImage.height();
		unsigned int channels = newImage.channels();

		if (!m_Image)
		{
			unsigned int size = width * height * channels;
			m_Image = new float[size];
			float *ie = new float[size];

			for (unsigned int i=0; i<size; i++)
			{
				m_Image[i] = 0.0f;
			}
		}
		else
		{
			unsigned int size = width * height * channels;
			float *first = new float[width * height * channels];
			m_Buffer.push_front(first);
			float *last = NULL;

			if (m_Buffer.size() < m_BufferSize)
			{
				for (unsigned int i=0; i<size; i++)
				{
					float value1 = m_Image[i];
					float value2 = m_Factor*(float(newImage[i])/255.0f);
					first[i] = value2;
					float result = value1 + value2;
					m_Image[i] = result;
				}
			}
			else
			{
				last = m_Buffer.back();

				for (unsigned int i=0; i<size; i++)
				{
					float value1 = m_Image[i];
					float value2 = m_Factor*(float(newImage[i])/255.0f);
					float value3 = last[i];
					first[i] = value2;
					float result = value1 + value2 - value3;
					m_Image[i] = result;
				}

				delete last;
				m_Buffer.pop_back();
			}
		}

		outImage.assign(m_Image, width, height, channels, false);
	}
	catch (Exception &e)
	{
		throw e;
	}
	catch (...)
	{
		throw ServiceException("RunningAverageService: unexpected error in update");
	}
};