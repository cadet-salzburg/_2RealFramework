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
	class PluginPool;
	class ServiceFactory;
	class ProductionGraphs;
	class IService;
	class Plugin;
	class ServiceContainer;
	class Container;
	class FactoryReference;
	class ServiceSlot;
	class ServiceValue;

	class EntityTable
	{

		/**
		*	engine will set pointers to plugin pool, service factory & production graph map
		*/
		friend class Engine;

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
		*	this kills every entity in existence.
		*	well, entities created by the 2 real engine anyway; porn videos are not affected.
		*/
		~EntityTable();

		/**
		*	i can haz typedef
		*/
		typedef std::pair< const unsigned int, Entity *const > ID;

		/**
		*	return entity, throw exception if not existing
		*/
		Entity *const get(unsigned int const& _id) throw(...);

		/**
		*	returns an identifier for an id
		*/
		const Identifier getIdentifier(unsigned int const& _id) const throw(...);

		void EntityTable::destroy(Container *_obj) throw(...);

		void EntityTable::destroy(ServiceContainer *_obj) throw(...);

		void EntityTable::destroy(ServiceSlot *_obj) throw(...);

		void EntityTable::destroy(ServiceValue *_obj) throw(...);

		void EntityTable::destroy(Plugin *_obj) throw(...);

		void EntityTable::destroy(FactoryReference *_obj) throw(...);

		/**
		*	destroy entity, throw if not existing
		*	plugin pool, service factory & production graphs all use this function
		*	to dispose of stuff they don't want anymore. especially toxic waste.
		*/
		void destroy(unsigned int const& _id) throw(...);

		/**
		*	requested by plugin pool on plugin installation
		*/
		const ID createPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class) throw(...);

		/**
		*	requested by service factory on service creation. actually creates the service container, not the service
		*/
		const ID createService(std::string const& _name, IService *const _service) throw(...);

		/**
		*	requested by production graph map on sequence / sync / nirvana creation
		*/
		const ID createContainer(std::string const& _name, Entity::eType const& _type) throw(...);

		/**
		*	requested by production graph map on sequence / sync / nirvana creation
		*/
		const ID createSequence(std::string const& _name) throw(...);

		/**
		*	requested by production graph map on sequence / sync / nirvana creation
		*/
		const ID createSystem(std::string const& _name) throw(...);

		/**
		*	requested by production graph map on sequence / sync / nirvana creation
		*/
		const ID createSynchronization(std::string const& _name) throw(...);

		/**
		*	requested by service factory on service registration
		*/
		const ID createFactoryRef(std::string const& _name, Plugin const *const _plugin, ServiceCreator _creator, ServiceMetadata const& _metadata) throw(...);

		/**
		*	requested by service factory on service creation
		*/
		const ID createInputSlot(std::string const& _name, ServiceContainer *const _service) throw(...);

		/**
		*	requested by service factory on service creation
		*/
		const ID createOutputSlot(std::string const& _name, ServiceContainer *const _service) throw(...);

		/**
		*	requested by service factory on service creation
		*/
		const ID createServiceValue(std::string const& _name, ServiceContainer *const _service) throw(...);

	private:

		/**
		*	i can haz typedef
		*/
		typedef std::pair< unsigned int, Entity * >	NamedEntity;

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

		/**
		*	service factory for communication
		*/
		ServiceFactory									*m_Factory;

		/**
		*	plugin pool, for communication
		*/
		PluginPool										*m_Plugins;

		/**
		*	production graphs, for communication
		*/
		ProductionGraphs								*m_Graphs;
	};

}