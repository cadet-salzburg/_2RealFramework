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
#include "_2RealFactoryReference.h"
#include "_2RealException.h"
#include "_2RealServiceImpl.h"
#include "_2RealEntities.h"
#include "_2RealMetadata.h"
//#include "_2RealProductionTreeImpl.h"

#include <sstream>

namespace _2Real
{

	ServiceFactory::ServiceFactory(Entities *const _entities) : m_Entities(_entities)
	{
	}

	ServiceFactory::ServiceFactory(ServiceFactory const& _src)
	{
		throw Exception::failure();
	}

	ServiceFactory& ServiceFactory::operator=(ServiceFactory const& _src)
	{
		throw Exception::failure();
	}

	ServiceFactory::~ServiceFactory()
	{
		for (ReferenceTable::iterator it = m_References.begin(); it != m_References.end(); it++)
		{
			ServiceReference ref = it->second;
			
			//delete factory reference
			delete ref.first;
			ref.first = NULL;
			
			//service containers are not deleted here, but in the next loop
			for (ServiceList::iterator it = ref.second.begin();  it != ref.second.end(); it++)
			{
				*it = NULL;
			}
		}

		for (ContainerMap::iterator it = m_Containers.begin(); it != m_Containers.end(); it++)
		{
			//delete all containers in the framework - deleting a service container will shutdown it's user service & delete it
			delete it->second;
			it->second = NULL;
		}
	}

	IdentifierImpl const *const ServiceFactory::registerService(std::string const& _name, Plugin *const _plugin, Metadata const *const _metadata, ServiceCreator _creator) throw(...)
	{

		//check if service w/ same name was already registered by same plugin, if so do nothing (no exception, either)
		for (ReferenceTable::iterator it = m_References.begin(); it != m_References.end(); it++)
		{
			//so there is a service with the same name - the plugin might be different still
			if (it->first.name() == _name)
			{
				ServiceReference ref = it->second;
				if (ref.first->plugin() == _plugin)
				{
					//the plugin pointers are equal, so don't register anything
					return NULL;
				}
			}
		}

		try
		{
			FactoryReference *factory = new FactoryReference(_plugin, _creator, _metadata);
			const IdentifierImpl *id = m_Entities->createID(_name, factory);
			ServiceReference ref(factory, ServiceList());
			m_References.insert(NamedServiceReference(*id, ref));
			return id;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	const bool ServiceFactory::canCreate(IdentifierImpl const& _serviceID) const throw (...)
	{
		ReferenceTable::const_iterator it = m_References.find(_serviceID);
		
		if (it != m_References.end())
		{
			ServiceReference ref = it->second;
			return ref.first->canCreate();
		}

		throw Exception::failure();
	}

	const bool ServiceFactory::isSingleton(IdentifierImpl const& _serviceID) const throw(...)
	{
		ReferenceTable::const_iterator it = m_References.find(_serviceID);
		
		if (it != m_References.end())
		{
			ServiceReference ref = it->second;
			return ref.first->isSingleton();
		}

		throw Exception::failure();
	}

	const bool ServiceFactory::canReconfigure(IdentifierImpl const& _serviceID) const throw(...)
	{
		ReferenceTable::const_iterator it = m_References.find(_serviceID);
		
		if (it != m_References.end())
		{
			ServiceReference ref = it->second;
			return ref.first->canReconfigure();
		}

		throw Exception::failure();
	}

	IdentifierImpl const *const ServiceFactory::createServiceContainer(IdentifierImpl const& _serviceID) throw (...)
	{
		ReferenceTable::iterator it = m_References.find(_serviceID);
		if (it == m_References.end())
		{
			throw Exception::failure();
		}
		
		IService *userService;
		ServiceImpl *container;
		
		ServiceReference ref = it->second;
		
		try
		{
			//service does not get identifier
			userService = ref.first->create();
			
			//service container id shares name of service
			container = new ServiceImpl(userService);
			const IdentifierImpl *id = m_Entities->createID(_serviceID.name(), container);
			m_Containers.insert(NamedContainer(*id, container));
			
			//save container id
			ref.second.push_back(container);
			return id;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

}