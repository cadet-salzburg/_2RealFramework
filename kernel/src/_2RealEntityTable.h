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

#include "_2RealEngineTypedefs.h"
#include "_2RealId.h"

#include <map>

namespace _2Real
{

	/**
	*	
	*/

	class ServiceMetadata;
	class Identifier;
	class Entity;
	class Plugin;
	class AbstractContainer;
	class ServiceContainer;
	class Container;
	class FactoryReference;
	class InputSlot;
	class OutputSlot;
	class ServiceParameter;
	class PluginParameter;
	class Parameter;

	class EntityTable
	{

	public:

		/**
		*	service factory function
		*/
		typedef IService *const (*const ServiceCreator)(void);

		/**
		*	standard ctor; pointer are set directly by engine
		*/
		EntityTable();

		/**
		*	copy kills music
		*/
		EntityTable(EntityTable const& _src);

		/**
		*	as does assignment
		*/
		EntityTable& operator=(EntityTable const& _src);

		/**
		*	kill all entities - entity table is deleted last
		*/
		~EntityTable();

		/**
		*	return entity, throw exception if not existing
		*/
		Entity *const get(unsigned int const& _id);

		/**
		*	return entity, throw exception if not existing
		*/
		Entity const *const get(unsigned int const& _id) const;

		/**
		*	returns an identifier for an id
		*/
		const Identifier getIdentifier(unsigned int const& _id) const;

		/**
		*	destroys a container
		*/
		void EntityTable::destroy(AbstractContainer *_obj);

		/**
		*	destroys a container
		*/
		void EntityTable::destroy(Container *_obj);

		/**
		*	destroys service container
		*/
		void EntityTable::destroy(ServiceContainer *_obj);

		/**
		*	destroys IO slot
		*/
		void EntityTable::destroy(Parameter *_obj);

		/**
		*	destroys plugin
		*/
		void EntityTable::destroy(Plugin *_obj);

		/**
		*	destroys factory ref
		*/
		void EntityTable::destroy(FactoryReference *_obj);

		/**
		*	requested by plugin pool on plugin installation
		*/
		Plugin *const createPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class);

		/**
		*	requested by service factory on service creation. actually creates the service container, not the service
		*/
		ServiceContainer *const createService(std::string const& _name, IService *const _service);

		/**
		*	requested by production graph map on system creation
		*/
		Container *const createSequence(std::string const& _name);

		/**
		*	requested by production graph map on seq creation
		*/
		Container *const createSystem(std::string const& _name);

		/**
		*	requested by production graph map on sync creation
		*/
		Container *const createSynchronization(std::string const& _name);

		/**
		*	requested by service factory on service registration
		*/
		FactoryReference *const createFactoryRef(std::string const& _name, unsigned int const& _pluginID, ServiceCreator _creator, ServiceMetadata const& _metadata);

		/**
		*	requested by service factory on service creation
		*/
		InputSlot *const createInputSlot(std::string const& _name, std::string const& _type, ServiceContainer *const _service);

		/**
		*	requested by service factory on service creation
		*/
		OutputSlot *const createOutputSlot(std::string const& _name, std::string const& _type, ServiceContainer *const _service);

		/**
		*	requested by service factory on service creation
		*/
		ServiceParameter *const createSetupParameter(std::string const& _name, std::string const& _type, ServiceContainer *const _service);

		/**
		*	requested by plugin pool on plugin installation
		*/
		PluginParameter *const createSetupParameter(std::string const& _name, std::string const& _type, Plugin *const _plugin);

	private:

		/**
		*	i can haz typedef
		*/
		typedef std::pair< unsigned int, Entity * >		NamedEntity;

		/**
		*	i can haz typedef
		*/
		typedef std::map< unsigned int, Entity * >		EntityMap;

		/**
		*	entities
		*/
		EntityMap										m_EntityTable;

		/**
		*	creation count is used as unique id
		*/
		unsigned int									m_iCreationCount;

	};

}