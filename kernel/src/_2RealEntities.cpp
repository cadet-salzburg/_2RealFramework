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

#include "_2RealEntities.h"
#include "_2RealEngineTypedefs.h"
#include "_2RealIEntity.h"
#include "_2RealIdentifier.h"
#include "_2RealIdentifierImpl.h"
#include "_2RealException.h"

#include "_2RealPlugin.h"
#include "_2RealServiceImpl.h"
#include "_2RealFactoryReference.h"
#include "_2RealContainer.h"
#include "_2RealServiceSlot.h"
#include "_2RealServiceValue.h"

#include <sstream>

namespace _2Real
{

	EntityTable::EntityTable() : m_iCreationCount(0), m_Factory(NULL), m_Plugins(NULL)
	{
	}

	EntityTable::EntityTable(EntityTable const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	EntityTable& EntityTable::operator=(EntityTable const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	EntityTable::~EntityTable()
	{
		for (EntityMap::iterator it = m_Entities.begin(); it != m_Entities.end(); it++)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	void EntityTable::destroy(Container *_obj)
	{
		try
		{
			if (_obj->type() != Entity::NIRVANA)
			{
				_obj->resetIO();
			}
			std::list< unsigned int > ids;
			std::list< unsigned int >::iterator it;
			ids = _obj->children();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			delete _obj;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void EntityTable::destroy(ServiceContainer *_obj)
	{
		try
		{
			_obj->resetIO();
			
			IDs ids;
			IDIterator it;
			
			ids = _obj->setupParamIDs();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			ids = _obj->inputSlotIDs();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			ids = _obj->outputSlotIDs();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			delete _obj;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void EntityTable::destroy(ServiceSlot *_obj)
	{
		try
		{
			delete _obj;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void EntityTable::destroy(ServiceValue *_obj)
	{
		try
		{
			delete _obj;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void EntityTable::destroy(Plugin *_obj)
	{
		try
		{
			std::list< unsigned int > ids;
			std::list< unsigned int >::iterator it;
			ids = _obj->serviceIDs();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			delete _obj;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void EntityTable::destroy(FactoryReference *_obj)
	{
		try
		{
			delete _obj;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void EntityTable::destroy(unsigned int const& _id)
	{
		EntityMap::iterator it = m_Entities.find(_id);

		if (it != m_Entities.end())
		{
			Entity *e = it->second;
			Entity::eType type = e->type();

			if (type == Entity::SERVICE)
			{
				ServiceContainer *service = static_cast< ServiceContainer * >(e);
				destroy(service);
			}
			else if (type == Entity::NIRVANA || e->type() == Entity::SEQUENCE || type == Entity::SYNCHRONIZATION)
			{
				Container *container = static_cast< Container * >(e);
				destroy(container);
			}
			else if (type == Entity::INPUT || type == Entity::OUTPUT)
			{
				ServiceSlot *slot = static_cast< ServiceSlot * >(e);
				destroy(slot);
			}
			else if (type == Entity::SETUP)
			{
				ServiceValue *value = static_cast< ServiceValue * >(e);
				destroy(value);
			}
			else if (type == Entity::PLUGIN)
			{
				Plugin *plugin = static_cast< Plugin * >(e);
				destroy(plugin);
			}
			else if (type == Entity::FACTORY)
			{
				FactoryReference *ref = static_cast< FactoryReference * >(e);
				destroy(ref);
			}

			it->second = NULL;
			m_Entities.erase(it);
		}
		else
		{
			throw Exception("entity destruction failed - non-existant entity");
		}
	}

	Entity *const EntityTable::get(unsigned int const& _id)
	{
		EntityMap::iterator it = m_Entities.find(_id);

		if (it != m_Entities.end())
		{
			return it->second;
		}

		throw Exception("could not get entity - id not found");
	}

	const Identifier EntityTable::getIdentifier(unsigned int const& _id) const
	{
		EntityMap::const_iterator it = m_Entities.find(_id);

		if (it != m_Entities.end())
		{
			return Identifier(it->second->m_ID);
		}

		throw Exception("could not get entity's identifier - id not found");
	}

	const EntityTable::ID EntityTable::createPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class)
	{

		++m_iCreationCount;

		//plugin info
		std::stringstream info;
		info << "i am a plugin" << std::endl;
		info << "creation id: " << m_iCreationCount << std::endl;
		info << "chosen name: " << _name << std::endl;
		info << "directory:   " << _dir << std::endl;
		info << "filename:    " << _file << std::endl;
		info << "classname:   " << _class << std::endl;

		IdentifierImpl *id = new IdentifierImpl(_name, "plugin", info.str(), Entity::PLUGIN, m_iCreationCount);
		Plugin *plugin = new Plugin(_dir, _file, _class, m_Factory, id);
		m_Entities.insert(NamedEntity(id->id(), plugin));
		return EntityTable::ID(id->id(), plugin);

	}

	const EntityTable::ID EntityTable::createService(std::string const& _name, IService *const _service)
	{
		try
		{
			IdentifierImpl *id = new IdentifierImpl(_name, "service", "", Entity::SERVICE, ++m_iCreationCount);
			ServiceContainer *service = new ServiceContainer(_service, id);
			m_Entities.insert(NamedEntity(id->id(), service));
			return EntityTable::ID(id->id(), service);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const EntityTable::ID EntityTable::createContainer(std::string const& _name, Entity::eType const& _type)
	{
		try
		{
			IdentifierImpl *id;
			Container *container;
			switch(_type)
			{

			case Entity::SEQUENCE:

				id = new IdentifierImpl(_name, "sequence", "", Entity::SEQUENCE, ++m_iCreationCount);
				break;

			case Entity::SYNCHRONIZATION:
				
				id = new IdentifierImpl(_name, "synchronization", "", Entity::SYNCHRONIZATION, ++m_iCreationCount);
				break;

			case Entity::NIRVANA:
				id = new IdentifierImpl(_name, "nirvana", "", Entity::NIRVANA, ++m_iCreationCount);
				break;
			}

			container = new Container(id);
			m_Entities.insert(NamedEntity(id->id(), container));
			return EntityTable::ID(id->id(), container);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const EntityTable::ID EntityTable::createFactoryRef(std::string const& _name, Plugin const *const _plugin, ServiceCreator _creator, ServiceMetadata const& _metadata)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "factory", _plugin->name(), Entity::FACTORY, ++m_iCreationCount);
		FactoryReference *ref = new FactoryReference(_name, _plugin, _creator, _metadata, id);
		m_Entities.insert(NamedEntity(id->id(), ref));
		return EntityTable::ID(id->id(), ref);
	}

	const EntityTable::ID EntityTable::createServiceValue(std::string const& _name, ServiceContainer *const _service)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "setup value", "", Entity::SETUP, ++m_iCreationCount);
		ServiceValue *val = new ServiceValue(id, _service);
		m_Entities.insert(NamedEntity(id->id(), val));
		return EntityTable::ID(id->id(), val);
	}

	const EntityTable::ID EntityTable::createInputSlot(std::string const& _name, ServiceContainer *const _service)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "input slot", "", Entity::INPUT, ++m_iCreationCount);
		ServiceSlot *slot = new ServiceSlot(id, _service);
		m_Entities.insert(NamedEntity(id->id(), slot));
		return EntityTable::ID(id->id(), slot);
	}

	const EntityTable::ID EntityTable::createOutputSlot(std::string const& _name, ServiceContainer *const _service)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "output slot", "", Entity::OUTPUT, ++m_iCreationCount);
		ServiceSlot *slot = new ServiceSlot(id, _service);
		m_Entities.insert(NamedEntity(id->id(), slot));
		return EntityTable::ID(id->id(), slot);
	}
}