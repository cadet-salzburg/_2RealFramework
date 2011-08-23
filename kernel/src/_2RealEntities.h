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

	class Metadata;
	class Identifier;
	class IEntity;
	class PluginPool;
	class ServiceFactory;
	class ProductionGraphs;
	class IService;
	class Plugin;
	class ServiceImpl;
	class Container;

	class Entities
	{

		/**
		*	engine will set pointers to plugin pool, service factory & production graph map
		*/
		friend class EngineImpl;

	public:

		/**
		*	standard ctor; pointer are set directly by engine
		*/
		Entities();

		/**
		*	copy kills music
		*/
		Entities(Entities const& _src) throw(...);

		/**
		*	as does assignment
		*/
		Entities& operator=(Entities const& _src) throw(...);

		/**
		*	this kills every entity in existence.
		*	well, entities created by the 2 real engine anyway; porn videos are not affected.
		*/
		~Entities();

		/**
		*	i can haz typedef
		*/
		typedef std::pair< const Identifier, IEntity *const > ID;

		/**
		*	return entity, throw exception if not existing
		*/
		IEntity *const get(unsigned int const& _id) throw(...);

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
		const ID createContainer(std::string const& _name, IdentifierImpl::eType const& _type) throw(...);

		/**
		*	requested by service factory on service registration
		*/
		const ID createFactoryRef(std::string const& _name, Plugin const *const _plugin, ServiceCreator _creator, Metadata const *const _metadata) throw(...);

		/**
		*	requested by service factory on service creation
		*/
		const ID createServiceParam(std::string const& _name, ServiceImpl const *const _service, IdentifierImpl::eType const& _type) throw(...);

	private:

		/**
		*	i can haz typedef
		*/
		typedef std::pair< unsigned int, IEntity * >	NamedEntity;

		/**
		*	i can haz typedef
		*/
		typedef std::map< unsigned int, IEntity * >		EntityMap;

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