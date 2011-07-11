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
	returned by the framework context when a service is registered
	can be used by the plugin activator to unregister a service
*/

namespace _2Real
{
	class _2RealServiceRegistration
	{
	public:

		~_2RealServiceRegistration();
		
		void unregister();
		const _2RealServicePtr servicePtr() const;
		const std::string& serviceName() const;
		const std::string& pluginName() const;

	private:

		_2RealContextPrivate*		m_ContextPtr;
		_2RealServicePtr			m_ServicePtr;
		std::string					m_ServiceName;
		std::string					m_PluginName;

		_2RealServiceRegistration(const std::string& _name, const std::string& _plugin, _2RealContextPrivate* _context, _2RealServicePtr& _service);

		friend class _2RealContextPrivate;
	};
}