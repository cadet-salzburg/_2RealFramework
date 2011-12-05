#include "PluginActivatorImpl.h"

#include "_2RealPluginContext.h"
#include "_2RealMetadata.h"

#include "ImageAccumulation.h"
#include "ImageConversion.h"

#include "_2RealImagebuffer.h"

#include <fstream>

using namespace _2Real;
using namespace std;

void ImageProcessing::getMetadata(Metadata &metadata)
{
	metadata.setDescription("This testplugin transcends time and space.");
	metadata.setAuthor("The Jabberwocky");
	metadata.setVersion(1, 1, 1);
	metadata.setContact("the author is dead.");
	metadata.addSetupParameter< string >("logfile");

	metadata.addService("ImageAccumulation");
	metadata.setDescription("ImageAccumulation", "...");
	metadata.addSetupParameter< unsigned char >("buffer size");
	metadata.addInputSlot< Pixelbuffer < float > >("ImageAccumulation", "input image");
	metadata.addOutputSlot< Pixelbuffer < float > >("ImageAccumulation", "output image");
}

void ImageProcessing::setup(PluginContext &context)
{
	std::cout << "setup called" << std::endl;

	string logfile = context.getParameterValue< string >("log file");

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

ImageProcessing::~ImageProcessing()
{
	//if (m_Logfile.is_open())
	//{
	//	m_Logfile << "image processing: destructor called\n";
	//	m_Logfile.close();
	//}
}

_2REAL_EXPORT_PLUGIN(ImageProcessing)