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
	a plugins' execution context within the framwork
*/

namespace _2Real
{
	class PluginContext
	{
	
		friend class Plugin;

	public:

		~PluginContext();

		/*
			listener functions
		*/

		void addServiceListerner(IPluginActivator& a);			//register a service event listerner for the plugin
		void addFrameworkListerner(IPluginActivator& a);		//register a framework event listener for the plugin
		void addPluginListerner(IPluginActivator& a);			//register a plugin event listener for the plugin
		void removeServiceListerner(IPluginActivator& a);		//remove the service event listerner for the plugin
		void removeFrameworkListerner(IPluginActivator& a);		//remove the framework event listener for the plugin
		void removePluginListerner(IPluginActivator& a);		//remove the plugin event listener for the plugin

		/*
			register service within framework
		*/

		ServiceRegPtr registerService(const std::string& name, const ServiceProperties& properties, ServicePtr service);

		/*
			get reference to a registered service
		*/

		ServiceRefPtr findService(const std::string& name, const ServiceProperties& properties) const;
		ServiceRefPtr findService(const std::string& name, const std::string& plugin) const;
		ServiceRefPtr findService(const std::string& name, const std::string& plugin, const ServiceProperties& properties) const;
		
	private:

		Plugin*			m_PluginPtr;							//the plugin this context belongs to

		PluginContext(Plugin* plugin);							//only plugin can create its own context
	};
}