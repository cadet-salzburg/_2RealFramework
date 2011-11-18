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
#include "_2RealSharedAny.h"
#include "_2RealData.h"

#include <vector>

#include "Poco\AbstractDelegate.h"

namespace _2Real
{

	class PluginPool;
	class ServiceFactory;
	class ProductionGraphs;
	class EntityTable;
	class Identifier;
	class TypeTable;
	class EngineTime;
	
	class Engine
	{

	public:

		static Engine *const instance();

		PluginPool & plugins()
		{
			return *m_Plugins;
		}

		PluginPool const& plugins() const
		{
			return *m_Plugins;
		}

		ServiceFactory & factory()
		{
			return *m_Factory;
		}

		ServiceFactory const& factory() const
		{
			return *m_Factory;
		}

		ProductionGraphs & graphs()
		{
			return *m_Graphs;
		}

		ProductionGraphs const& graphs() const
		{
			return *m_Graphs;
		}

		EntityTable & entities()
		{
			return *m_Entities;
		}

		EntityTable const& entities() const
		{
			return *m_Entities;
		}

		TypeTable & types()
		{
			return *m_Types;
		}

		TypeTable const& types() const
		{
			return *m_Types;
		}

		//EngineTime & time()
		//{
		//	return *m_Time;
		//}

		//EngineTime const& time() const
		//{
		//	return *m_Time;
		//}

		const Identifier createSystem(std::string const& name);
		void destroySystem(Identifier const& id);
		
		const Identifier installPlugin(std::string const& name, std::string const& dir, std::string const& file, std::string const& classname, Identifier const& system);
		void startPlugin(Identifier const& plugin, Identifier const& system);

		void dumpPluginInfo(Identifier const& plugin, Identifier const& system) const;
		void dumpServiceInfo(Identifier const& plugin, std::string const& service, Identifier const& system) const;
		
		const Identifier createService(std::string const& name, Identifier const& plugin, std::string const& service, Identifier const& system);

		void setParameterValue(Identifier const& entity, std::string const& name, SharedAny any, std::string const& type, Identifier const& system);

		//Identifiers getSetupParameters(Identifier const& _id, Identifier const& _system) const;
		//Identifiers getInputSlots(Identifier const& _id, Identifier const& _system) const;
		//Identifiers getOutputSlots(Identifier const& _id, Identifier const& _system) const;
		//const Identifier createSequence(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _system);
		//const Identifier createSynchronization(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _system);
		//void insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src, Identifier const& _system);
		//void append(Identifier const& _dst, Identifier const& _src, Identifier const& _system);
		
		//void link(Identifier const& _in, Identifier const& _out, Identifier const& _system);
		void linkSlots(Identifier const& idIn, std::string const& nameIn, Identifier const& idOut, std::string const& nameOut, Identifier const& system);
		//void registerToException(Identifier const& _id, ExceptionCallback _callback, Identifier const& _system);

		//Identifiers getChildren(Identifier const& _id, Identifier const& _system);
		void start(Identifier const& id, Identifier const& system);
		//void startAll(Identifier const& _system);
		void stop(Identifier const& id, Identifier const& system);
		//void stopAll(Identifier const& _system);
		//void destroy(Identifier const& _id, Identifier const& _system);

		/*
		*	the functionality used for receiving data from output slots
		*/
		void registerToNewData(Identifier const& service, std::string const& out, DataCallback callback, Identifier const& system);
		//void unregisterFromNewData(Identifier const& service, std::string const& out, DataCallback callback, Identifier const& system);
		//DataHandle createDataHandle(Identifier const& service, std::string const& out, Identifier const& system);

	private:

		template< typename T >
		friend class SingletonHolder;

		Engine();
		Engine(const Engine &_src);
		Engine& operator=(const Engine &_src);
		~Engine();

		PluginPool				*m_Plugins;
		ServiceFactory			*m_Factory;
		EntityTable				*m_Entities;
		ProductionGraphs		*m_Graphs;
		TypeTable				*m_Types;
		//EngineTime				*m_Time;

	};

}