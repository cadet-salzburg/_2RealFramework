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
		
		m_Plugins.clear();
		m_Services.clear();
	};

	Plugin* PluginFrameworkContext::installPlugin(const std::string& pluginName)
	{
		Plugin* plugin = new Plugin(pluginName, this);
		plugin->readMetadata();

		m_Plugins.insert(std::pair<std::string, Plugin*>(pluginName, plugin));
		m_PluginNotificationCenter.postNotification(new PluginNotification(pluginName, "plugin was installed"));
		
		return plugin;
	}

	void PluginFrameworkContext::uninstallPlugin(Plugin* plugin)
	{
		std::map<std::string, Plugin*>::iterator it = m_Plugins.find(plugin->m_PluginName);
		m_Plugins.erase(it);

		m_PluginNotificationCenter.postNotification(new PluginNotification(plugin->m_PluginName, "plugin was uninstalled"));
	}

	ServiceRegistration* PluginFrameworkContext::registerService(const std::string& serviceName, const ServiceProperties& properties, IService* service, Plugin* plugin)
	{
		ServiceRegistration* reg = new ServiceRegistration(serviceName, this, service);
		ServiceReference* ref = new ServiceReference(serviceName, properties, service, plugin);

		m_Services.insert(std::pair<std::string, ServiceReference*>(serviceName, ref));

		m_ServiceNotificationCenter.postNotification(new ServiceNotification(serviceName, plugin->m_PluginName, "service was added"));

		return reg;
	}

	void PluginFrameworkContext::unregisterService(const std::string& serviceName, IService* service)
	{
		std::pair<Services::iterator, Services::iterator> range = m_Services.equal_range(serviceName);
		for (Services::iterator it = range.first; it != range.second; it++)
		{
			if (it->second->servicePtr() == service)
			{
				m_Services.erase(it);
				m_ServiceNotificationCenter.postNotification(new ServiceNotification(serviceName, it->second->pluginPtr()->m_PluginName, "service was removed"));
			}
		}
	}

	ServiceReference* PluginFrameworkContext::findService(const std::string& serviceName, const std::string& pluginName, const ServiceProperties& properties)
	{
		std::pair<Services::iterator, Services::iterator> range = m_Services.equal_range(serviceName);

		for (Services::iterator sIter = range.first; sIter != range.second; sIter++)
		{
			std::string plugin = sIter->second->pluginPtr()->m_PluginName;

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

		return NULL;
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
		//m_ServiceNotificationCenter.removeObserver(Poco::NObserver<IPluginActivator, ServiceNotification>(a, &IPluginActivator::handleServiceNotification));
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