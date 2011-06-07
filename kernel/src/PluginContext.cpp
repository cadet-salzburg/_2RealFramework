#include "PluginContext.h"
#include "PluginFrameworkContext.h"
#include "Plugin.h"
#include "ServiceRegistration.h"

namespace _2Real
{
	PluginContext::PluginContext(Plugin* plugin)
	{
		m_Plugin = plugin;
	}

	PluginContext::~PluginContext()
	{
		m_Plugin = NULL;
	}

	ServiceRegistration* PluginContext::registerService(const std::string& name, const ServiceProperties& properties, IService::ConstPtr service)
	{
		return m_Plugin->getFrameworkContext()->registerService(name, properties, service, m_Plugin);
	}

	Plugin* PluginContext::getPlugin()
	{
		return m_Plugin;
	}
}