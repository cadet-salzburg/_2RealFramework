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

#include "_2RealInternalService.h"
#include "_2RealIService.h"
#include "_2RealIUserService.h"
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
	}

	void ServiceFactory::registerService(std::string const& _name, PluginPtr _pluginPtr, UserServiceCreator _creator, bool const& _singleton)
	{

		ServiceRefPtr ref = ServiceRefPtr(new ServiceReference(_pluginPtr, _creator, _singleton));

		std::string service = _pluginPtr->name() + "." + _name;

		m_ReferenceMap.insert(NamedServiceRefPtr(service, ref));
	}

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

	OutputContainerPtr ServiceFactory::createOutputContainer()
	{
		ServiceName name;
#ifdef _DEBUG
		std::string tmp = "Framework.OutputContainer";
		name = generateContainerName(tmp);
#else
		name = generateContainerName();
#endif
		return OutputContainerPtr(new OutputContainer(name));
	}

	GroupContainerPtr ServiceFactory::createSequenceContainer()
	{
		ServiceName name;
#ifdef _DEBUG
		std::string tmp = "Framework.SequenceContainer";
		name = generateContainerName(tmp);
#else
		name = generateContainerName();
#endif
		return GroupContainerPtr(new SequenceContainer(name));
	}

	GroupContainerPtr ServiceFactory::createSynchronizationContainer()
	{
		ServiceName name;
#ifdef _DEBUG
		std::string tmp = "Framework.SynchronizationContainer";
		name = generateContainerName(tmp);
#else
		name = generateContainerName();
#endif
		return GroupContainerPtr(new SynchronizationContainer(name));
	}

	NamedServicePtr ServiceFactory::createService(std::string const& _name, std::string const& _plugin)
	{
		std::string service = _plugin + "." + _name;
		NamedReferenceMap::iterator it = m_ReferenceMap.find(service);

		UserServicePtr servicePtr;
		ServicePtr internalServicePtr;
		ServicePtr containerServicePtr;
		ServiceName name;
		if (it != m_ReferenceMap.end())
		{
			ServiceRefPtr ref = it->second;

			if (ref->canCreate())
			{

				std::cout << "service factory: creating service " << _name << " " << _plugin << std::endl; 
				servicePtr = ref->create();

				if (servicePtr.isNull())
				{
					std::cout << "TODO: error handling; could not create service" << std::endl;
				}
#ifndef _DEBUG
				else
				{
					m_iCreationCount++;
				}
#endif

				name = generateServiceName(*it);
			}
		}

		std::cout << name << std::endl;
		internalServicePtr = ServicePtr(new InternalService(name, servicePtr));

		ServiceName containerName;
#ifdef _DEBUG
		containerName = generateContainerName("Framework.ServiceContainer");
#else
		containerName = generateContainerName();
#endif

		containerServicePtr = ServicePtr(new ServiceContainer(containerName, internalServicePtr));

		//store container
		NamedServicePtr result(name, containerServicePtr);
		m_ServiceMap.insert(result);

		return result;
	}

	const ServiceName ServiceFactory::generateServiceName(NamedServiceRefPtr const& _ref) const
	{
#ifdef _DEBUG
		std::stringstream tmp;
		tmp << _ref.second->creationCount();
		return _ref.first + "." + tmp.str();
#else
		return m_iCreationCount;
#endif
	}

#ifdef _DEBUG
	const ServiceName ServiceFactory::generateContainerName(std::string const& container) const
	{
		std::stringstream tmp;
		tmp <<  m_iCreationCount;
		return container + "." + tmp.str();
	}
#else
	const ServiceName ServiceFactory::generateContainerName() const
	{
		return m_iCreationCount;
	}
#endif

}