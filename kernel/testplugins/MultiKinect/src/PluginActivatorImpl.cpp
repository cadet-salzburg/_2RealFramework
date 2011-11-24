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
	bool			m_AlignColorDepth;
	unsigned int	m_GeneratorFlags;
	unsigned int	m_ImageFlags;
	_2RealLogLevel	m_LogLevel;
	string			m_LogPath;
	ofstream		m_LogStream;

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
	MetadataReader reader(info);
	reader.readMetadata();
}

void
#ifdef _WINSDK
	MultiKinectWinSdk::
#elif _OPENNI
	MultiKinectOpenNI::
#endif
	setup(PluginContext &context)
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

void
#ifdef _WINSDK
	MultiKinectWinSdk::
#elif _OPENNI
	MultiKinectOpenNI::
#endif
	getGeneratorFlags(PluginContext &context)
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

void
#ifdef _WINSDK
	MultiKinectWinSdk::
#elif _OPENNI
	MultiKinectOpenNI::
#endif
	getImageFlags(PluginContext &context)
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

	unsigned int mirrored = 0;
	for (vector< string >::iterator it = flags.begin(); it != flags.end(); it++)
	{
		if (*it == "mirrored")
		{
			depth = ::IMAGE_MIRRORING;
			break;
		}
	}

	m_ImageFlags = depth | color | mirrored;
}

void
#ifdef _WINSDK
	MultiKinectWinSdk::
#elif _OPENNI
	MultiKinectOpenNI::
#endif
	getLogSettings(PluginContext &context)
{
	m_LogPath = string();

	m_LogPath = context.getParameterValue< string >("log file");
	string level = context.getParameterValue< string >("log level");

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

#ifdef _WINSDK
	_2REAL_EXPORT_PLUGIN(MultiKinectWinSdk)
#elif _OPENNI
	_2REAL_EXPORT_PLUGIN(MultiKinectOpenNI)
#endif