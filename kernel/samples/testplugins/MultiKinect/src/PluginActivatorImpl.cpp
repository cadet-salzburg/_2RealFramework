#include "PluginActivatorImpl.h"

#include "_2RealIPluginActivator.h"
#include "_2RealPluginContext.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"

#include "Generators.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace _2Real;
using namespace std;

MultiKinectOpenNI::MultiKinectOpenNI() :
	m_2RealKinect(NULL)
{
}

MultiKinectOpenNI::~MultiKinectOpenNI()
{
}

void MultiKinectOpenNI::shutdown()
{
	if (m_2RealKinect)
	{
		m_2RealKinect->shutdown();
	}
}

void MultiKinectOpenNI::getMetadata(Metadata &info)
{
	try
	{
		info.setAuthor("Azatoth, Chicken of Insanity");
		info.setDescription("test plugin based on 2real kinect wrapper. it sucks.");
		info.setContact("the other side of the moon");
		info.setVersion(100, 1, 5000);

		info.addSetupParameter< vector < string > >("generator flags");
		info.addSetupParameter< bool >("align color depth", false);
		info.addSetupParameter< vector < string > >("image flags");
		info.addSetupParameter< string >("logfile");
		info.addSetupParameter< string >("loglevel");

		info.addService("Image Generator");
		info.setDescription("Image Generator", "generic image generator - sends out a pixelbuffer of unsigned chars containing the current image");

		info.addSetupParameter< unsigned int >("Image Generator", "device id", 0);
		info.addSetupParameter< string >("Image Generator", "image type", "color");
		info.addOutputSlot< Pixelbuffer < unsigned char > >("Image Generator", "output image");
	}
	catch (XMLFormatException &e)
	{
		cout << e.message() << endl;
		throw PluginException(e.message());
	}
	catch (InvalidTypeException &e)
	{
		cout << e.message() << endl;
		throw PluginException(e.message());
	}
	catch (...)
	{
		throw PluginException("unexpected error in getMetadata");
	}
}

void MultiKinectOpenNI::setup(PluginContext &context)
{
	try
	{
		getGeneratorFlags(context);
		getImageFlags(context);
		getLogSettings(context);

		m_2RealKinect = _2RealKinect::getInstance();
	
		m_2RealKinect->setLogLevel(m_LogLevel);
		if (!m_LogPath.empty())
		{
			m_LogStream.open(m_LogPath);
			m_2RealKinect->setLogOutputStream(&m_LogStream);
		}

		context.registerService("Image Generator", ::createImageService);

		m_2RealKinect->start(m_GeneratorFlags, m_ImageFlags);
	}
	catch (...)
	{
		throw PluginException("unexpected error in setup");
	}
}

void MultiKinectOpenNI::getGeneratorFlags(PluginContext &context)
{
	m_AlignColorDepth = context.getParameterValue< bool >("align color depth");
	vector< string > flags = context.getParameterValue< vector < string > >("generator flags");

	m_GeneratorFlags = 0;

	unsigned int depth = 0;
	for (vector< string >::iterator it = flags.begin(); it != flags.end(); it++)
	{
		if (*it == "depth")
		{
			depth = ::DEPTHIMAGE;
			break;
		}
	}

	unsigned int color = 0;
	for (vector< string >::iterator it = flags.begin(); it != flags.end(); it++)
	{
		if (*it == "color")
		{
			color = ::COLORIMAGE;
			break;
		}
		else if (*it == "infrared")
		{
			color = ::INFRAREDIMAGE;
			break;
		}
	}

	unsigned int user = 0;
	for (vector< string >::iterator it = flags.begin(); it != flags.end(); it++)
	{
		if (*it == "user id")
		{
			user = ::USERIMAGE;
			break;
		}
		else if (*it == "user colored")
		{
			user = ::USERIMAGE_COLORED;
			break;
		}
	}

	m_GeneratorFlags = depth | color | user;
}

void MultiKinectOpenNI::getImageFlags(PluginContext &context)
{
	vector< string > flags = context.getParameterValue< vector < string > >("image flags");

	m_ImageFlags = 0;

	unsigned int depth = 0;
	for (vector< string >::iterator it = flags.begin(); it != flags.end(); it++)
	{
		if (*it == "depth640X480")
		{
			depth = ::IMAGE_USER_DEPTH_640X480;
			break;
		}
		else
		{
			depth = ::IMAGE_USER_DEPTH_640X480;
			break;
		}
	}

	unsigned int color = 0;
	for (vector< string >::iterator it = flags.begin(); it != flags.end(); it++)
	{
		if (*it == "color640X480")
		{
			depth = ::IMAGE_COLOR_640X480;
			break;
		}
		else
		{
			depth = ::IMAGE_COLOR_640X480;
			break;
		}
	}

	m_ImageFlags = depth | color;
}

void MultiKinectOpenNI::getLogSettings(PluginContext &context)
{
	m_LogPath = string();

	m_LogPath = context.getParameterValue< string >("logfile");
	string level = context.getParameterValue< string >("loglevel");

	if (level == "none")
	{
		m_LogLevel = none;
	}
	else if (level == "error")
	{
		m_LogLevel = error;
	}
	else if (level == "warn")
	{
		m_LogLevel = warn;
	}
	else if (level == "info")
	{
		m_LogLevel = info;
	}
	else if (level == "debug")
	{
		m_LogLevel = debug;
	}
	else
	{
		throw PluginException("invalid parameter - log level");
	}
}

_2REAL_EXPORT_PLUGIN(MultiKinectOpenNI)