#include "PluginFrameworkContext.h"
#include "Plugin.h"
#include "ServiceReference.h"
#include "ServiceRegistration.h"

namespace _2Real
{
	PluginFrameworkContext::PluginFrameworkContext()
	{
	}

	PluginFrameworkContext::~PluginFrameworkContext()
	{
		m_Plugins.clear();
		m_Services.clear();
	};

	Plugin* PluginFrameworkContext::installPlugin(const std::string& pluginName)
	{		
		Plugin* plugin = new Plugin(pluginName, this);
	
		m_Plugins.insert(std::pair<std::string, Plugin*>(pluginName, plugin));
		
		return plugin;
	}

	ServiceRegistration* PluginFrameworkContext::registerService(const std::string& serviceName, const ServiceProperties& properties, IService::ConstPtr service, Plugin* plugin)
	{
		ServiceRegistration* reg = new ServiceRegistration(serviceName, this, service);
		ServiceReference* ref = new ServiceReference(serviceName, properties, service, plugin);

		m_Services.insert(std::pair<std::string, ServiceReference*>(serviceName, ref));

		return reg;
	}

	void PluginFrameworkContext::unregisterService(const std::string& name, IService* service)
	{
		std::pair<Services::iterator, Services::iterator> range = m_Services.equal_range(name);
		for (Services::iterator it = range.first; it != range.second; it++)
		{
			if (it->second->getService() == service)
			{
				//todo: remove service
			}
		}
	}
}