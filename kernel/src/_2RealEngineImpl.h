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
#include "_2RealIDataQueue.h"

#include <vector>

#include "Poco/Any.h"
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
		void destroyProductionGraph(Identifier const& _id, Identifier const& _top) throw(...);

		/**
		*	install plugin, return id + service ids
		*/
		const Identifier installPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class, Identifiers &_ids, Identifier const& _top) throw(...);

		/**
		*	
		*/
		void dumpPluginInfo(Identifier const& _id) const throw(...);

		/**
		*	
		*/
		void dumpServiceInfo(Identifier const& _id) const throw(...);

		/**
		*	
		*/
		void dumpServiceInfo(Identifier const& _id, std::string const& _name) const throw(...);

		/**
		*	
		*/
		const Identifier createService(std::string const& _name, Identifier const& _id, Identifiers &_setupIDs, Identifier const& _top) throw(...);

		/**
		*	
		*/
		const Identifier createService(std::string const& _name, Identifier const& _id, std::string const& _service, Identifiers &_setupIDs, Identifier const& _top) throw(...);

		/**
		*
		*/
		Identifiers getInputSlots(Identifier const& _id) const throw(...);

		/**
		*
		*/
		Identifiers getOutputSlots(Identifier const& _id) const throw(...);

		/**
		*	
		*/
		const Identifier createSequenceContainer(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _top) throw(...);

		/**
		*	
		*/
		const Identifier createSynchronizationContainer(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _top) throw(...);

		/**
		*
		*/
		void setParameterValue(Identifier const& _id, Poco::Any _any) throw(...);

		/**
		*
		*/
		void link(Identifier const& _in, Identifier const& _out) throw(...);

		/**
		*	
		*/
		void registerToException(Identifier const& _id, ExceptionCallback _callback) throw(...);

		/**
		*	
		*/
		void registerToNewData(Identifier const& _id, NewDataCallback _callback) throw(...);

		/**
		*	
		*/
		Identifiers getChildren(Identifier const& _id) throw(...);

		/**
		*	
		*/
		void start(Identifier const& _id) throw(...);

		/**
		*	
		*/
		void stop(Identifier const& _id) throw(...);

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