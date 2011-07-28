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
	a plugins' execution context within the framwork
	used to add listeners & create services
*/

namespace _2Real
{
	class PluginContext
	{

	public:

		~PluginContext();

		void addServiceListerner(IPluginActivator& _activator);								//register a service event listerner for the plugin
		void addFrameworkListerner(IPluginActivator& _activator);							//register a framework event listener for the plugin
		void addPluginListerner(IPluginActivator& _activator);								//register a plugin event listener for the plugin
		void removeServiceListerner(IPluginActivator& _activator);							//remove the service event listerner for the plugin
		void removeFrameworkListerner(IPluginActivator& _activator);						//remove the framework event listener for the plugin
		void removePluginListerner(IPluginActivator& _activator);							//remove the plugin event listener for the plugin

		bool registerService(const std::string _name, ServicePtr (*_func)(void));
		void unregisterService(const std::string _name);
	
	private:

		PluginContext(Plugin* _plugin, ContextPrivate* _context);

		Plugin*				m_PluginPtr;
		ContextPrivate*		m_ContextPtr;

		friend class Plugin;

	};
}