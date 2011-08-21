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
#include "_2RealServiceTypedefs.h"

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
	class IService;
	class Plugin;
	class ServiceImpl;
	class Container;

	class Entities
	{

	public:

		/**
		*
		*/
		Entities();

		/**
		*
		*/
		Entities(Entities const& _src) throw(...);

		/**
		*
		*/
		Entities& operator=(Entities const& _src) throw(...);

		/**
		*
		*/
		~Entities();

		/**
		*
		*/
		typedef std::pair< const Identifier, IEntity *const > ID;

		/**
		*	
		*/
		IEntity *const get(unsigned int const& _id) throw(...);

		/**
		*	
		*/
		void destroy(unsigned int const& _id) throw(...);

		/**
		*	
		*/
		const ID createPlugin(std::string const& _name, std::string const& _path, std::string const& _class) throw(...);

		/**
		*	
		*/
		const ID createService(std::string const& _name, Container *const _father, IService *const _service) throw(...);

		/**
		*	
		*/
		const ID createContainer(std::string const& _name, Container *const _father, IdentifierImpl::eType const& _type) throw(...);

		/**
		*	
		*/
		const ID createFactoryRef(std::string const& _name, Plugin const *const _plugin, ServiceCreator _creator, Metadata const *const _metadata) throw(...);

		/**
		*	
		*/
		const ID createServiceParam(std::string const& _name, ServiceImpl const *const _service, IdentifierImpl::eType const& _type) throw(...);

	private:

		friend class EngineImpl;

		typedef std::pair< unsigned int, IEntity * >	NamedEntity;

		typedef std::map< unsigned int, IEntity * >		EntityMap;

		EntityMap										m_Entities;

		unsigned int									m_iCreationCount;

		ServiceFactory									*m_Factory;

		PluginPool										*m_Plugins;

	};

}