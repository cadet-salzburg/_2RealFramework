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

#include "ServiceReference.h"
#include "IService.h"
#include "Plugin.h"

namespace _2Real
{
	ServiceReference::ServiceReference(const std::string& serviceName, const std::string& pluginName, const ServiceProperties& properties, ServicePtr service) :
										m_ServiceName(serviceName), m_PluginName(pluginName), m_Properties(properties), m_ServicePtr(service)
	{
	}

	ServiceReference::~ServiceReference()
	{
	}

	const std::string& ServiceReference::serviceName() const
	{
		return m_ServiceName;
	}

	const ServiceProperties& ServiceReference::serviceProperties() const
	{
		return m_Properties;
	}

	ServicePtr ServiceReference::servicePtr() const
	{
		return m_ServicePtr;
	}

	const std::string& ServiceReference::pluginName() const
	{
		return m_PluginName;
	}
}