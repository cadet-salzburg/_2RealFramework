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

#include "Common.h"
#include "IService.h"

/*
	the frameworks' execution context
*/

namespace _2Real
{
	class PluginFrameworkContext
	{
		friend class Plugin;
		friend class PluginContext;
		friend class ServiceRegistration;
		friend class PluginFramework;

	public:
		
		~PluginFrameworkContext();

	private:

		PluginFrameworkContext();

		Plugins							m_Plugins;							//map of installed plugins
		Services						m_Services;							//multimap of registered services
		
		Poco::NotificationCenter		m_ServiceNotificationCenter;		//poco notification center for service events
		Poco::NotificationCenter		m_FrameworkNotificationCenter;		//poco notification center for framework events
		Poco::NotificationCenter		m_PluginNotificationCenter;			//poco notification center for plugin events

		/*
			listerner functions
		*/

		void addServiceListener(IPluginActivator& a);
		void addFrameworkListener(IPluginActivator& a);
		void addPluginListener(IPluginActivator& a);
		void removeServiceListener(IPluginActivator& a);
		void removeFrameworkListener(IPluginActivator& a);
		void removePluginListener(IPluginActivator& a);

		/*
			service related functions
		*/

		ServiceRegistration* registerService(const std::string& name, const ServiceProperties& properties, IService* service, Plugin* plugin);
		void unregisterService(const std::string& name, IService* service);
		ServiceReference* findService(const std::string& name, const std::string& plugin, const ServiceProperties& properties); 

		/*
			plugin related functions
		*/

		Plugin* installPlugin(const std::string& pluginName);
		void uninstallPlugin(Plugin* plugin);
	};
}