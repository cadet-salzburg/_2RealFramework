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
#include "_2RealIdentifier.h"
#include "_2RealEntities.h"
#include "_2RealMetadata.h"
#include "_2RealContainer.h"

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
			for (ServiceList::iterator it = ref.second.begin();  it != ref.second.end(); it++)
			{
				*it = NULL;
			}
		}
	}

	const Identifier ServiceFactory::registerService(std::string const& _name, Plugin *const _plugin, Metadata const *const _metadata, ServiceCreator _creator) throw(...)
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
			ServiceReference ref(factory, ServiceList());
			m_References.insert(NamedServiceReference(factory->id(), ref));
			return id.first;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	const bool ServiceFactory::canCreate(unsigned int const& _id) const throw (...)
	{
		ReferenceTable::const_iterator it = m_References.find(_id);
		
		if (it != m_References.end())
		{
			ServiceReference ref = it->second;
			return ref.first->canCreate();
		}

		throw Exception::failure();
	}

	const bool ServiceFactory::isSingleton(unsigned int const& _id) const throw(...)
	{
		ReferenceTable::const_iterator it = m_References.find(_id);
		
		if (it != m_References.end())
		{
			ServiceReference ref = it->second;
			return ref.first->isSingleton();
		}

		throw Exception::failure();
	}

	const bool ServiceFactory::canReconfigure(unsigned int const& _id) const throw(...)
	{
		ReferenceTable::const_iterator it = m_References.find(_id);
		
		if (it != m_References.end())
		{
			ServiceReference ref = it->second;
			return ref.first->canReconfigure();
		}

		throw Exception::failure();
	}

	const Identifier ServiceFactory::createService(std::string const& _name, Identifier const& _id, Identifiers &_setupIDs) throw (...)
	{	
		try
		{
			ReferenceTable::iterator it = m_References.find(_id.id());
			if (it == m_References.end())
			{
				throw Exception::failure();
			}		

			ServiceReference ref = it->second;
			FactoryReference factory = *ref.first;
			ServiceList services = ref.second;

			//service does not get identifier
			IService *userService = factory.create();

			const Metadata *data = factory.metadata();
			//TODO: get metadata & store
			
			//service container id shares name of service
			const Entities::ID id = m_Entities->createService(_name, NULL, userService);
			ServiceImpl *service = static_cast< ServiceImpl * >(id.second);
			
			//save container id
			services.push_back(service);

			return id.first;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

}