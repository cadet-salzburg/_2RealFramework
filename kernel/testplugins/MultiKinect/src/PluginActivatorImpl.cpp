#include "_2RealIPluginActivator.h"
#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"

#include "Generators.h"

#ifdef _WINSDK
	#define TARGET_MSKINECTSDK
#endif


#include <iostream>
#include <fstream>
#include <vector>

using namespace _2Real;
using namespace std;

#ifdef _WINSDK
	class MultiKinectWinSdk : public IPluginActivator
#elif _OPENNI
	class MultiKinectOpenNI : public IPluginActivator
#endif
{

public:

#ifdef _WINSDK
	~MultiKinectWinSdk();
#elif _OPENNI
	~MultiKinectOpenNI();
#endif

	void			getMetadata(PluginMetadata &info);
	void			setup(PluginContext &context);
	
private:

	_2RealKinect*	m_2RealKinect;
	unsigned int	m_GeneratorFlags;
	unsigned int	m_ImageFlags;
	_2RealLogLevel	m_LogLevel;
	string			m_LogPath;
	ofstream		m_LogStream;
	bool			m_bDepthService;
	bool			m_bColorService;
	bool			m_bUserService;

	void			getGeneratorFlags(PluginContext &context);
	void			getImageFlags(PluginContext &context);
	void			getLogSettings(PluginContext &context);
};

#ifdef _WINSDK
	MultiKinectWinSdk::~MultiKinectWinSdk()
#elif _OPENNI
	MultiKinectOpenNI::~MultiKinectOpenNI()
#endif
{
	if (m_2RealKinect)
	{
		cout << "shutting down" << std::endl;
		m_2RealKinect->shutdown();
	}
}

void
#ifdef _WINSDK
	MultiKinectWinSdk::
#elif _OPENNI
	MultiKinectOpenNI::
#endif
	getMetadata(PluginMetadata &info)
{
	try
	{
		MetadataReader reader;
		reader.readMetadata(info);
	}
	catch (Exception &e)
	{
		cout << "multi kinect: exception while reading metadata" << endl;
		throw e;
	}
}

void
#ifdef _WINSDK
	MultiKinectWinSdk::
#elif _OPENNI
	MultiKinectOpenNI::
#endif
	setup(PluginContext &context)
{
	try
	{
		getImageFlags(context);
		getGeneratorFlags(context);
		getLogSettings(context);

		context.registerService("Image Generator", &::createImageService);

		m_2RealKinect = _2RealKinect::getInstance();
	
		m_2RealKinect->setLogLevel(m_LogLevel);
		if (!m_LogPath.empty())
		{
			m_LogStream.open(m_LogPath);
			m_2RealKinect->setLogOutputStream(&m_LogStream);
		}

		m_2RealKinect->start(m_GeneratorFlags, m_ImageFlags);
	}
	catch (Exception &e)
	{
		cout << "multi kinect: exception while executing setup" << endl;
		throw e;
	}
	catch (...)
	{
		cout << "multi kinect: exception while executing setup" << endl;
		throw Exception("exception during plugin setup");
	}
}

void
#ifdef _WINSDK
	MultiKinectWinSdk::
#elif _OPENNI
	MultiKinectOpenNI::
#endif
	getGeneratorFlags(PluginContext &context)
{
	try
	{
		m_GeneratorFlags = 0;
		m_bDepthService = false;
		m_bColorService = false;
		m_bUserService = false;

		context.getSetupParameter("use depth", m_bDepthService);
		if (m_bDepthService)
		{
			m_GeneratorFlags |= DEPTHIMAGE;
		}

		context.getSetupParameter("use color", m_bColorService);
		if (m_bColorService)
		{
			m_GeneratorFlags |= COLORIMAGE;
		}

		context.getSetupParameter("use usermap", m_bUserService);
		if (m_bUserService)
		{
			m_GeneratorFlags |= USERIMAGE;
		}
	}
	catch (Exception &e)
	{
		cout << "multi kinect: exception in getGeneratorFlags()" << endl;
		throw e;
	}
}

void
#ifdef _WINSDK
	MultiKinectWinSdk::
#elif _OPENNI
	MultiKinectOpenNI::
#endif
	getImageFlags(PluginContext &context)
{
	try
	{
		m_ImageFlags = 0;

		string imageResolution;
		string depthResolution;
		bool useMirroring = true;

		context.getSetupParameter("use mirroring", useMirroring);
		if (useMirroring)
		{
			m_ImageFlags |= IMAGE_MIRRORING;
		}

		context.getSetupParameter("image resolution", imageResolution);
		if (imageResolution == "640x480")
		{
			m_ImageFlags |= IMAGE_COLOR_640X480;
		}
		else
		{
			m_ImageFlags |= IMAGE_COLOR_640X480;
		}

		context.getSetupParameter("depth resolution", depthResolution);
		if (depthResolution == "640x480")
		{
			m_ImageFlags |= IMAGE_USER_DEPTH_640X480;
		}
		else
		{
			m_ImageFlags |= IMAGE_USER_DEPTH_640X480;
		}

	}
	catch (Exception &e)
	{
		cout << "multi kinect: exception in getImageFlags()" << endl;
		throw e;
	}
}

void
#ifdef _WINSDK
	MultiKinectWinSdk::
#elif _OPENNI
	MultiKinectOpenNI::
#endif
	getLogSettings(PluginContext &context)
{
	try
	{
		m_LogPath = string();
		m_LogLevel = none;
		string level;

		context.getSetupParameter("log file", m_LogPath);
		context.getSetupParameter("log level", level);
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
			throw Exception("invalid parameter - log level");
		}
	}
	catch (Exception &e)
	{
		cout << "multi kinect: exception in getLogSettings()" << endl;
		throw e;
	}
}

#ifdef _WINSDK
	_2REAL_EXPORT_PLUGIN(MultiKinectWinSdk)
#elif _OPENNI
	_2REAL_EXPORT_PLUGIN(MultiKinectOpenNI)
#endif