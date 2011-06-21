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

#include <string>

#include "Common.h"

/*
	reference to a service
	that is registered in the framework
*/

namespace _2Real
{
	class ServiceReference
	{

		friend class PluginFrameworkContext;
	
	public:

		~ServiceReference();

		ServicePtr					servicePtr() const;
		const std::string&			serviceName() const;
		const std::string&			pluginName() const;
		const ServiceProperties&	serviceProperties() const;

	private:

		ServiceReference(const std::string& serviceName, const std::string& pluginName, const ServiceProperties& properties, ServicePtr service);

		std::string					m_ServiceName;	//name under which the service is registered
		ServicePtr					m_ServicePtr;	//pointer to the service
		ServiceProperties			m_Properties;	//the services' properties
		std::string					m_PluginName;	//the plugin that offers the service
	};
}