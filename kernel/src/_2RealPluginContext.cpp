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

#include "_2RealPluginContext.h"
#include "_2RealContextPrivate.h"
#include "_2RealPlugin.h"
#include "_2RealServiceRegistration.h"

namespace _2Real
{
	PluginContext::PluginContext(Plugin* _plugin, ContextPrivate* _context) : m_PluginPtr(_plugin), m_ContextPtr(_context)
	{
	}

	PluginContext::~PluginContext()
	{
		m_PluginPtr = NULL;
		m_ContextPtr = NULL;
	}

	bool PluginContext::registerService(const std::string _name, ServicePtr (*_func)(void))
	{
		return m_ContextPtr->registerService(_name, m_PluginPtr->name(), _func);
	}

	void PluginContext::unregisterService(const std::string _name)
	{
		return m_ContextPtr->invalidateService(_name, m_PluginPtr->name());
	}

	void PluginContext::addServiceListerner(IPluginActivator& _activator)
	{
		m_ContextPtr->addServiceListener(_activator);
	}

	void PluginContext::addFrameworkListerner(IPluginActivator& _activator)
	{
		m_ContextPtr->addFrameworkListener(_activator);
	}

	void PluginContext::addPluginListerner(IPluginActivator& _activator)
	{
		m_ContextPtr->addPluginListener(_activator);
	}

	void PluginContext::removeServiceListerner(IPluginActivator& _activator)
	{
		m_ContextPtr->removeServiceListener(_activator);
	}

	void PluginContext::removeFrameworkListerner(IPluginActivator& _activator)
	{
		m_ContextPtr->removeFrameworkListener(_activator);
	}

	void PluginContext::removePluginListerner(IPluginActivator& _activator)
	{
		m_ContextPtr->removePluginListener(_activator);
	}
}