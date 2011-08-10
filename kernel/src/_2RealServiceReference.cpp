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

#include "_2RealServiceReference.h"

#include "_2RealIUserService.h"
#include "_2RealPlugin.h"

namespace _2Real
{
	ServiceReference::ServiceReference(Plugin const *const _pluginPtr, UserServiceCreator _creator, bool const& _singleton) :
		m_PluginPtr(_pluginPtr), m_ServiceCreator(_creator), m_bIsSingleton(_singleton), m_iNrOfCreations(0)
	{
	}

	bool const& ServiceReference::isSingleton() const
	{
		return m_bIsSingleton;
	}

	const bool ServiceReference::canCreate() const
	{
		if (m_PluginPtr->state() == Plugin::ACTIVE)
		{
			return true;
		}

		return false;
	}

	unsigned int const& ServiceReference::creationCount() const
	{
		return m_iNrOfCreations;
	}

	UserServicePtr ServiceReference::create()
	{
		UserServicePtr result;
		try
		{
			result = m_ServiceCreator();
		}
		catch (...)
		{
			std::cout << "TODO: error handling!" << std::endl;
		}

		if (!m_bIsSingleton)
		{
			m_iNrOfCreations++;
		}

		return result;
	}
}