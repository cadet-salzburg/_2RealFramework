#include "Generators.h"

using namespace _2Real;
using namespace std;

#include <iostream>

_2Real::IService *const createImageService()
{
	_2Real::IService *service = new ImageService();
	return service;
}

void ImageService::setup(ServiceContext *const context)
{
	try
	{
		m_2RealKinect = _2RealKinect::getInstance();

		string generator;
		context->getSetupParameter("image type", generator);

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
			throw Exception("generator type " + generator + " is invalid");
		}

		context->getSetupParameter("device id", m_DeviceId);
		context->registerOutputSlot("output", m_Output);
	}
	catch (Exception &e)
	{
		string exc = e.what();
		throw Exception(exc + "depth image service - error in setup()");
	}
	catch (...)
	{
		throw Exception("depth image service - error in setup()");
	}
}

void ImageService::update()
{
	try
	{
		m_Output.buffer = m_2RealKinect->getImageData(m_DeviceId, m_Generator);
		m_Output.height = m_2RealKinect->getImageHeight(m_DeviceId, m_Generator);
		m_Output.width = m_2RealKinect->getImageWidth(m_DeviceId, m_Generator);
	}
	catch (...)
	{
		throw Exception("depth image service - error in update()");
	}
}