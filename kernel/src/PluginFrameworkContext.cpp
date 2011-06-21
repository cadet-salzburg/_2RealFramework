/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "PluginFrameworkContext.h"
#include "Plugin.h"
#include "ServiceReference.h"
#include "ServiceRegistration.h"
#include "ServiceNotification.h"
#include "FrameworkNotification.h"
#include "PluginNotification.h"

namespace _2Real
{
	PluginFrameworkContext::PluginFrameworkContext()
	{
	}

	PluginFrameworkContext::~PluginFrameworkContext()
	{
	};

	PluginPtr PluginFrameworkContext::installPlugin(const std::string& pluginName)
	{
		PluginPtr plugin = PluginPtr(new Plugin(pluginName, this));
		plugin->readMetadata();

		m_Plugins.insert(std::pair<std::string, PluginPtr>(pluginName, plugin));
		m_PluginNotificationCenter.postNotification(new PluginNotification(pluginName, "installed"));

		return m_Plugins.find(pluginName)->second;
	}

	void PluginFrameworkContext::uninstallPlugin(Plugin* plugin)
	{
		Plugins::iterator it = m_Plugins.find(plugin->m_PluginName);
		m_Plugins.erase(it);

		m_PluginNotificationCenter.postNotification(new PluginNotification(plugin->m_PluginName, "uninstalled"));
	}

	ServiceRegPtr PluginFrameworkContext::registerService(const std::string& serviceName, const std::string& pluginName, const ServiceProperties& properties, ServicePtr service)
	{
		ServiceRegPtr reg = ServiceRegPtr(new ServiceRegistration(serviceName, this, service));
		ServiceRefPtr ref = ServiceRefPtr(new ServiceReference(serviceName, pluginName, properties, service));

		m_Services.insert(std::pair<std::string, ServiceRefPtr>(serviceName, ref));

		m_ServiceNotificationCenter.postNotification(new ServiceNotification(serviceName, pluginName, "added"));

		return reg;
	}

	void PluginFrameworkContext::unregisterService(const std::string& serviceName, ServicePtr service)
	{
		std::pair<Services::iterator, Services::iterator> range = m_Services.equal_range(serviceName);
		for (Services::iterator it = range.first; it != range.second; it++)
		{
			if (it->second->servicePtr() == service)
			{
				m_Services.erase(it);
				m_ServiceNotificationCenter.postNotification(new ServiceNotification(serviceName, it->second->pluginName(), "removed"));
			}
		}
	}

	ServiceRefPtr PluginFrameworkContext::findService(const std::string& serviceName, const std::string& pluginName, const ServiceProperties& properties)
	{
		std::pair<Services::iterator, Services::iterator> range = m_Services.equal_range(serviceName);

		for (Services::iterator sIter = range.first; sIter != range.second; sIter++)
		{
			std::string plugin = sIter->second->pluginName();

			if (pluginName.empty() || pluginName == plugin)
			{
				return sIter->second;

				//ignoring the service properties until I know what precisely they are
				/*
				ServiceProperties p = sIter->second->serviceProperties();
				for (ServiceProperties::const_iterator pIterA = properties.begin(); pIterA != properties.end(); pIterA++)
				{
					for (ServiceProperties::const_iterator pIterB = p.begin(); pIterB != p.end(); pIterB++)
					{
					}
				}
				*/
			}
		}

		return ServiceRefPtr(NULL);
	}

	void PluginFrameworkContext::addServiceListener(IPluginActivator& a)
	{
		m_ServiceNotificationCenter.addObserver(Poco::NObserver<IPluginActivator, ServiceNotification>(a, &IPluginActivator::handleServiceNotification));
	}

	void PluginFrameworkContext::addFrameworkListener(IPluginActivator& a)
	{
		m_FrameworkNotificationCenter.addObserver(Poco::NObserver<IPluginActivator, FrameworkNotification>(a, &IPluginActivator::handleFrameworkNotification));
	}

	void PluginFrameworkContext::addPluginListener(IPluginActivator& a)
	{
		m_PluginNotificationCenter.addObserver(Poco::NObserver<IPluginActivator, PluginNotification>(a, &IPluginActivator::handlePluginNotification));
	}

	void PluginFrameworkContext::removeServiceListener(IPluginActivator& a)
	{
		m_ServiceNotificationCenter.removeObserver(Poco::NObserver<IPluginActivator, ServiceNotification>(a, &IPluginActivator::handleServiceNotification));
	}

	void PluginFrameworkContext::removeFrameworkListener(IPluginActivator& a)
	{
		m_FrameworkNotificationCenter.removeObserver(Poco::NObserver<IPluginActivator, FrameworkNotification>(a, &IPluginActivator::handleFrameworkNotification));
	}

	void PluginFrameworkContext::removePluginListener(IPluginActivator& a)
	{
		m_PluginNotificationCenter.removeObserver(Poco::NObserver<IPluginActivator, PluginNotification>(a, &IPluginActivator::handlePluginNotification));
	}
}