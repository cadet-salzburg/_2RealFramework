#pragma once

#include <vector>
#include <map>
#include <string>

#include "Poco\SharedPtr.h"
#include "Poco\ClassLoader.h"
#include "Poco\Any.h"

namespace _2Real
{
	class Plugin;
	class PluginManifest;
	class PluginFramework;
	class PluginContext;
	class PluginFrameworkContext;

	class ServiceReference;
	class ServiceRegistration;

	class IPluginActivator;
	class IService;

	//typedef Poco::SharedPtr<PluginContext> PluginContextPtr;
	//typedef Poco::SharedPtr<PluginFrameworkContext> FrameworkContextPtr;
	//typedef Poco::SharedPtr<Plugin> PluginPtr;

	typedef std::map<std::string, std::string> PluginMetadata;
	typedef std::map<std::string, Plugin* > Plugins;
	
	typedef Poco::ClassLoader<IPluginActivator> PluginLoader;
	
	//typedef Poco::SharedPtr<IService> ServicePtr;
	//typedef Poco::SharedPtr<ServiceReference> ServiceRefPtr;
	//typedef Poco::SharedPtr<ServiceRegistration> ServiceRegPtr;

	typedef std::multimap<std::string, ServiceReference* > Services;
	typedef std::map<std::string, Poco::Any> ServiceProperties;
}