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
	_2RealPluginContext::_2RealPluginContext(_2RealPlugin* _plugin, _2RealContextPrivate* _context) : m_PluginPtr(_plugin), m_ContextPtr(_context)
	{
	}

	_2RealPluginContext::~_2RealPluginContext()
	{
		m_PluginPtr = NULL;
		m_ContextPtr = NULL;
	}

	_2RealServiceRegPtr _2RealPluginContext::registerService(const std::string& _name, _2RealServicePtr& _service)
	{
		return m_ContextPtr->registerService(_name, m_PluginPtr->name(), _service);
	}

	void _2RealPluginContext::addServiceListerner(_2RealIPluginActivator& _activator)
	{
		m_ContextPtr->addServiceListener(_activator);
	}

	void _2RealPluginContext::addFrameworkListerner(_2RealIPluginActivator& _activator)
	{
		m_ContextPtr->addFrameworkListener(_activator);
	}

	void _2RealPluginContext::addPluginListerner(_2RealIPluginActivator& _activator)
	{
		m_ContextPtr->addPluginListener(_activator);
	}

	void _2RealPluginContext::removeServiceListerner(_2RealIPluginActivator& _activator)
	{
		m_ContextPtr->removeServiceListener(_activator);
	}

	void _2RealPluginContext::removeFrameworkListerner(_2RealIPluginActivator& _activator)
	{
		m_ContextPtr->removeFrameworkListener(_activator);
	}

	void _2RealPluginContext::removePluginListerner(_2RealIPluginActivator& _activator)
	{
		m_ContextPtr->removePluginListener(_activator);
	}
}