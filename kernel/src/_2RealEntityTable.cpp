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

#include "_2RealEntityTable.h"
#include "_2RealEngineTypedefs.h"
#include "_2RealEntity.h"
#include "_2RealIdentifier.h"
#include "_2RealId.h"
#include "_2RealException.h"

#include "_2RealPlugin.h"
#include "_2RealServiceContainer.h"
#include "_2RealFactoryReference.h"
#include "_2RealContainer.h"
#include "_2RealServiceSlot.h"
#include "_2RealServiceValue.h"
#include "_2RealApplicationCallback.h"

#include <sstream>
#include <iostream>

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
			try
			{
				std::cout << "internal error: entity left in entity table" << std::endl;
				//delete it->second;
			}
			catch (Exception &e)
			{
				std::cout << "error on entity table destruction: " << e.what() << std::endl;
			}
			catch (...)
			{
				std::cout << "error on entity table destruction" << std::endl;
			}
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
			else if (!e->second)
			{
				throw Exception("internal error: null pointer stored in entity map");
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
			if (_obj->type() == Entity::SERVICE)
			{
				ServiceContainer *service = static_cast< ServiceContainer * >(_obj);
				destroy(service);
			}
			else
			{
				Container *container = static_cast< Container * >(_obj);
				destroy(container);
			}
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
			EntityMap::iterator e = m_Entities.find(_obj->id());
			if (e == m_Entities.end())
			{
				throw Exception("internal error: entity not in entity map");
			}
			else if (!e->second)
			{
				throw Exception("internal error: null pointer stored in entity map");
			}

			_obj->resetIO();

			std::list< ServiceValue * > setup = _obj->setupParams();
			for (std::list< ServiceValue * >::iterator it = setup.begin(); it != setup.end(); it++)
			{
				destroy(*it);
			}

			std::list< ServiceSlot * > input = _obj->inputSlots();
			for (std::list< ServiceSlot * >::iterator it = input.begin(); it != input.end(); it++)
			{
				destroy(*it);
			}

			std::list< ServiceSlot * > output = _obj->outputSlots();
			for (std::list< ServiceSlot * >::iterator it = output.begin(); it != output.end(); it++)
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

	void EntityTable::destroy(ServiceSlot *_obj)
	{
		try
		{
			EntityMap::iterator e = m_Entities.find(_obj->id());
			if (e == m_Entities.end())
			{
				throw Exception("internal error: entity not in entity map");
			}
			else if (!e->second)
			{
				throw Exception("internal error: null pointer stored in entity map");
			}

			delete _obj;
			m_Entities.erase(e);
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
			EntityMap::iterator e = m_Entities.find(_obj->id());
			if (e == m_Entities.end())
			{
				throw Exception("internal error: entity not in entity map");
			}
			else if (!e->second)
			{
				throw Exception("internal error: null pointer stored in entity map");
			}

			delete _obj;
			m_Entities.erase(e);
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
			std::list< FactoryReference * > services = _obj->services();
			for (std::list<FactoryReference * >::iterator it = services.begin(); it != services.end(); it++)
			{
				destroy(*it);
			}

			EntityMap::iterator e = m_Entities.find(_obj->id());
			if (e == m_Entities.end())
			{
				throw Exception("internal error: entity not in entity map");
			}
			else if (!e->second)
			{
				throw Exception("internal error: null pointer stored in entity map");
			}

			delete _obj;
			m_Entities.erase(e);
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
			EntityMap::iterator e = m_Entities.find(_obj->id());
			if (e == m_Entities.end())
			{
				throw Exception("internal error: entity not in entity map");
			}
			else if (!e->second)
			{
				throw Exception("internal error: null pointer stored in entity map");
			}

			//delete _obj;
			m_Entities.erase(e);
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

			std::stringstream info;
			info << "i am a plugin" << std::endl;
			info << "creation id:\t" << m_iCreationCount << std::endl;
			info << "chosen name:\t" << _name << std::endl;
			info << "directory:\t" << _dir << std::endl;
			info << "filename:\t" << _file << std::endl;
			info << "classname:\t" << _class << std::endl;

			Id *id = new Id(_name, "plugin", info.str(), Entity::PLUGIN, m_iCreationCount);
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
			info << "creation id:\t" << m_iCreationCount << std::endl;
			info << "chosen name:\t" << _name << std::endl;

			Id *id = new Id(_name, "service", info.str(), Entity::SERVICE, m_iCreationCount);
			Identifier i(id);
			ApplicationCallback *output = new ApplicationCallback(i);
			ServiceContainer *service = new ServiceContainer(_service, id, output);
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
			info << "creation id:\t" << m_iCreationCount << std::endl;
			info << "chosen name:\t" << _name << std::endl;

			Id *id;
			Container *container;
			id = new Id(_name, "system", info.str(), Entity::SYSTEM, m_iCreationCount);
			Identifier i(id);
			ApplicationCallback *output = new ApplicationCallback(i);
			container = new Container(id, output);
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
			info << "creation id:\t" << m_iCreationCount << std::endl;
			info << "chosen name:\t" << _name << std::endl;

			Id *id;
			Container *container;
			id = new Id(_name, "sequence", info.str(), Entity::SEQUENCE, m_iCreationCount);
			Identifier i(id);
			ApplicationCallback *output = new ApplicationCallback(i);
			container = new Container(id, output);
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
			info << "creation id:\t\t" << m_iCreationCount << std::endl;
			info << "chosen name:\t\t" << _name << std::endl;

			Id *id;
			Container *container;
			id = new Id(_name, "synchronization", info.str(), Entity::SYNCHRONIZATION, m_iCreationCount);
			Identifier i(id);
			ApplicationCallback *output = new ApplicationCallback(i);
			container = new Container(id, output);
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
			info << "creation id:\t\t" << m_iCreationCount << std::endl;
			info << "service name:\t\t" << _name << std::endl;

			Id *id = new Id(_name, "factory", info.str(), Entity::FACTORY, m_iCreationCount);
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
			info << "creation id:\t" << m_iCreationCount << std::endl;
			info << "param name: \t" << _name << std::endl;

			Id *id = new Id(_name, "setup parameter", info.str(), Entity::SETUP, m_iCreationCount);
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
			info << "i am an input slot" << std::endl;
			info << "creation id:\t\t" << m_iCreationCount << std::endl;
			info << "slot name:\t\t" << _name << std::endl;

			Id *id = new Id(_name, "input slot", info.str(), Entity::INPUT, m_iCreationCount);
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
			info << "creation id:\t\t" << m_iCreationCount << std::endl;
			info << "slot name:\t\t" << _name << std::endl;

			Id *id = new Id(_name, "output slot", info.str(), Entity::OUTPUT, m_iCreationCount);
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