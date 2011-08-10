#pragma once

#include "_2RealIUserService.h"
#include "_2RealServiceContext.h"

#include "RandomValues.h"
#include "Image.h"

/*
	factory method for random image service; registered in service factory of framework
*/

template< typename T >
static _2Real::UserServicePtr createRandomImage(void)
{
	_2Real::UserServicePtr service(new RandomImageService< T >());
	return service;
}

/*
	random image service
*/

template< typename T >
class RandomImageService : public _2Real::IUserService
{

public:

	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext *const _context);
	const bool init() { return true; }

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
	_context->registerSetupParameter< unsigned int >("image width", m_iImageWidth);
	_context->registerSetupParameter< unsigned int >("image height", m_iImageHeight);
	_context->registerOutputVariable< ::Image< T, 2 > >("output image", m_OutputImage);
}

template< typename T >
void RandomImageService< T >::update()
{
	/*
		this function performs the actual service
	*/
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
};