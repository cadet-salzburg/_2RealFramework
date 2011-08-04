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

#pragma once

#include "_2RealFramework.h"
#include "_2RealServiceFactory.h"
#include "_2RealServiceRegistry.h"

#include "Poco/NotificationCenter.h"

namespace _2Real
{
	class ContextPrivate
	{

	public:
		
		~ContextPrivate();

		void addServiceListener(IPluginActivator& _activator);									//called by a plugin, adds service listener
		void addFrameworkListener(IPluginActivator& _activator);								//called by a plugin, adds framework listener
		void addPluginListener(IPluginActivator& _activator);									//called by a plugin, adds plugin listener
		void removeServiceListener(IPluginActivator& _activator);								//called by a plugin, removes service listener
		void removeFrameworkListener(IPluginActivator& _activator);								//called by a plugin, removes framework listener
		void removePluginListener(IPluginActivator& _activator);								//called by a plugin, removes plugin listener

		bool registerService(const std::string _name, const std::string& _plugin, ServiceFactoryMethod _func);
		void invalidateService(const std::string _name, const std::string& _plugin);
		ContainerPtr createService(const std::string _name, ConfigMetadataPtr const _config);
		bool updateServiceRegistry();

		void uninstallPlugin(Plugin* _plugin);													//called by a plugin, removes plugin from plugin pool
		void stopPlugin(Plugin* _plugin);														//called by a plugin, generates plugin stopped message
		void startPlugin(Plugin* _plugin);														//called by a plugin, generates plugin started message

	private:

		typedef std::map<std::string, PluginPtr>	Plugins;

		ContextPrivate();

		Plugins						m_Plugins;													//map of installed plugins
		ServiceRegistry				m_ServiceRegistry;
		ServiceFactory				m_ServiceFactory;
		
		Poco::NotificationCenter		m_ServiceNotificationCenter;							//poco notification center for service events
		Poco::NotificationCenter		m_FrameworkNotificationCenter;							//poco notification center for framework events
		Poco::NotificationCenter		m_PluginNotificationCenter;								//poco notification center for plugin events

		PluginPtr installPlugin(const std::string _name, const std::string _path);			//install plugin

		friend class Context;
	};
}