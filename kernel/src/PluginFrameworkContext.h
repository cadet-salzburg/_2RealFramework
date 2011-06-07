#pragma once

#include "Common.h"
#include "IService.h"

/*
	the frameworks' execution context
*/

namespace _2Real
{
	class PluginFrameworkContext
	{
	public:
		~PluginFrameworkContext();
		PluginFrameworkContext();

		Plugin* installPlugin(const std::string& pluginName);
		
		ServiceRegistration* registerService(const std::string& name, const ServiceProperties& properties, IService::ConstPtr service, Plugin* plugin);
		void unregisterService(const std::string& name, IService* service);

	private:	

		Plugins					m_Plugins;  //map of installed plugins (key = name)
		Services				m_Services; //multimap of registered services (key = name)
	};
}