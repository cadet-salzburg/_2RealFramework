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

/*
	as of 7/7/11, replaces the PluginFrameworkContext class
*/

namespace _2Real
{
	class _2RealContextPrivate
	{

	public:
		
		~_2RealContextPrivate();

		void addServiceListener(_2RealIPluginActivator& _activator);								//called by a plugin, adds service listener
		void addFrameworkListener(_2RealIPluginActivator& _activator);								//called by a plugin, adds framework listener
		void addPluginListener(_2RealIPluginActivator& _activator);									//called by a plugin, adds plugin listener
		void removeServiceListener(_2RealIPluginActivator& _activator);								//called by a plugin, removes service listener
		void removeFrameworkListener(_2RealIPluginActivator& _activator);							//called by a plugin, removes framework listener
		void removePluginListener(_2RealIPluginActivator& _activator);								//called by a plugin, removes plugin listener

		_2RealServiceRegPtr registerService(const std::string& _name, const std::string& _plugin, _2RealServicePtr& _service);
		void unregisterService(const std::string& _name, const std::string& _plugin, _2RealServicePtr& _service);

		void uninstallPlugin(_2RealPlugin* _plugin);												//called by a plugin, removes plugin from plugin pool
		void stopPlugin(_2RealPlugin* _plugin);														//called by a plugin, generates plugin stopped message
		void startPlugin(_2RealPlugin* _plugin);													//called by a plugin, generates plugin started message

	private:

		_2RealContextPrivate();

		Plugins						m_Plugins;														//map of installed plugins
		Services					m_Services;														//map of registered services
		
		Poco::NotificationCenter		m_ServiceNotificationCenter;								//poco notification center for service events
		Poco::NotificationCenter		m_FrameworkNotificationCenter;								//poco notification center for framework events
		Poco::NotificationCenter		m_PluginNotificationCenter;									//poco notification center for plugin events

		_2RealPluginPtr loadPlugin(const std::string& path);										//install + start plugin
		const std::vector<_2RealPluginPtr>& loadAllPlugins(const std::string& directory);			//install + start all plugins from one directory
		void unloadPlugin(const std::string& path);													//uninstall + stop plugin
		void unloadAllPlugins();																	//uninstall + stop all plugins
		_2RealPluginPtr installPlugin(const std::string& _name, const std::string& _path);			//install plugin
		const std::vector<_2RealPluginPtr>& installAllPlugins(const std::string& directory);		//install all plugins from one directory

		friend class _2RealContext;
	};
}