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
#include "_2RealServiceContainer.h"
#include "_2RealEntityTable.h"
#include "_2RealServiceMetadata.h"
#include "_2RealContainer.h"
#include "_2RealPlugin.h"

#include <sstream>
#include <iostream>

namespace _2Real
{

	ServiceFactory::ServiceFactory() :
		m_EntityTable(NULL)
	{
	}

	ServiceFactory::ServiceFactory(ServiceFactory const& _src)
	{
		throw Exception("internal error: attempted to copy service factory");
	}

	ServiceFactory& ServiceFactory::operator=(ServiceFactory const& _src)
	{
		throw Exception("internal error: attempted to copy service factory");
	}

	ServiceFactory::~ServiceFactory()
	{
		//for (ReferenceTable::iterator it = m_References.begin(); it != m_References.end(); it++)
		//{
		//	try
		//	{
		//		unsigned int id = it->first;
		//		m_EntityTable->destroy(it->second);
		//	}
		//	catch (Exception &e)
		//	{
		//		std::cout << "error on service factory destruction: " << e.what() << std::endl;
		//	}
		//	catch (...)
		//	{
		//		std::cout << "error on service factory destruction" << std::endl;
		//	}
		//}
	}

	FactoryReference const *const ServiceFactory::ref(unsigned int const& _plugin, std::string const& _service) const
	{
		ReferenceTable::const_iterator it;
		for (it = m_References.begin(); it != m_References.end(); it++)
		{
			FactoryReference *ref = it->second;
			if (ref->name() == _service && ref->plugin() == _plugin)
			{
				break;
			}
		}

		if (it == m_References.end())
		{
			throw Exception("could not retrieve factory ref - does not exist");
		}
		else if (!it->second)
		{
			throw Exception("internal error - null pointer stored in service factory");
		}

		return it->second;
	}

	FactoryReference *const ServiceFactory::ref(unsigned int const& _plugin, std::string const& _service)
	{
		ReferenceTable::iterator it;
		for (it = m_References.begin(); it != m_References.end(); it++)
		{
			FactoryReference *ref = it->second;
			if (ref->name() == _service && ref->plugin() == _plugin)
			{
				break;
			}
		}

		if (it == m_References.end())
		{
			throw Exception("could not retrieve factory ref - does not exist");
		}
		else if (!it->second)
		{
			throw Exception("internal error - null pointer stored in service factory");
		}

		return it->second;
	}

	FactoryReference *const ServiceFactory::registerService(std::string const& _name, unsigned int const& _pluginID, ServiceMetadata const& _metadata, ServiceCreator _creator)
	{
		try
		{
			//check if service w/ same name was already registered by same plugin
			for (ReferenceTable::iterator it = m_References.begin(); it != m_References.end(); it++)
			{
				FactoryReference *ref = it->second;
				if (ref->name() == _name && ref->plugin() == _pluginID)
				{
					throw Exception("could not register service - service with same name was already registered by plugin");
				}
			}

			FactoryReference *factory = m_EntityTable->createFactoryRef(_name, _pluginID, _creator, _metadata);
			m_References.insert(NamedReference(factory->id(), factory));
			return factory;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	ServiceContainer *const ServiceFactory::createService(std::string const& _name, unsigned int const& _plugin, std::string const& _service)
	{
		try
		{
			FactoryReference *factory = ref(_plugin, _service);
			IService *userService = factory->create();

			if (userService == NULL)
			{
				throw Exception("internal error: could not create instance of service " + _service);
			}

			//service container gets identifier
			ServiceContainer *service = m_EntityTable->createService(_name, userService);
			const ServiceMetadata data = factory->metadata();

			//get setup / input / output from metadata
			typedef std::map< std::string, std::string > StringMap;
			StringMap setup = data.getSetupParams();
			StringMap input = data.getInputParams();
			StringMap output = data.getOutputParams();

			//add to service container
			for (StringMap::iterator it = setup.begin(); it != setup.end(); it++)
			{
				ServiceValue *val = m_EntityTable->createServiceValue(it->first, it->second, service);
				service->addSetupValue(val->id(), val);
			}

			for (StringMap::iterator it = input.begin(); it != input.end(); it++)
			{
				ServiceSlot *slot = m_EntityTable->createInputSlot(it->first, it->second, service);
				service->addInputSlot(slot->id(), slot);
			}

			for (StringMap::iterator it = output.begin(); it != output.end(); it++)
			{
				ServiceSlot *slot = m_EntityTable->createOutputSlot(it->first, it->second, service);
				service->addOutputSlot(slot->id(), slot);
			}

			//done
			return service;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	ServiceMetadata const& ServiceFactory::info(unsigned int const& _plugin, std::string const& _service) const
	{
		try
		{
			const FactoryReference *factory = ref(_plugin, _service);
			return factory->metadata();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	std::list< FactoryReference * > ServiceFactory::getServices(unsigned int const& _plugin)
	{
		try
		{
			std::list< FactoryReference * > result;
			for (ReferenceTable::iterator it = m_References.begin(); it != m_References.end(); it++)
			{
				FactoryReference *ref = it->second;
				if (ref->plugin() == _plugin)
				{
					result.push_back(ref);
				}
			}
			return result;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	IDs ServiceFactory::getServiceIDs(unsigned int const& _plugin) const
	{
		try
		{
			IDs result;
			for (ReferenceTable::const_iterator it = m_References.begin(); it != m_References.end(); it++)
			{
				FactoryReference *ref = it->second;
				if (ref->plugin() == _plugin)
				{
					result.push_back(ref->id());
				}
			}
			return result;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}