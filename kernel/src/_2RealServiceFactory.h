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

#include "_2RealServiceTypedefs.h"
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
	class ServiceImpl;
	class Identifier;
	class Entities;
	class Metadata;

	class ServiceFactory
	{

	public:

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
		const Identifier registerService(std::string const& _name, Plugin *const _plugin, Metadata const *const _metadata, ServiceCreator _creator) throw(...);

		/**
		*	returns true if service in question can be created
		*/
		const bool canCreate(unsigned int const& _id) const;

		/**
		*	returns true if the service in question is a singleton
		*/
		const bool isSingleton(unsigned int const& _id) const;

		/**
		*	returns true if the service in question can be reconfigured
		*/
		const bool canReconfigure(unsigned int const& _id) const;

		/**
		*	creates service container holding an instance of user service identified by _serviceID
		*/
		const Identifier createService(std::string const& _name, Identifier const& _id, Identifiers &_setupIDs) throw(...);

	private:

		friend class EngineImpl;

		/**
		*	yay, typedefs
		*/
		typedef std::list< ServiceImpl * >							ServiceList;

		/**
		*	yay, typedefs
		*/
		typedef std::pair< FactoryReference *, ServiceList >		ServiceReference;

		/**
		*	yay, typedefs
		*/
		typedef std::pair< unsigned int, ServiceReference >			NamedServiceReference;
		
		/**
		*	yay, typedefs
		*/
		typedef std::map< unsigned int, ServiceReference >			ReferenceTable;

		/**
		*	
		*/
		ReferenceTable												m_References;

		/**
		*
		*/
		Entities													*m_Entities;

		/**
		*
		*/
		PluginPool													*m_Plugins;

		/**
		*	production graphs
		*/
		ProductionGraphs									*m_Graphs;

	};

}