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

	EntityTable::EntityTable() :
		m_iCreationCount(0)
	{
	}

	EntityTable::EntityTable(EntityTable const& _src)
	{
		throw Exception("internal error: attempted to copy entity table");
	}

	EntityTable& EntityTable::operator=(EntityTable const& _src)
	{
		throw Exception("internal error: attempted to copy entity table");
	}

	EntityTable::~EntityTable()
	{
		for (EntityMap::iterator it = m_Entities.begin(); it != m_Entities.end(); it++)
		{
			//TODO: destroy
			delete it->second;
		}
	}

	void EntityTable::destroy(Container *_obj)
	{
		try
		{
			EntityMap::iterator e = m_Entities.find(_obj->id());
			if (e == m_Entities.end())
			{
				throw Exception("internal error: entity not in entity map");
			}

			if (_obj->type() != Entity::SYSTEM)
			{
				_obj->resetIO();
			}
			
			std::list< AbstractContainer * > children;
			std::list< AbstractContainer * >::iterator it;
			children = _obj->children();

			for (it = children.begin(); it != children.end(); it++)
			{
				destroy(*it);
			}

			delete _obj;
			m_Entities.erase(e);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void EntityTable::destroy(AbstractContainer *_obj)
	{
		try
		{
			//
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
			//IO needs to be reset here
			//_obj->resetIO();
			//
			//std::list< ServiceValue * > setup = _obj->setupParams();
			//for (std::list< ServiceValue * >::iterator it = setup.begin(); it != setup.end(); it++)
			//{
			//	destroy(*it);
			//}

			//std::list< ServiceSlot * > input = _obj->inputSlots();
			//for (std::list< ServiceSlot * >::iterator it = input.begin(); it != input.end(); it++)
			//{
			//	destroy(*it);
			//}

			//std::list< ServiceSlot * > output = _obj->outputSlots();
			//for (std::list< ServiceSlot * >::iterator it = output.begin(); it != output.end(); it++)
			//{
			//	destroy(*it);
			//}

			//delete _obj;
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
			//delete _obj;
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
			//delete _obj;
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
			//IDs ids;
			//IDIterator it;
			//ids = _obj->serviceIDs();
			//for (it = ids.begin(); it != ids.end(); it++)
			//{
			//	destroy(*it);
			//}

			//delete _obj;
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
			//delete _obj;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Entity const *const EntityTable::get(unsigned int const& _id) const
	{
		EntityMap::const_iterator it = m_Entities.find(_id);

		if (it == m_Entities.end())
		{
			throw Exception("internal error: could not find entity in entity table");
		}
		else if (!it->second)
		{
			throw Exception("internal error: null pointer stored in entity table");
		}

		return it->second;
	}

	Entity *const EntityTable::get(unsigned int const& _id)
	{
		EntityMap::iterator it = m_Entities.find(_id);

		if (it == m_Entities.end())
		{
			throw Exception("internal error: could not find entity in entity table");
		}
		else if (!it->second)
		{
			throw Exception("internal error: null pointer stored in entity table");
		}

		return it->second;
	}

	const Identifier EntityTable::getIdentifier(unsigned int const& _id) const
	{
		EntityMap::const_iterator it = m_Entities.find(_id);

		if (it == m_Entities.end())
		{
			throw Exception("internal error: could not find entity in entity table");
		}
		else if (!it->second)
		{
			throw Exception("internal error: null pointer stored in entity table");
		}

		return Identifier(it->second->m_ID);
	}

	Plugin *const EntityTable::createPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class)
	{
		try
		{
			++m_iCreationCount;

			std::cout << "entity table creation count: " << m_iCreationCount << std::endl;


			std::stringstream info;
			info << "i am a plugin" << std::endl;
			info << "creation id: " << m_iCreationCount << std::endl;
			info << "chosen name: " << _name << std::endl;
			info << "directory:   " << _dir << std::endl;
			info << "filename:    " << _file << std::endl;
			info << "classname:   " << _class << std::endl;

			IdentifierImpl *id = new IdentifierImpl(_name, "plugin", info.str(), Entity::PLUGIN, m_iCreationCount);
			Plugin *plugin = new Plugin(_dir, _file, _class, id);
			m_Entities.insert(NamedEntity(id->id(), plugin));
			return plugin;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	ServiceContainer *const EntityTable::createService(std::string const& _name, IService *const _service)
	{
		try
		{
			++m_iCreationCount;

			std::stringstream info;
			info << "i am a service" << std::endl;
			info << "creation id:  " << m_iCreationCount << std::endl;
			info << "chosen name:  " << _name << std::endl;

			IdentifierImpl *id = new IdentifierImpl(_name, "service", info.str(), Entity::SERVICE, m_iCreationCount);
			ServiceContainer *service = new ServiceContainer(_service, id);
			m_Entities.insert(NamedEntity(id->id(), service));
			return service;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Container *const EntityTable::createSystem(std::string const& _name)
	{
		try
		{
			++m_iCreationCount;

			std::stringstream info;
			info << "i am a system" << std::endl;
			info << "creation id: " << m_iCreationCount << std::endl;
			info << "chosen name: " << _name << std::endl;

			IdentifierImpl *id;
			Container *container;
			id = new IdentifierImpl(_name, "system", info.str(), Entity::SYSTEM, m_iCreationCount);
			container = new Container(id);
			m_Entities.insert(NamedEntity(id->id(), container));
			return container;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Container *const EntityTable::createSequence(std::string const& _name)
	{
		try
		{
			++m_iCreationCount;

			std::stringstream info;
			info << "i am a sequence" << std::endl;
			info << "creation id:   " << m_iCreationCount << std::endl;
			info << "chosen name:   " << _name << std::endl;

			IdentifierImpl *id;
			Container *container;
			id = new IdentifierImpl(_name, "sequence", info.str(), Entity::SEQUENCE, m_iCreationCount);
			container = new Container(id);
			m_Entities.insert(NamedEntity(id->id(), container));
			return container;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Container *const EntityTable::createSynchronization(std::string const& _name)
	{
		try
		{
			++m_iCreationCount;

			std::stringstream info;
			info << "i am a synchronization" << std::endl;
			info << "creation id:          " << m_iCreationCount << std::endl;
			info << "chosen name:          " << _name << std::endl;

			IdentifierImpl *id;
			Container *container;
			id = new IdentifierImpl(_name, "synchronization", info.str(), Entity::SYNCHRONIZATION, m_iCreationCount);
			container = new Container(id);
			m_Entities.insert(NamedEntity(id->id(), container));
			return container;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	FactoryReference *const EntityTable::createFactoryRef(std::string const& _name, unsigned int const& _pluginID, ServiceCreator _creator, ServiceMetadata const& _metadata)
	{
		try
		{
			++m_iCreationCount;

			std::stringstream info;
			info << "i am a factory reference" << std::endl;
			info << "creation id:            " << m_iCreationCount << std::endl;
			info << "name:                   " << _name << std::endl;

			IdentifierImpl *id = new IdentifierImpl(_name, "factory", info.str(), Entity::FACTORY, m_iCreationCount);
			FactoryReference *ref = new FactoryReference(_name, _pluginID, _creator, _metadata, id);
			m_Entities.insert(NamedEntity(id->id(), ref));
			return ref;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	ServiceValue *const EntityTable::createServiceValue(std::string const& _name, ServiceContainer *const _service)
	{
		try
		{
			++m_iCreationCount;

			std::stringstream info;
			info << "i am a setup parameter" << std::endl;
			info << "creation id:          " << m_iCreationCount << std::endl;
			info << "name:                 " << _name << std::endl;

			IdentifierImpl *id = new IdentifierImpl(_name, "setup parameter", info.str(), Entity::SETUP, m_iCreationCount);
			ServiceValue *val = new ServiceValue(id, _service);
			m_Entities.insert(NamedEntity(id->id(), val));
			return val;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	ServiceSlot *const EntityTable::createInputSlot(std::string const& _name, ServiceContainer *const _service)
	{
		try
		{
			++m_iCreationCount;

			std::stringstream info;
			info << "i am an inpu slot" << std::endl;
			info << "creation id:     " << m_iCreationCount << std::endl;
			info << "name:            " << _name << std::endl;

			IdentifierImpl *id = new IdentifierImpl(_name, "input slot", info.str(), Entity::INPUT, m_iCreationCount);
			ServiceSlot *slot = new ServiceSlot(id, _service);
			m_Entities.insert(NamedEntity(id->id(), slot));
			return slot;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	ServiceSlot *const EntityTable::createOutputSlot(std::string const& _name, ServiceContainer *const _service)
	{
		try
		{
			++m_iCreationCount;

			std::stringstream info;
			info << "i am an output slot" << std::endl;
			info << "creation id:       " << m_iCreationCount << std::endl;
			info << "name:              " << _name << std::endl;

			IdentifierImpl *id = new IdentifierImpl(_name, "output slot", info.str(), Entity::OUTPUT, m_iCreationCount);
			ServiceSlot *slot = new ServiceSlot(id, _service);
			m_Entities.insert(NamedEntity(id->id(), slot));
			return slot;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}