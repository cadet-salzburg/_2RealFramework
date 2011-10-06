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
	*	stores top level production graphs = systems
	*	needs to be refactored
	*	(but not as badly as some other classes)
	*/

	class EntityTable;
	class Container;

	class ProductionGraphs
	{

	public:	

		/**
		*	standard ctor; pointers will be set directly by engine
		*/
		ProductionGraphs();

		/**
		*	no copies allowed
		*/
		ProductionGraphs(ProductionGraphs const& _src);

		/**
		*	no assignment allowed
		*/
		ProductionGraphs& operator=(ProductionGraphs const& _src);

		/**
		*	causes destruction of all existing systems
		*/
		~ProductionGraphs();

		/**
		*	create sequence; _a & _b must be two existing containers, _top must be member of container map, result will be placed in _top
		*/
		const unsigned int createSequence(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top);

		/**
		*	create sync; _a & _b must be two existing containers, _top must be member of container map, result will be placed in _top
		*/
		const unsigned int createSynchronization(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top);

		/**
		*	creates a new system
		*/
		const unsigned int createSystem(std::string const& _name);

		/**
		*	returns system, throws if nonexistant
		*/
		Container *const getSystem(unsigned int const& _id);

		/**
		*	destroys a system (and everything inside it)
		*/
		void destroySystem(unsigned int const& _id);

		/**
		*	destroys container, stops container's root
		*/
		void destroy(unsigned int const& _id, unsigned int const& _top);

	private:

		friend class Engine;

		/**
		*	yay, typedefs
		*/
		typedef std::pair< unsigned int, Container * >	NamedSystem;

		/**
		*	yay, typedefs
		*/
		typedef	std::map< unsigned int, Container * >	SystemMap;

		/**
		*	systems in existence
		*/
		SystemMap										m_Systems;

		/**
		*	entity table, for communication
		*/
		EntityTable										*m_EntityTable;

	};

}