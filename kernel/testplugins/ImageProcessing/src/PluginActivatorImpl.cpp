#include "PluginActivatorImpl.h"

#include "_2RealPluginContext.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"

#include "ImageAccumulation.h"
#include "ImageConversion.h"

#include "_2RealImagebuffer.h"

#include <fstream>

using namespace _2Real;
using namespace std;

void ImageProcessing::getMetadata(Metadata &metadata)
{
	try
	{
		metadata.setDescription("This testplugin transcends time and space.");
		metadata.setAuthor("The Jabberwocky");
		metadata.setVersion(1, 1, 1);
		metadata.setContact("the author is dead.");
		metadata.addSetupParameter< string >("logfile");

		metadata.addService("ImageAccumulation");
		metadata.setDescription("ImageAccumulation", "life is wonderful");
		metadata.addSetupParameter< unsigned char >("ImageAccumulation", "buffer size");
		metadata.addInputSlot< Pixelbuffer < float > >("ImageAccumulation", "input image");
		metadata.addOutputSlot< Pixelbuffer < float > >("ImageAccumulation", "output image");

		metadata.addService("ImageAccumulation_uchar");
		metadata.setDescription("ImageAccumulation_uchar", "life is wonderful");
		metadata.addSetupParameter< unsigned char >("ImageAccumulation_uchar", "buffer size");
		metadata.addInputSlot< Pixelbuffer < unsigned char > >("ImageAccumulation_uchar", "input image");
		metadata.addOutputSlot< Pixelbuffer < float > >("ImageAccumulation_uchar", "output image");
	}
	catch (_2Real::Exception &e)
	{
		m_Logfile << e.message() << endl;
		e.rethrow();
	}
	catch (std::exception &e)
	{
		m_Logfile << e.what() << endl;
		std::ostringstream msg;
		msg << "exception during setup: " << e.what();
		throw PluginException(msg.str());
	}
}

void ImageProcessing::setup(PluginContext &context)
{
	try
	{
		string logfile = context.getParameterValue< string >("logfile");
		m_Logfile.open(logfile.c_str());
		if (!m_Logfile.is_open())
		{
			throw PluginException("could not open logfile");
		}

		m_Logfile << "image processing: setup called" << endl;

		context.registerService("ImageAccumulation_uchar", ::createImageAccumulation< unsigned char >);
		context.registerService("ImageAccumulation", ::createImageAccumulation< float >);
		//context.registerService("ToFloat_uchar", ::createFloatConversion< unsigned char >);

		m_Logfile << "image processing: setup completed" << endl;
	}
	catch (_2Real::Exception &e)
	{
		m_Logfile << e.message() << endl;
		e.rethrow();
	}
	catch (std::exception &e)
	{
		m_Logfile << e.what() << endl;
		std::ostringstream msg;
		msg << "exception during setup: " << e.what();
		throw PluginException(msg.str());
	}
}

void ImageProcessing::shutdown()
{
	if (m_Logfile.is_open())
	{
		m_Logfile << "image processing: shutdown" << endl;
		m_Logfile.close();
	}
}

ImageProcessing::~ImageProcessing()
{
}

_2REAL_EXPORT_PLUGIN(ImageProcessing)