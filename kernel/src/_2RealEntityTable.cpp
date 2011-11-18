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

#include "_2RealEntity.h"
//#include "_2RealIdentifier.h"
//#include "_2RealId.h"
//#include "_2RealException.h"
//#include "_2RealEngine.h"
#include "_2RealTypes.h"

#include "_2RealExceptionHandler.h"
#include "_2RealSystemGraph.h"
#include "_2RealService.h"
#include "_2RealSequence.h"
#include "_2RealSynchronization.h"
#include "_2RealSetupParameter.h"
#include "_2RealInputSlot.h"
#include "_2RealOutputSlot.h"
#include "_2RealPlugin.h"
#include "_2RealServiceTemplate.h"

#include <sstream>
#include <iostream>

namespace _2Real
{

	EntityTable::EntityTable(Engine &_engine) :
		m_Entities(),
		m_Engine(_engine),
		m_CreationCount(0)
	{
	}

	EntityTable::~EntityTable()
	{
		for (EntityMap::iterator it = m_Entities.begin(); it != m_Entities.end(); it++)
		{
			//~~~
			std::cout << "shutdown: entity left in entity table: " << it->second->info() << std::endl;
		}
	}

	void EntityTable::destroy(SystemGraph *_obj)
	{
		EntityMap::iterator e = m_Entities.find(_obj->id());

		std::list< Runnable * > children;
		std::list< Runnable * >::iterator it;
		children = _obj->children();

		for (it = children.begin(); it != children.end(); it++)
		{
			if ((*it)->hasParameters())
			{
				Service *service = static_cast< Service * >(*it);
				destroy(service);
			}
			else
			{
				RunnableGraph *graph = static_cast< RunnableGraph * >(*it);
				destroy(graph);
			}
		}

		delete _obj;
		m_Entities.erase(e);
	}

	void EntityTable::destroy(RunnableGraph *_obj)
	{
		EntityMap::iterator e = m_Entities.find(_obj->id());

		std::list< Runnable * > children;
		std::list< Runnable * >::iterator it;
		children = _obj->children();

		for (it = children.begin(); it != children.end(); it++)
		{
			if ((*it)->hasParameters())
			{
				Service *service = static_cast< Service * >(*it);
				destroy(service);
			}
			else
			{
				RunnableGraph *graph = static_cast< RunnableGraph * >(*it);
				destroy(graph);
			}
		}

		delete _obj;
		m_Entities.erase(e);
	}

	void EntityTable::destroy(Service *_obj)
	{
		EntityMap::iterator e = m_Entities.find(_obj->id());

		//_obj->resetIO();

		ParameterMap setup = _obj->setupParameters();
		for (ParameterMap::iterator it = setup.begin(); it != setup.end(); it++)
		{
			destroy(it->second);
		}

		InputMap input = _obj->inputSlots();
		for (InputMap::iterator it = input.begin(); it != input.end(); it++)
		{
			destroy(it->second);
		}

		OutputMap output = _obj->outputSlots();
		for (OutputMap::iterator it = output.begin(); it != output.end(); it++)
		{
			destroy(it->second);
		}

		delete _obj;
		m_Entities.erase(e);
	}

	void EntityTable::destroy(Parameter *_obj)
	{
		EntityMap::iterator e = m_Entities.find(_obj->id());

		delete _obj;
		m_Entities.erase(e);
	}

	void EntityTable::destroy(Plugin *_obj)
	{
		EntityMap::iterator e = m_Entities.find(_obj->id());

		TemplateMap &services = _obj->services();
		for (TemplateMap::iterator it = services.begin(); it != services.end(); it++)
		{
			destroy(it->second);
		}

		//~~~
		//IDs ids = _obj->setupParameterIDs();
		//for (std::list< SetupParameter * >::iterator it = parameters.begin(); it != parameters.end(); it++)
		//{
		//	destroy(*it);
		//}

		delete _obj;
		m_Entities.erase(e);
	}

	void EntityTable::destroy(ServiceTemplate *_obj)
	{
		EntityMap::iterator e = m_Entities.find(_obj->id());
		//delete _obj;
		m_Entities.erase(e);
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
			++m_CreationCount;

			std::stringstream info;
			info << "i am a plugin" << std::endl;
			info << "creation id:\t" << m_CreationCount << std::endl;
			info << "chosen name:\t" << _name << std::endl;
			info << "directory:\t" << _dir << std::endl;
			info << "filename:\t" << _file << std::endl;
			info << "classname:\t" << _class << std::endl;

			Id *id = new Id(_name, "plugin", info.str(), m_CreationCount);

			PluginMetadata *meta = new PluginMetadata(_class, _file, _dir, m_Engine.types());
			Plugin *plugin = new Plugin(meta, id);
			m_Entities.insert(NamedEntity(id->id(), plugin));
			return plugin;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Service *const EntityTable::createService(std::string const& _name, SystemGraph *const _system, IService *const _service)
	{
		try
		{
			++m_CreationCount;

			std::stringstream info;
			info << "i am a service" << std::endl;
			info << "creation id:\t" << m_CreationCount << std::endl;
			info << "chosen name:\t" << _name << std::endl;

			Id *id = new Id(_name, "service", info.str(), m_CreationCount);
			Identifier i(id);
			Service *service = new Service(_service, id, _system);
			m_Entities.insert(NamedEntity(id->id(), service));
			return service;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	SystemGraph *const EntityTable::createSystem(std::string const& _name)
	{
		try
		{
			++m_CreationCount;

			std::stringstream info;
			info << "i am a system" << std::endl;
			info << "creation id:\t" << m_CreationCount << std::endl;
			info << "chosen name:\t" << _name << std::endl;

			Id *id = new Id(_name, "system", info.str(), m_CreationCount);

			Identifier i(id);
			ExceptionHandler *output = new ExceptionHandler(i);

			SystemGraph *graph = new SystemGraph(id, output);
			m_Entities.insert(NamedEntity(id->id(), graph));
			return graph;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Sequence *const EntityTable::createSequence(std::string const& _name, SystemGraph *const _system)
	{
		try
		{
			//++m_CreationCount;

			//std::stringstream info;
			//info << "i am a sequence" << std::endl;
			//info << "creation id:\t" << m_CreationCount << std::endl;
			//info << "chosen name:\t" << _name << std::endl;

			//Id *id = new Id(_name, "sequence", info.str(), m_CreationCount);

			//Identifier i(id);
			//ExceptionHandler *output = new ExceptionHandler(i);

			//Sequence *seq = new Sequence(id, output);
			//m_Entities.insert(NamedEntity(id->id(), seq));
			Sequence *seq;
			return seq;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Synchronization *const EntityTable::createSynchronization(std::string const& _name, SystemGraph *const _system)
	{
		try
		{
			//++m_CreationCount;

			//std::stringstream info;
			//info << "i am a synchronization" << std::endl;
			//info << "creation id:\t\t" << m_CreationCount << std::endl;
			//info << "chosen name:\t\t" << _name << std::endl;

			//Id *id = new Id(_name, "synchronization", info.str(), m_CreationCount);

			//Identifier i(id);
			//ExceptionHandler *output = new ExceptionHandler(i);

			//Synchronization *sync = new Synchronization(id, output);
			//m_Entities.insert(NamedEntity(id->id(), sync));
			Synchronization *sync;
			return sync;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	ServiceTemplate *const EntityTable::createFactoryRef(std::string const& _name, unsigned int const& _pluginID, ServiceCreator _creator, ServiceMetadata const& _metadata)
	{
		try
		{
			++m_CreationCount;

			std::stringstream info;
			info << "i am a factory reference" << std::endl;
			info << "creation id:\t\t" << m_CreationCount << std::endl;
			info << "service name:\t\t" << _name << std::endl;

			Id *id = new Id(_name, "factory", info.str(), m_CreationCount);
			ServiceTemplate *ref = new ServiceTemplate(_name, _pluginID, _creator, _metadata, id);
			m_Entities.insert(NamedEntity(id->id(), ref));
			return ref;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	SetupParameter *const EntityTable::createSetupParameter(std::string const& _name, std::string const& _key, Service *const _service)
	{
		try
		{
			++m_CreationCount;

			std::stringstream info;
			info << "i am a setup parameter" << std::endl;
			info << "creation id:\t" << m_CreationCount << std::endl;
			info << "param name: \t" << _name << std::endl;

			Id *id = new Id(_name, "setup parameter", info.str(), m_CreationCount);
			std::string type = m_Engine.types().getTypename(_key);
			SetupParameter *param = new SetupParameter(id, type, _key);
			m_Entities.insert(NamedEntity(id->id(), param));
			return param;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	SetupParameter *const EntityTable::createSetupParameter(std::string const& _name, std::string const& _key, Plugin *const _plugin)
	{
		try
		{
			++m_CreationCount;

			std::stringstream info;
			info << "i am a setup parameter" << std::endl;
			info << "creation id:\t" << m_CreationCount << std::endl;
			info << "param name: \t" << _name << std::endl;

			Id *id = new Id(_name, "setup parameter", info.str(), m_CreationCount);
			std::string type = m_Engine.types().getTypename(_key);
			SetupParameter *param = new SetupParameter(id, type, _key);
			m_Entities.insert(NamedEntity(id->id(), param));
			return param;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	InputSlot *const EntityTable::createInputSlot(std::string const& _name, std::string const& _key, Service *const _service)
	{
		try
		{
			++m_CreationCount;

			std::stringstream info;
			info << "i am an input slot" << std::endl;
			info << "creation id:\t\t" << m_CreationCount << std::endl;
			info << "slot name:\t\t" << _name << std::endl;

			Id *id = new Id(_name, "input slot", info.str(), m_CreationCount);
			std::string type = m_Engine.types().getTypename(_key);
			InputSlot *slot = new InputSlot(id, _service, type, _key);
			m_Entities.insert(NamedEntity(id->id(), slot));
			return slot;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	OutputSlot *const EntityTable::createOutputSlot(std::string const& _name, std::string const& _key, Service *const _service)
	{
		try
		{
			++m_CreationCount;

			std::stringstream info;
			info << "i am an output slot" << std::endl;
			info << "creation id:\t\t" << m_CreationCount << std::endl;
			info << "slot name:\t\t" << _name << std::endl;

			Id *id = new Id(_name, "output slot", info.str(), m_CreationCount);
			std::string type = m_Engine.types().getTypename(_key);
			SharedAny init;
			// ~~ createfromkeyword
			m_Engine.types().create(_key, init);
			OutputSlot *slot = new OutputSlot(id, _service, type, _key, init);
			m_Entities.insert(NamedEntity(id->id(), slot));
			return slot;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}