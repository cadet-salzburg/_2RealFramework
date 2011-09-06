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
void RandomImageService< T >::setup(_2Real::ServiceContext *const _context) throw(...)
{
	/*
		register all setup parameters, input & output variables as defined in the service metadata
	*/
	try
	{
		std::cout << "RANDOM IMAGE SETUP" << std::endl;

		_context->getSetupParameter< unsigned int >("image width", m_iImageWidth);

		std::cout << "RANDOM IMAGE SETUP: width = " << m_iImageWidth << std::endl;

		_context->getSetupParameter< unsigned int >("image height", m_iImageHeight);

		std::cout << "RANDOM IMAGE SETUP: height = " << m_iImageHeight << std::endl;

		_context->registerOutputSlot< ::Image< T, 2 > >("output image", m_OutputImage);

		std::cout << "RANDOM IMAGE SETUP: success" << std::endl;
	}
	catch(...)
	{
		std::cout << "RANDOM IMAGE SETUP: ERROR " << std::endl;
		throw;
	}

}

template< typename T >
void RandomImageService< T >::update() throw(...)
{
	/*
		this function performs the actual service
	*/
	try
	{
		std::cout << "RANDOM IMAGE UPDATE" << std::endl;
		
		unsigned int sz = m_iImageHeight*m_iImageWidth;
		
		T* tmp = new T[sz];

		for (unsigned int y=0; y<m_iImageHeight; y++)
		{
			for (unsigned int x=0; x<m_iImageWidth; x++)
			{
				unsigned int i = y*m_iImageWidth + x;
				tmp[i] = random_t< T >();
			}
		}

		Image< T, 2 >::Resolution res;
		res.set(0, m_iImageWidth);
		res.set(1, m_iImageHeight);

		m_OutputImage.setData(tmp);
		m_OutputImage.setResolution(res);

		std::cout << "RANDOM IMAGE UPDATE: success" << std::endl;
	}
	catch (...)
	{
		std::cout << "RANDOM IMAGE UPDATE: error" << std::endl;
		throw;
	}
};