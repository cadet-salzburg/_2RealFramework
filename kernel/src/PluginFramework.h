#pragma once

#include "Common.h"
#include "PluginFrameworkContext.h"

namespace _2Real
{	
	class PluginFramework
	{
	public:

		PluginFramework();
		~PluginFramework();

		//installs a plugin in the framework;
		//the pointer can the be used to start/stop/uninstall the plugin
		Plugin* installPlugin(const std::string& pluginName);

	private:

		Poco::Mutex				m_Lock;		
		PluginFrameworkContext	m_Context;
	};
}