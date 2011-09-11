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
#include "_2RealIdentifierImpl.h"

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
	class ServiceSlot;
	class ServiceValue;

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
		EntityTable(EntityTable const& _src) throw(...);

		/**
		*	as does assignment
		*/
		EntityTable& operator=(EntityTable const& _src) throw(...);

		/**
		*	kill all entities - entity table is deleted last
		*/
		~EntityTable();

		/**
		*	return entity, throw exception if not existing
		*/
		Entity *const get(unsigned int const& _id) throw(...);

		/**
		*	return entity, throw exception if not existing
		*/
		Entity const *const get(unsigned int const& _id) const throw(...);

		/**
		*	returns an identifier for an id
		*/
		const Identifier getIdentifier(unsigned int const& _id) const throw(...);

		/**
		*	destroys a container
		*/
		void EntityTable::destroy(AbstractContainer *_obj) throw(...);

		/**
		*	destroys a container
		*/
		void EntityTable::destroy(Container *_obj) throw(...);

		/**
		*	destroys service container
		*/
		void EntityTable::destroy(ServiceContainer *_obj) throw(...);

		/**
		*	destroys IO slot
		*/
		void EntityTable::destroy(ServiceSlot *_obj) throw(...);

		/**
		*	destroys setup param
		*/
		void EntityTable::destroy(ServiceValue *_obj) throw(...);

		/**
		*	destroys plugin
		*/
		void EntityTable::destroy(Plugin *_obj) throw(...);

		/**
		*	destroys factory ref
		*/
		void EntityTable::destroy(FactoryReference *_obj) throw(...);

		/**
		*	requested by plugin pool on plugin installation
		*/
		Plugin *const createPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class) throw(...);

		/**
		*	requested by service factory on service creation. actually creates the service container, not the service
		*/
		ServiceContainer *const createService(std::string const& _name, IService *const _service) throw(...);

		/**
		*	requested by production graph map on system creation
		*/
		Container *const createSequence(std::string const& _name) throw(...);

		/**
		*	requested by production graph map on seq creation
		*/
		Container *const createSystem(std::string const& _name) throw(...);

		/**
		*	requested by production graph map on sync creation
		*/
		Container *const createSynchronization(std::string const& _name) throw(...);

		/**
		*	requested by service factory on service registration
		*/
		FactoryReference *const createFactoryRef(std::string const& _name, unsigned int const& _pluginID, ServiceCreator _creator, ServiceMetadata const& _metadata) throw(...);

		/**
		*	requested by service factory on service creation
		*/
		ServiceSlot *const createInputSlot(std::string const& _name, ServiceContainer *const _service) throw(...);

		/**
		*	requested by service factory on service creation
		*/
		ServiceSlot *const createOutputSlot(std::string const& _name, ServiceContainer *const _service) throw(...);

		/**
		*	requested by service factory on service creation
		*/
		ServiceValue *const createServiceValue(std::string const& _name, ServiceContainer *const _service) throw(...);

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
		EntityMap										m_Entities;

		/**
		*	creation count is used as unique id
		*/
		unsigned int									m_iCreationCount;

	};

}