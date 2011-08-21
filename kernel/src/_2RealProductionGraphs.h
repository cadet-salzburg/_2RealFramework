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

#include <map>
#include <string>

namespace _2Real
{

	/**
	*	stores top level production graphs
	*/

	class PluginPool;
	class ServiceFactory;
	class Entities;
	class Container;
	class Identifier;

	class ProductionGraphs
	{

		friend class EngineImpl;
	
	public:	

		/**
		*	pointers will be set directly by engine
		*/
		ProductionGraphs();

		/**
		*	no copies allowed
		*/
		ProductionGraphs(ProductionGraphs const& _src) throw(...);

		/**
		*	no assignment allowed
		*/
		ProductionGraphs& operator=(ProductionGraphs const& _src) throw(...);

		/**
		*	causes destruction of nirvana - and therefore, of all existing graphs
		*/
		~ProductionGraphs();

		/**
		*	create sequence
		*/
		const Identifier createSequence(std::string const& _name, Identifier const& _a, Identifier const& _b, Identifier const& _top) throw(...);

		/**
		*	create synch
		*/
		const Identifier createSynchronization(std::string const& _name, Identifier const& _a, Identifier const& _b, Identifier const& _top) throw(...);

		/**
		*	create top level pg. if only it was that simple in real life ;)
		*/
		const Identifier createNirvana(std::string const& _name);

		/**
		*	destroys a container
		*/
		void destroy(Identifier const& _id, Identifier const& _top) throw(...);

	private:

		/**
		*	yay, typedefs
		*/
		typedef std::pair< unsigned int, Container * >	NamedContainer;

		/**
		*	yay, typedefs
		*/
		typedef	std::map< unsigned int, Container * >	ContainerMap;

		/**
		*	all top level pgs
		*/
		ContainerMap									m_Containers;

		/**
		*	plugins
		*/
		PluginPool										*m_Plugins;

		/**
		*	services + factory refs
		*/
		ServiceFactory									*m_Factory;

		/**
		*	all entities
		*/
		Entities										*m_Entities;

	};

}