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

#include "_2RealEngine.h"
#include "_2RealException.h"

#include <map>
#include <string>

namespace _2Real
{

	class Identifier;
	class SystemGraph;
	class RunnableGraph;
	class Runnable;

	typedef std::pair< unsigned int, SystemGraph * >	NamedSystem;
	typedef	std::map< unsigned int, SystemGraph * >		SystemTable;

	class ProductionGraphs
	{

	public:	

		ProductionGraphs(Engine &engine);
		virtual ~ProductionGraphs();

		/**
		*	create sequence; top must be in system table, a & b must be two existing runnables in top -> result will be placed in top
		*/
		const Identifier createSequence(std::string const& name, unsigned int const& a, unsigned int const& b, unsigned int const& top);

		/**
		*	create syncronization; top must be in system table, a & b must be two existing runnables in top -> result will be placed in top
		*/
		const Identifier createSynchronization(std::string const& name, unsigned int const& a, unsigned int const& b, unsigned int const& top);

		/**
		*	creates a new system
		*/
		const Identifier createSystemGraph(std::string const& name);


		SystemGraph *const getSystemGraph(Identifier const& id);
		SystemGraph const *const getSystemGraph(Identifier const& id) const;

		/**
		*	
		*/
		const bool isSystemGraph(Identifier const& id);

		/**
		*	destroys a system (and everything inside it)
		*/
		void destroySystemGraph(Identifier const& id);

		/**
		*	destroys a runnable & everything it may contain
		*/
		//does this really belong here?
		void destroyRunnable(unsigned int const& id, unsigned int const& top);

		Runnable *const belongsToSystem(Identifier const& system, Identifier const& runnable) const;

	private:

		/**
		*	systems in existence
		*/
		SystemTable					m_Systems;

		/**
		*	the 2 real engine
		*/
		Engine						&m_Engine;

	};

	class SystemGraphsException : public Exception
	{
	};

}