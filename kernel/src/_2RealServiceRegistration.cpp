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

#include "_2RealServiceRegistration.h"
#include "_2RealContext.h"
#include "_2RealIService.h"

namespace _2Real
{
	_2RealServiceRegistration::_2RealServiceRegistration(const std::string& _name, const std::string& _plugin, _2RealContextPrivate* _context, _2RealServicePtr& _service) 
											: m_ContextPtr(_context), m_ServicePtr(_service), m_ServiceName(_name), m_PluginName(_plugin)
	{
	}

	_2RealServiceRegistration::~_2RealServiceRegistration()
	{
	}

	void _2RealServiceRegistration::unregister()
	{
		m_ContextPtr->unregisterService(m_ServiceName, m_PluginName, m_ServicePtr);
	}

	const _2RealServicePtr _2RealServiceRegistration::servicePtr() const
	{
		return m_ServicePtr;
	}

	const std::string& _2RealServiceRegistration::serviceName() const
	{
		return m_ServiceName;
	}

	const std::string& _2RealServiceRegistration::pluginName() const
	{
		return m_PluginName;
	}
}