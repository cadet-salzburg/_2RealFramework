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
	class _2RealPluginContext
	{

	public:

		~_2RealPluginContext();

		void addServiceListerner(_2RealIPluginActivator& _activator);								//register a service event listerner for the plugin
		void addFrameworkListerner(_2RealIPluginActivator& _activator);								//register a framework event listener for the plugin
		void addPluginListerner(_2RealIPluginActivator& _activator);								//register a plugin event listener for the plugin
		void removeServiceListerner(_2RealIPluginActivator& _activator);							//remove the service event listerner for the plugin
		void removeFrameworkListerner(_2RealIPluginActivator& _activator);							//remove the framework event listener for the plugin
		void removePluginListerner(_2RealIPluginActivator& _activator);								//remove the plugin event listener for the plugin

		_2RealServiceRegPtr registerService(const std::string& _name, _2RealServicePtr& _service);	//register a service within the framework
		
	private:

		_2RealPluginContext(_2RealPlugin* _plugin, _2RealContextPrivate* _context);

		_2RealPlugin*			m_PluginPtr;
		_2RealContextPrivate*	m_ContextPtr;

		friend class _2RealPlugin;

	};
}