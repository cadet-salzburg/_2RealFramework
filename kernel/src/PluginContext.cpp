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

#include "PluginContext.h"
#include "PluginFrameworkContext.h"
#include "Plugin.h"
#include "ServiceRegistration.h"

namespace _2Real
{
	PluginContext::PluginContext(Plugin* plugin)
	{
		m_PluginPtr = plugin;
	}

	PluginContext::~PluginContext()
	{
		m_PluginPtr = NULL;
	}

	ServiceRegistration* PluginContext::registerService(const std::string& name, const ServiceProperties& properties, IService* service)
	{
		return m_PluginPtr->frameworkContextPtr()->registerService(name, properties, service, m_PluginPtr);
	}

	ServiceReference* PluginContext::findService(const std::string& name, const ServiceProperties& properties)
	{
		std::string p = std::string();
		return m_PluginPtr->frameworkContextPtr()->findService(name, p, properties);
	}

	ServiceReference* PluginContext::findService(const std::string& name, const std::string& plugin)
	{
		ServiceProperties p = ServiceProperties();
		return m_PluginPtr->frameworkContextPtr()->findService(name, plugin, p);
	}

	ServiceReference* PluginContext::findService(const std::string& name, const std::string& plugin, const ServiceProperties& properties)
	{
		return m_PluginPtr->frameworkContextPtr()->findService(name, plugin, properties);
	}

	void PluginContext::addServiceListerner(IPluginActivator& a)
	{
		m_PluginPtr->frameworkContextPtr()->addServiceListener(a);
	}

	void PluginContext::addFrameworkListerner(IPluginActivator& a)
	{
		m_PluginPtr->frameworkContextPtr()->addFrameworkListener(a);
	}

	void PluginContext::addPluginListerner(IPluginActivator& a)
	{
		m_PluginPtr->frameworkContextPtr()->addPluginListener(a);
	}

	void PluginContext::removeServiceListerner(IPluginActivator& a)
	{
		m_PluginPtr->frameworkContextPtr()->removeServiceListener(a);
	}

	void PluginContext::removeFrameworkListerner(IPluginActivator& a)
	{
		m_PluginPtr->frameworkContextPtr()->removeFrameworkListener(a);
	}

	void PluginContext::removePluginListerner(IPluginActivator& a)
	{
		m_PluginPtr->frameworkContextPtr()->removePluginListener(a);
	}
}