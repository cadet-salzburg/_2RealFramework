#pragma once

#include "Common.h"
#include "IService.h"

/* 
	a plugins' execution context within the framwork
*/

namespace _2Real
{
	class PluginContext
	{
	public:

		~PluginContext();
		Plugin* getPlugin();

	private:

		PluginContext(Plugin* plugin);
		
		//registers a service in the framework
		ServiceRegistration* registerService(const std::string& name, const ServiceProperties& properties, IService::ConstPtr service);

		Plugin*			m_Plugin;

		friend class Plugin;
	};
}