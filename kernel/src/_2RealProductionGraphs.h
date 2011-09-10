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

	class ProductionGraphs
	{

		/**
		*	engine
		*/
		friend class Engine;
	
	public:	

		/**
		*	standard ctor; pointers will be set directly by engine
		*/
		ProductionGraphs();

		/**
		*	no copies allowed for
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
		*	create sequence; _a & _b must be two existing containers, _top must be member of container map, result will be placed in _top
		*/
		const unsigned int createSequence(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top) throw(...);

		/**
		*	create sync; _a & _b must be two existing containers, _top must be member of container map, result will be placed in _top
		*/
		const unsigned int createSynchronization(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top) throw(...);

		/**
		*	creates nirvana. if only it was that simple in real life ;)
		*/
		const unsigned int createNirvana(std::string const& _name);

		/**
		*	destroys a container, stops it's superior
		*/
		void destroy(unsigned int const& _id, unsigned int const& _top) throw(...);

		/**
		*
		*/
		const bool isNirvana(unsigned int const& _id);

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
		*	nirvana!
		*/
		ContainerMap									m_Containers;

		/**
		*	plugin pool, for communication
		*/
		PluginPool										*m_Plugins;

		/**
		*	service factory, for communication
		*/
		ServiceFactory									*m_Factory;

		/**
		*	entity table, for communication
		*/
		Entities										*m_Entities;

	};

}