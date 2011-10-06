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

#include "_2RealTypedefs.h"
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
	class EntityTable;
	class Identifier;
	
	class Engine
	{

	public:

		/**
		*	get instance
		*/
		static Engine *const instance();

		/**
		*	
		*/
		Engine();

		/**
		*	engine must not be copied
		*/
		Engine(const Engine &_src);

		/**
		*	engine must not be copied
		*/
		Engine& operator=(const Engine &_src);

		/**
		*	destruction of engine destroys all entities
		*/
		~Engine();

		/**
		*	ref count++
		*/
		void retain();

		/**
		*	ref count--
		*/
		void release();

		/**
		*	creates a system = nirvana
		*/
		const Identifier createSystem(std::string const& _name);

		/*
		*	destroys a system = nirvana
		*/
		void destroySystem(Identifier const& _id);

		/**
		*	install plugin, return id
		*/
		const Identifier installPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class, Identifier const& _top);

		/**
		*	start plugin
		*/
		void startPlugin(Identifier const& _plugin, Identifier const& _top);

		/**
		*	
		*/
		void dumpPluginInfo(Identifier const& _plugin, Identifier const& _top) const;

		/**
		*	
		*/
		void dumpServiceInfo(Identifier const& _plugin, std::string const& _service, Identifier const& _top) const;

		/**
		*	
		*/
		const Identifier createService(std::string const& _name, Identifier const& _plugin, std::string const& _service, Identifier const& _top);

		/**
		*
		*/
		Identifiers getSetupParameters(Identifier const& _id, Identifier const& _top) const;

		/**
		*
		*/
		Identifiers getInputSlots(Identifier const& _id, Identifier const& _top) const;

		/**
		*
		*/
		Identifiers getOutputSlots(Identifier const& _id, Identifier const& _top) const;

		/**
		*	
		*/
		const Identifier createSequence(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _top);

		/**
		*	
		*/
		const Identifier createSynchronization(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _top);

		/**
		*
		*/
		void insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src, Identifier const& _top);

		/**
		*
		*/
		void append(Identifier const& _dst, Identifier const& _src, Identifier const& _top);

		/**
		*
		*/
		void setParameterValue(Identifier const& _id, Poco::Any _any, std::string const& _type, Identifier const& _top);

		/**
		*
		*/
		void link(Identifier const& _in, Identifier const& _out, Identifier const& _top);

		/**
		*
		*/
		void linkSlots(Identifier const& _in, Identifier const& _out, Identifier const& _top);

		/**
		*	
		*/
		void registerToException(Identifier const& _id, ExceptionCallback _callback, Identifier const& _top);

		/**
		*	
		*/
		void registerToNewData(Identifier const& _id, NewDataCallback _callback, Identifier const& _top);

		/**
		*	
		*/
		Identifiers getChildren(Identifier const& _id, Identifier const& _top);

		/**
		*	
		*/
		void start(Identifier const& _id, Identifier const& _top);

		/**
		*
		*/
		void startAll(Identifier const& _top);

		/**
		*	
		*/
		void stop(Identifier const& _id, Identifier const& _top);

		/**
		*
		*/
		void stopAll(Identifier const& _top);

		/**
		*
		*/
		void destroy(Identifier const& _id, Identifier const& _top);

	private:

		/**
		*	singleton instance
		*/
		static Engine				*s_Instance;

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
		EntityTable						*m_EntityTable;

		/**
		*	manages production graphs
		*/
		ProductionGraphs				*m_Graphs;

	};

}