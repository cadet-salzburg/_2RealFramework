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
#include "_2RealServiceSlot.h"
#include "_2RealServiceValue.h"
#include "_2RealFactoryReference.h"
#include "_2RealProductionGraphs.h"
#include "_2RealException.h"
#include "_2RealServiceImpl.h"
#include "_2RealEntities.h"
#include "_2RealServiceMetadata.h"
#include "_2RealContainer.h"
#include "_2RealPlugin.h"

#include <sstream>
#include <iostream>

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

	const unsigned int ServiceFactory::registerService(std::string const& _name, Plugin *const _plugin, ServiceMetadata const& _metadata, ServiceCreator _creator)
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
			//!
			services->push_back(NULL);
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

	const unsigned int ServiceFactory::createService(std::string const& _name, unsigned int const& _id, std::list< unsigned int > &_ids, unsigned int const& _top)
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

			if (services == NULL || factory == NULL)
			{
				throw Exception::failure();
			}

			//service does not get identifier
			IService *userService = factory->create();

			if (userService == NULL)
			{
				throw Exception::failure();
			}

			//service container id shares name of service
			const Entities::ID id = m_Entities->createService(_name, userService);
			ServiceImpl *service = static_cast< ServiceImpl * >(id.second);
			unsigned int serviceID = id.first;

			//this is the services metadata
			const ServiceMetadata data = factory->metadata();

			//now, what is needed are input / output / setup ids
			typedef std::list< std::string > ParamList;
			ParamList setup = data.getSetupParams();
			ParamList input = data.getInputParams();
			ParamList output = data.getOutputParams();

			for (ParamList::iterator it = setup.begin(); it != setup.end(); it++)
			{
				const Entities::ID e = m_Entities->createServiceValue(*it, service);
				ServiceValue *p = static_cast< ServiceValue* >(e.second);
				service->addValue(e.first, p);
				//save setup param
				_ids.push_back(e.first);
			}

			for (ParamList::iterator it = input.begin(); it != input.end(); it++)
			{
				const Entities::ID e = m_Entities->createInputSlot(*it, service);
				ServiceSlot *p = static_cast< ServiceSlot * >(e.second);
				service->addSlot(e.first, p);
			}

			for (ParamList::iterator it = output.begin(); it != output.end(); it++)
			{
				const Entities::ID e = m_Entities->createOutputSlot(*it, service);
				ServiceSlot *p = static_cast< ServiceSlot * >(e.second);
				service->addSlot(e.first, p);
			}

			//add service to nirvanas children
			IEntity *e = m_Entities->get(_top);
			Container *top = static_cast< Container * >(e);
			top->add(service, 0);

			//save container id
			services->push_back(service);

			//done
			return serviceID;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	const unsigned int ServiceFactory::createService(std::string const& _name, unsigned int const& _id, std::string const& _service, std::list< unsigned int > &_ids, unsigned int const& _top)
	{
		try
		{
			//check if _top is valid
			if (!m_Graphs->isNirvana(_top))
			{
				throw Exception::failure();
			}

			//find factory ref & service list
			FactoryReference *factory = NULL;
			ServiceList *services = NULL;
			for (ReferenceTable::iterator it = m_References.begin(); it != m_References.end(); it++)
			{
				factory = it->second.first;
				if (factory->plugin()->id() == _id && factory->name() == _service)
				{
					services = it->second.second;
					break;
				}
			}

			if (services == NULL || factory == NULL)
			{
				std::cout << "FACTORY: NOTEXISTANT REF! " << _service << " " << _id << std::endl;
				throw Exception::failure();
			}

			//service does not get identifier
			IService *userService = factory->create();

			if (userService == NULL)
			{
				throw Exception::failure();
			}

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
				const Entities::ID e = m_Entities->createServiceValue(*it, service);
				ServiceValue *p = static_cast< ServiceValue* >(e.second);
				service->addValue(e.first, p);
				//save setup param
				_ids.push_back(e.first);
			}

			for (ParamList::iterator it = input.begin(); it != input.end(); it++)
			{
				const Entities::ID i = m_Entities->createInputSlot(*it, service);
				ServiceSlot *p = static_cast< ServiceSlot * >(i.second);
				service->addSlot(i.first, p);
			}

			for (ParamList::iterator it = output.begin(); it != output.end(); it++)
			{
				const Entities::ID i = m_Entities->createOutputSlot(*it, service);
				ServiceSlot *p = static_cast< ServiceSlot * >(i.second);
				service->addSlot(i.first, p);
			}

			//add service to nirvanas children
			IEntity *e = m_Entities->get(_top);
			Container *top = static_cast< Container * >(e);
			top->add(service, 0);

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