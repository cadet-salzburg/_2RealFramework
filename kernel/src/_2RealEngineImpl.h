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
#include "_2RealServiceTypedefs.h"

#include <vector>

#include "Poco/Mutex.h"

namespace _2Real
{

	/**
	*	the 2 real engine
	*/

	class PluginPool;
	class ServiceFactory;
	class ProductionGraphs;
	class Entities;
	class Identifier;
	
	class EngineImpl
	{

	public:

		/**
		*	get instance
		*/
		static EngineImpl *const instance() throw(...);

		/**
		*	
		*/
		EngineImpl();

		/**
		*	engine must not be copied
		*/
		EngineImpl(const EngineImpl &_src) throw(...);

		/**
		*	engine must not be copied
		*/
		EngineImpl& operator=(const EngineImpl &_src) throw(...);

		/**
		*	destruction if engine destroys all entities
		*/
		~EngineImpl();

		/**
		*	ref count++
		*/
		void retain() throw(...);

		/**
		*	ref count--
		*/
		void release() throw(...);

		/**
		*
		*/
		const Identifier createProductionGraph(std::string const& _name) throw(...);

		/*
		*
		*/
		void destroyProductionGraph(Identifier const& _id) throw(...);

		/**
		*	
		*/
		const Identifier installPlugin(std::string const& _name, std::string const& _path, std::string const& _class, Identifiers &_serviceIDs) throw(...);

		/**
		*	
		*/
		const Identifier createService(std::string const& _name, Identifier const& _id, Identifiers &_setupIDs) throw(...);

	private:

		/**
		*	singleton instance
		*/
		static EngineImpl				*s_Instance;

		/**
		*	
		*/
		static Poco::Mutex				s_Mutex;

		/**
		*	ref count for singleton
		*/
		static unsigned int				s_iRefCount;

		/**
		*	takes care of installed plugins
		*/
		PluginPool						*m_Plugins;

		/**
		*	takes care of service creation
		*/
		ServiceFactory					*m_Factory;

		/**
		*	takes care of identifier creation
		*/
		Entities						*m_Entities;

		/**
		*	manages production graphs
		*/
		ProductionGraphs				*m_Graphs;

	};

}