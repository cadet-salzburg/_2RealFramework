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

#include <map>
#include <list>

namespace _2Real
{

	/**
	*	
	*	
	*	
	*/

	class PluginPool;
	class Plugin;
	class FactoryReference;
	class ProductionGraphs;
	class ServiceContainer;
	class EntityTable;
	class ServiceMetadata;
	class IService;

	class ServiceFactory
	{

	public:

		/**
		*	service factory function
		*/
		typedef IService *const (*const ServiceCreator)(void);

		/**
		*	
		*/
		ServiceFactory();

		/**
		*	factory should never be copied
		*/
		ServiceFactory(ServiceFactory const& _src) throw(...);

		/**
		*	factory should never be copied
		*/
		ServiceFactory& operator=(ServiceFactory const& _src) throw(...);

		/**
		*	
		*/
		~ServiceFactory();

		/**
		*	registers factory function of a service, returns identifier of factory function
		*/
		FactoryReference *const registerService(std::string const& _name, unsigned int const& _id, ServiceMetadata const& _metadata, ServiceCreator _creator) throw(...);

		/**
		*	creates service container holding an instance of user service identified by _serviceID
		*/
		ServiceContainer *const createService(std::string const& _name, unsigned int const& _id, std::string const& _service) throw(...);

		/**
		*
		*/
		FactoryReference const *const ref(unsigned int const& _plugin, std::string const& _service) const throw(...);

		/**
		*
		*/
		FactoryReference *const ref(unsigned int const& _plugin, std::string const& _service) throw(...);

		/**
		*
		*/
		ServiceMetadata const& info(unsigned int const& _plugin, std::string const& _service) const throw(...);

		/**
		*	
		*/
		std::list< FactoryReference * > getServices(unsigned int const& _plugin) throw(...);

		/**
		*	
		*/
		IDs getServiceIDs(unsigned int const& _plugin) const throw(...);

	private:

		friend class Engine;

		/**
		*	yay, typedefs
		*/
		typedef std::pair< unsigned int, FactoryReference * >		NamedReference;
		
		/**
		*	yay, typedefs
		*/
		typedef std::map< unsigned int, FactoryReference * >		ReferenceTable;

		/**
		*	
		*/
		ReferenceTable												m_References;

		/**
		*
		*/
		EntityTable													*m_Entities;

	};

}