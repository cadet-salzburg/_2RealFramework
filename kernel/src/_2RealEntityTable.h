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

#pragma once

//#include "_2RealEngineTypedefs.h"
#include "_2RealId.h"
#include "_2RealEngine.h"
#include "_2RealException.h"

#include <map>

namespace _2Real
{

	class IService;
	typedef IService *const (*const ServiceCreator)(void);

	class SystemGraph;
	class RunnableGraph;
	class Sequence;
	class Synchronization;
	class Service;
	class Parameter;
	class InputSlot;
	class OutputSlot;
	class SetupParameter;
	class Plugin;
	class ServiceTemplate;
	class ServiceMetadata;
	class PluginMetadata;

	class EntityTable
	{

	public:

		EntityTable(Engine &engine);
		virtual ~EntityTable();

		Entity *const get(unsigned int const& id);
		Entity const *const get(unsigned int const& id) const;
		const Identifier getIdentifier(unsigned int const& id) const;

		void EntityTable::destroy(SystemGraph *obj);
		void EntityTable::destroy(RunnableGraph *obj);
		void EntityTable::destroy(Service *obj);
		void EntityTable::destroy(Parameter *obj);
		void EntityTable::destroy(Plugin *obj);
		void EntityTable::destroy(ServiceTemplate *obj);

		Plugin *const createPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class);
		Service *const createService(std::string const& _name, SystemGraph *const system, IService *const _service);
		ServiceTemplate *const createFactoryRef(std::string const& _name, unsigned int const& _pluginID, ServiceCreator _creator, ServiceMetadata const& _metadata);

		SystemGraph *const createSystem(std::string const& _name);
		Sequence *const createSequence(std::string const& _name, SystemGraph *const system);
		Synchronization *const createSynchronization(std::string const& _name, SystemGraph *const system);

		InputSlot *const createInputSlot(std::string const& name, std::string const& key, Service *const service);
		OutputSlot *const createOutputSlot(std::string const& name, std::string const& key, Service *const service);
		SetupParameter *const createSetupParameter(std::string const& name, std::string const& key, Service *const service);
		SetupParameter *const createSetupParameter(std::string const& name, std::string const& key, Plugin *const plugin);

	private:

		typedef std::pair< unsigned int, Entity * >		NamedEntity;
		typedef std::map< unsigned int, Entity * >		EntityMap;

		/**
		*	entities
		*/
		EntityMap										m_Entities;

		/**
		*	the 2 real engine
		*/
		Engine											&m_Engine;

		/**
		*	uinque id for entities
		*/
		unsigned int									m_CreationCount;

	};

}