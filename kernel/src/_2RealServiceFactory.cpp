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
#include "_2RealServiceParam.h"
#include "_2RealFactoryReference.h"
#include "_2RealProductionGraphs.h"
#include "_2RealException.h"
#include "_2RealServiceImpl.h"
#include "_2RealIdentifier.h"
#include "_2RealEntities.h"
#include "_2RealServiceMetadata.h"
#include "_2RealContainer.h"
#include "_2RealPlugin.h"

#include <sstream>

namespace _2Real
{

	ServiceFactory::ServiceFactory() : m_Entities(NULL), m_Plugins(NULL)
	{
	}

	ServiceFactory::ServiceFactory(ServiceFactory const& _src)
	{
		throw Exception::noCopy();
	}

	ServiceFactory& ServiceFactory::operator=(ServiceFactory const& _src)
	{
		throw Exception::noCopy();
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
			for (ServiceList::iterator it = ref.second->begin();  it != ref.second->end(); it++)
			{
				*it = NULL;
			}

			delete ref.second;
			ref.second = NULL;
		}
	}

	const Identifier ServiceFactory::registerService(std::string const& _name, Plugin *const _plugin, ServiceMetadata const& _metadata, ServiceCreator _creator)
	{
		try
		{
			//check if service w/ same name was already registered by same plugin
			for (ReferenceTable::iterator it = m_References.begin(); it != m_References.end(); it++)
			{
				ServiceReference ref = it->second;
				if (ref.first->name() == _name && ref.first->plugin() == _plugin)
				{
					throw Exception::failure();
				}
			}

			const Entities::ID id = m_Entities->createFactoryRef(_name, _plugin, _creator, _metadata);
			FactoryReference *factory = static_cast< FactoryReference * >(id.second);
			ServiceList *services = new ServiceList();
			ServiceReference ref(factory, services);
			m_References.insert(NamedServiceReference(factory->id(), ref));
			return id.first;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	const bool ServiceFactory::canCreate(unsigned int const& _id) const
	{
		ReferenceTable::const_iterator it = m_References.find(_id);
		
		if (it != m_References.end())
		{
			ServiceReference ref = it->second;
			return ref.first->canCreate();
		}

		throw Exception::failure();
	}

	const bool ServiceFactory::isSingleton(unsigned int const& _id) const
	{
		ReferenceTable::const_iterator it = m_References.find(_id);
		
		if (it != m_References.end())
		{
			ServiceReference ref = it->second;
			return ref.first->isSingleton();
		}

		throw Exception::failure();
	}

	const bool ServiceFactory::canReconfigure(unsigned int const& _id) const
	{
		ReferenceTable::const_iterator it = m_References.find(_id);
		
		if (it != m_References.end())
		{
			ServiceReference ref = it->second;
			return ref.first->canReconfigure();
		}

		throw Exception::failure();
	}

	const Identifier ServiceFactory::createService(std::string const& _name, unsigned int const& _id, Identifiers &_setupIDs, Identifier const& _top)
	{
		try
		{
			//check if _top is valid
			if (!m_Graphs->isNirvana(_top))
			{
				throw Exception::failure();
			}

			//find factory ref & service list
			ReferenceTable::iterator it = m_References.find(_id);
			if (it == m_References.end())
			{
				throw Exception::failure();
			}

			FactoryReference *factory = it->second.first;
			ServiceList *services = it->second.second;

			//service does not get identifier
			IService *userService = factory->create();

			//service container id shares name of service
			const Entities::ID id = m_Entities->createService(_name, userService);
			ServiceImpl *service = static_cast< ServiceImpl * >(id.second);
			Identifier i = id.first;
			std::cout << i.name() << std::endl;

			//this is the services metadata
			const ServiceMetadata data = factory->metadata();

			//now, what is needed are input / output / setup ids
			typedef std::list< std::string > ParamList;
			ParamList setup = data.getSetupParams();
			ParamList input = data.getInputParams();
			ParamList output = data.getOutputParams();

			for (ParamList::iterator it = setup.begin(); it != setup.end(); it++)
			{
				const Entities::ID i = m_Entities->createServiceParam(*it, service, IdentifierImpl::SETUP);
				ServiceParam *p = static_cast< ServiceParam * >(i.second);
				service->addParam(p);
				//save setup param
				_setupIDs.push_back(i.first);
			}

			for (ParamList::iterator it = input.begin(); it != input.end(); it++)
			{
				const Entities::ID i = m_Entities->createServiceParam(*it, service, IdentifierImpl::SETUP);
				ServiceParam *p = static_cast< ServiceParam * >(i.second);
				service->addParam(p);
			}

			for (ParamList::iterator it = output.begin(); it != output.end(); it++)
			{
				const Entities::ID i = m_Entities->createServiceParam(*it, service, IdentifierImpl::SETUP);
				ServiceParam *p = static_cast< ServiceParam * >(i.second);
				service->addParam(p);
			}

			//save container id
			services->push_back(service);

			return id.first;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	const Identifier ServiceFactory::createService(std::string const& _name, unsigned int const& _id, std::string const& _service, Identifiers &_setupIDs, Identifier const& _top)
	{
		//std::cout << _id << " " << _service << std::endl;
		try
		{
			//check if _top is valid
			if (!m_Graphs->isNirvana(_top))
			{
				throw Exception::failure();
			}

			//find factory ref & service list
			FactoryReference *factory;
			ServiceList *services;
			for (ReferenceTable::iterator it = m_References.begin(); it != m_References.end(); it++)
			{
				factory = it->second.first;
				if (factory->plugin()->id() == _id && factory->name() == _service)
				{
					services = it->second.second;
					break;
				}
			}

			//service does not get identifier
			IService *userService = factory->create();

			//service container id shares name of service
			const Entities::ID id = m_Entities->createService(_name, userService);
			ServiceImpl *service = static_cast< ServiceImpl * >(id.second);

			//this is the services metadata
			const ServiceMetadata data = factory->metadata();

			//now, what is needed are input / output / setup ids
			typedef std::list< std::string > ParamList;
			ParamList setup = data.getSetupParams();
			ParamList input = data.getInputParams();
			ParamList output = data.getOutputParams();

			for (ParamList::iterator it = setup.begin(); it != setup.end(); it++)
			{
				const Entities::ID i = m_Entities->createServiceParam(*it, service, IdentifierImpl::SETUP);
				ServiceParam *p = static_cast< ServiceParam * >(i.second);
				service->addParam(p);
				//save setup param
				_setupIDs.push_back(i.first);
			}

			for (ParamList::iterator it = input.begin(); it != input.end(); it++)
			{
				const Entities::ID i = m_Entities->createServiceParam(*it, service, IdentifierImpl::SETUP);
				ServiceParam *p = static_cast< ServiceParam * >(i.second);
				service->addParam(p);
			}

			for (ParamList::iterator it = output.begin(); it != output.end(); it++)
			{
				const Entities::ID i = m_Entities->createServiceParam(*it, service, IdentifierImpl::SETUP);
				ServiceParam *p = static_cast< ServiceParam * >(i.second);
				service->addParam(p);
			}

			//save container id
			services->push_back(service);

			//done
			return id.first;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	ServiceMetadata const& ServiceFactory::serviceInfo(unsigned int const& _id) const
	{
		ReferenceTable::const_iterator it = m_References.find(_id);

		if (it == m_References.end())
		{
			throw Exception::failure();
		}
	
		FactoryReference *ref = it->second.first;
		return ref->metadata();
	}

	ServiceMetadata const& ServiceFactory::serviceInfo(unsigned int const& _id, std::string const& _name) const
	{
		for (ReferenceTable::const_iterator it = m_References.begin(); it != m_References.end(); it++)
		{
			FactoryReference *ref = it->second.first;
			if (ref->plugin()->id() == _id && ref->name() == _name)
			{
				return ref->metadata();
			}
		}

		throw Exception::failure();
	}
}