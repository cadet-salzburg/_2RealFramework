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
	}
	catch (_2Real::Exception &e)
	{
		cout << e.message() << endl;
	}
	catch (...)
	{
		cout << "unknwon exception" << endl;
	}
}

void ImageProcessing::setup(PluginContext &context)
{
	try
	{
		std::cout << "setup called" << std::endl;

		string logfile = context.getParameterValue< string >("logfile");

		m_Logfile.open(logfile.c_str());

		if (!m_Logfile.is_open())
		{
		//	throw PluginException("could not open logfile");
		}

		m_Logfile << "image processing: setup called\n";

	////_context.registerService("ImageAccumulation_uchar", ::createImageAccumulation< unsigned char >);
	////_context.registerService("ImageAccumulation", ::createImageAccumulation< float >);
	////_context.registerService("ToFloat_uchar", ::createFloatConversion< unsigned char >);

	//m_Logfile << "image processing: setup completed\n";
	}
	catch (_2Real::Exception &e)
	{
		cout << e.message() << endl;
	}
	catch (...)
	{
		cout << "unknwon exception" << endl;
	}
}

ImageProcessing::~ImageProcessing()
{
	//if (m_Logfile.is_open())
	//{
	//	m_Logfile << "image processing: destructor called\n";
	//	m_Logfile.close();
	//}
}

_2REAL_EXPORT_PLUGIN(ImageProcessing)