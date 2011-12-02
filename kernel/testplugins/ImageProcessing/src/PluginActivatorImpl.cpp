#include "PluginActivatorImpl.h"

#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"

#include "ImageAccumulation.h"
#include "ImageConversion.h"

#include <iostream>
#include <fstream>

using namespace _2Real;
using namespace std;

void ImageProcessing::getMetadata(PluginMetadata &info)
{
	MetadataReader reader(info);
	reader.readMetadata();
}

void ImageProcessing::setup(PluginContext &context)
{
	//string logfile = context.getParameterValue< string >("log file");

	//m_Logfile.open(logfile.c_str());

	//if (!m_Logfile.is_open())
	//{
	//	throw PluginException("could not open logfile");
	//}

	//m_Logfile << "image processing: setup called\n";

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

void pocoInitializeLibrary()
{
	std::cout << "PluginLibrary initializing" << std::endl;
}

void pocoUninitializeLibrary()
{
	std::cout << "PluginLibrary uninitializing" << std::endl;
}

POCO_BEGIN_MANIFEST(IPluginActivator)
POCO_EXPORT_CLASS(ImageProcessing)
POCO_END_MANIFEST