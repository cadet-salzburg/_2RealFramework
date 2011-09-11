#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"

#include "RandomValues.h"
#include "Image.h"

#include <iostream>

/*
	factory method for random image service; registered in service factory of framework
*/

template< typename T >
static _2Real::IService *const createRandomImage(void)
{
	_2Real::IService *service = new RandomImageService< T >();
	return service;
}

/*
	random image service
*/

template< typename T >
class RandomImageService : public _2Real::IService
{

public:

	void shutdown() throw(...) {}
	void update() throw(...);
	void setup(_2Real::ServiceContext *const _context) throw(...);

private:

	::Image< T, 2 >			m_OutputImage;			//the resulting image, output param
	unsigned int			m_iImageWidth;			//desired width of result image, setup param
	unsigned int			m_iImageHeight;			//desired height of result image, setup param

};

template< typename T >
void RandomImageService< T >::setup(_2Real::ServiceContext *const _context)
{
	/*
		register all setup parameters, input & output variables as defined in the service metadata
	*/
	try
	{
		_context->getSetupParameter< unsigned int >("image width", m_iImageWidth);
		_context->getSetupParameter< unsigned int >("image height", m_iImageHeight);
		_context->registerOutputSlot< ::Image< T, 2 > >("output image", m_OutputImage);
	}
	catch(Exception &e)
	{
		throw e;
	}

}

template< typename T >
void RandomImageService< T >::update()
{
	/*
		this function performs the actual service
	*/
	try
	{
		unsigned int sz = m_iImageHeight*m_iImageWidth;
		
		T* tmp = new T[sz];

		for (unsigned int y=0; y<m_iImageHeight; y++)
		{
			for (unsigned int x=0; x<m_iImageWidth; x++)
			{
				unsigned int i = y*m_iImageWidth + x;
				tmp[i] = random_t< T >();
				//if (x < 1 && y < 1)
				//{
				//	std::cout << "random value: " << tmp[i] << std::endl;
				//}
			}
		}

		Image< T, 2 >::Resolution res;
		res.set(0, m_iImageWidth);
		res.set(1, m_iImageHeight);

		m_OutputImage.setData(tmp);
		m_OutputImage.setResolution(res);
	}
	catch (Exception &e)
	{
		throw e;
	}
};