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

#include "_2RealServiceFactory.h"

#include "_2RealIService.h"
#include "_2RealPlugin.h"
#include "_2RealServiceContainer.h"
#include "_2RealSequenceContainer.h"
#include "_2RealSynchronizationContainer.h"
#include "_2RealOutputContainer.h"

#include <sstream>

namespace _2Real
{

	ServiceFactory::ServiceFactory() : m_iCreationCount(0)
	{
		//registerFrameworkService("ServiceContainer", &ServiceFactory::createContainer< ServiceContainer >, false);
		//registerFrameworkService("OutputContainer", &ServiceFactory::createContainer< OutputContainer >, false);
		//registerFrameworkService("SequenceContainer", &ServiceFactory::createContainer< SequenceContainer >, false);
		//registerFrameworkService("SynchronizationContainer", &ServiceFactory::createContainer< SynchronizationContainer >, false);
	}

	void ServiceFactory::registerService(std::string const& _name, PluginPtr _pluginPtr, UserServiceCreator _creator, bool const& _singleton)
	{

		ServiceRefPtr ref = ServiceRefPtr(new ServiceReference(_pluginPtr, _creator, _singleton));

		std::string service = _pluginPtr->name() + "." + _name;

		m_ReferenceMap.insert(NamedServiceRefPtr(service, ref));
	}

	/*
	void ServiceFactory::registerFrameworkService(std::string const& _name, ServiceCreator _factory, bool const& _singleton)
	{
		PluginPtr pluginPtr = PluginPtr();
		ServiceRefPtr ref = ServiceRefPtr(new ServiceReference(pluginPtr, _factory, _singleton, true));

		std::string service =  "Framework." + _name;

		m_ReferenceMap.insert(NamedServiceRefPtr(service, ref));
	}
	*/

	const bool ServiceFactory::canCreate(std::string const& _name, std::string const& _plugin) const
	{
		std::string service = _plugin + "." + _name;
		NamedReferenceMap::const_iterator it = m_ReferenceMap.find(service);
		
		if (it != m_ReferenceMap.end())
		{
			return it->second->canCreate();
		}

		return false;
	}

	const bool ServiceFactory::isSingleton(std::string const& _name, std::string const& _plugin) const
	{
		std::string service = _plugin + "." + _name;
		NamedReferenceMap::const_iterator it = m_ReferenceMap.find(service);
		
		if (it != m_ReferenceMap.end())
		{
			return it->second->isSingleton();
		}

		return false;
	}

	NamedServicePtr ServiceFactory::createService(std::string const& _name, std::string const& _plugin)
	{
		std::string service = _plugin + "." + _name;
		NamedReferenceMap::iterator it = m_ReferenceMap.find(service);

		ServicePtr servicePtr;
		ServiceName name;
		if (it != m_ReferenceMap.end())
		{
			ServiceRefPtr ref = it->second;

			if (ref->canCreate())
			{

				servicePtr = ref->create();

				if (servicePtr.isNull())
				{
					std::cout << "TODO: error handling; could not create service" << std::endl;
				}
				else
				{
					m_iCreationCount++;
				}

				name = generateName(*it);
			}
		}

		NamedServicePtr result(name, servicePtr);
		m_ServiceMap.insert(result);

		return result;
	}

	const ServiceName ServiceFactory::generateName(NamedServiceRefPtr const& _ref) const
	{
#ifdef _DEBUG
		std::stringstream tmp;
		tmp << _ref.second->creationCount();
		return _ref.first + "." + tmp.str();
#else
		return m_iCreationCount;
#endif
	}

}