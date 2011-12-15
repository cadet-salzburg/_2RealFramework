#include "Generators.h"
#include "_2RealException.h"

#include <iostream>

using namespace _2Real;
using namespace std;

_2Real::IService *const createImageService()
{
	_2Real::IService *service = new ImageService();
	return service;
}

void ImageService::setup(ServiceContext &context)
{
	try
	{
		m_2RealKinect = _2RealKinect::getInstance();

		string const& generator = context.getParameterValue< string >("image type");

		if (generator == "color")
		{
			m_Generator = COLORIMAGE;
		}
		else if (generator == "infrared")
		{
			m_Generator = INFRAREDIMAGE;
		}
		else if (generator == "depth")
		{
			m_Generator = DEPTHIMAGE;
		}
		else if (generator == "user id")
		{
			m_Generator = USERIMAGE;
		}
		else if (generator == "user color")
		{
			m_Generator = USERIMAGE_COLORED;
		}
		else
		{
			throw ServiceException("generator type " + generator + " is invalid");
		}

		cout << "get device id" << endl;

		m_DeviceId = context.getParameterValue< unsigned int >("device id");
		
		m_Output = context.getOutputHandle("output image");

		if (m_Generator == DEPTHIMAGE)
		{
			m_Channels = 1;
		}
		else
		{
			m_Channels = 3;
		}

		m_Bpp = m_Channels*sizeof(unsigned char);

	}
	catch (Exception &e)
	{
		cout << e.message() << endl;
		e.rethrow();
	}
	catch (...)
	{
		cout << "exception" << endl;
		throw ServiceException("unexpected error in setup");
	}
}

void ImageService::update()
{
	try
	{
		m_Output.data< Buffer2D_uchar >().assign(	m_2RealKinect->getImageData(m_DeviceId, m_Generator),
													m_2RealKinect->getImageHeight(m_DeviceId, m_Generator),
													m_2RealKinect->getImageWidth(m_DeviceId, m_Generator),
													m_Channels,
													false);
	}
	catch (TypeMismatchException &e)
	{
		throw e;
	}
	catch (PixelbufferException &e)
	{
		throw e;
	}
	catch (...)
	{
		throw ServiceException("ImageService: unexpected error in update");
	}
}