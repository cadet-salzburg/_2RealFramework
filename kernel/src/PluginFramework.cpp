#include "PluginFramework.h"
#include "Plugin.h"

namespace _2Real
{
	PluginFramework::PluginFramework()
	{
	}

	PluginFramework::~PluginFramework()
	{
	}

	Plugin* PluginFramework::installPlugin(const std::string& pluginName)
	{		
		Poco::ScopedLock<Poco::Mutex> lock(m_Lock);
		return m_Context.installPlugin(pluginName);
	}
}