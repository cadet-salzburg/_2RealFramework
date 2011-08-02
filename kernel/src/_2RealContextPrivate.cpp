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

#include "_2RealContextPrivate.h"
#include "_2RealPlugin.h"
#include "_2RealIService.h"
#include "_2RealServiceContainer.h"
#include "_2RealIContainer.h"
#include "_2RealServiceRegistration.h"
#include "_2RealServiceNotification.h"
#include "_2RealFrameworkNotification.h"
#include "_2RealPluginNotification.h"

#include "Poco/NObserver.h"

namespace _2Real
{
	ContextPrivate::ContextPrivate()
	{
	}

	ContextPrivate::~ContextPrivate()
	{
	}

	PluginPtr ContextPrivate::installPlugin(const std::string& _name, const std::string& _path)
	{
		PluginPtr plugin = PluginPtr(new Plugin(_name, _path, this));
		m_Plugins.insert(std::pair<std::string, PluginPtr>(_name, plugin));
		if (m_PluginNotificationCenter.hasObservers())
		{
			m_PluginNotificationCenter.postNotification(new PluginNotification(_name, "installed"));
		}

		return m_Plugins.find(_name)->second;
	}

	void ContextPrivate::uninstallPlugin(Plugin* _plugin)
	{
		Plugins::iterator it = m_Plugins.find(_plugin->name());
		m_Plugins.erase(it);

		if (m_PluginNotificationCenter.hasObservers())
		{
			m_PluginNotificationCenter.postNotification(new PluginNotification(_plugin->name(), "uninstalled"));
		}
	}

	bool ContextPrivate::updateServiceRegistry()
	{
		return m_ServiceRegistry.update();
	}

	bool ContextPrivate::registerService(const std::string _name, const std::string& _plugin, ServiceFactoryMethod _func)
	{
		return m_ServiceFactory.registerService(_name, _func);
	}

	ContainerPtr ContextPrivate::createService(const std::string& _name, ConfigMetadataPtr const& _config)
	{
		ServicePtr service = m_ServiceFactory.createService(_name);

		if (!service.isNull())
		{
			ServicePtr container(new ServiceContainer(service));

			m_ServiceRegistry.addService(_name, container);

			if (container->setup(_config))
			{
				service->addListener(container);
			}
			else
			{
				std::cout << "error on service configuration" << std::endl;
			}
			
			ContainerPtr result = container.unsafeCast< IContainer >();

			return result;
		}

		return ContainerPtr();
	}

	void ContextPrivate::invalidateService(const std::string _name, const std::string& _plugin)
	{
		return m_ServiceFactory.unregisterService(_name);
	}

	void ContextPrivate::addServiceListener(IPluginActivator& _activator)
	{
		m_ServiceNotificationCenter.addObserver(Poco::NObserver<IPluginActivator, ServiceNotification>(_activator, &IPluginActivator::handleServiceNotification));
	}

	void ContextPrivate::addFrameworkListener(IPluginActivator& _activator)
	{
		m_FrameworkNotificationCenter.addObserver(Poco::NObserver<IPluginActivator, FrameworkNotification>(_activator, &IPluginActivator::handleFrameworkNotification));
	}

	void ContextPrivate::addPluginListener(IPluginActivator& _activator)
	{
		m_PluginNotificationCenter.addObserver(Poco::NObserver<IPluginActivator, PluginNotification>(_activator, &IPluginActivator::handlePluginNotification));
	}

	void ContextPrivate::removeServiceListener(IPluginActivator& _activator)
	{
		m_ServiceNotificationCenter.removeObserver(Poco::NObserver<IPluginActivator, ServiceNotification>(_activator, &IPluginActivator::handleServiceNotification));
	}

	void ContextPrivate::removeFrameworkListener(IPluginActivator& _activator)
	{
		m_FrameworkNotificationCenter.removeObserver(Poco::NObserver<IPluginActivator, FrameworkNotification>(_activator, &IPluginActivator::handleFrameworkNotification));
	}

	void ContextPrivate::removePluginListener(IPluginActivator& _activator)
	{
		m_PluginNotificationCenter.removeObserver(Poco::NObserver<IPluginActivator, PluginNotification>(_activator, &IPluginActivator::handlePluginNotification));
	}
}