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

/**
*	annoying & ultimately harmless warning about
*	template expressions that are longer than 255 bytes
*/
#pragma warning(disable:4503)

#include "_2RealTypetable.h"
#include "_2RealServiceFactory.h"
#include "_2RealProductionGraphs.h"
#include "_2RealTimer.h"

namespace _2Real
{

	class Data;
	class RunnableError;

	typedef void (*ExceptionCallback)(RunnableError &exception);
	typedef void (*DataCallback)(Data &data);

	class Identifier;
	class OutputListener;
	class ExceptionListener;
	
	class Engine
	{

	public:

		static Engine & instance();

		Typetable const&		types() const;
		Timer const&			timer() const;
		StringMap const&		getAllowedTypes() const;
		long					getTimestamp() const;

		const Identifier		createSystem(std::string const& name);
		void					destroySystem(Identifier const& id);
		void					setSystemLogfile(std::string const& file, Identifier const& system);
		void					setSystemDirectory(std::string const& directory, Identifier const& system);

		//void destroy(Identifier const& _id, Identifier const& _system);
		
		const Identifier		load(std::string const& name, std::string const& classname, Identifier const& system);
		const std::string		getInfo(Identifier const& plugin, Identifier const& system) const;
		const Identifier		createService(std::string const& name, Identifier const& plugin, std::string const& service, Identifier const& system);
		void					setup(Identifier const& setupAble, Identifier const& system);
		void					setUpdateRate(Identifier const& runnable, float const& updatesPerSecond, Identifier const& system);
		void					setValue(Identifier const& setupAble, std::string const& name, EngineData any, std::string const& type, Identifier const& system);

		void linkSlots(Identifier const& idIn, std::string const& nameIn, Identifier const& idOut, std::string const& nameOut, Identifier const& system);

		void start(Identifier const& runnable, Identifier const& system);
		void startAll(Identifier const& system);
		void stop(Identifier const& runnable, Identifier const& system);
		void stopAll(Identifier const& system);
		//void update(Identifier const& runnable, unsigned int const& count, Identifier const& system);

		/**
		*	functions for registering exception & data callbacks
		*/
		void registerToNewData(Identifier const& service, std::string const& out, DataCallback callback, Identifier const& system);
		void unregisterFromNewData(Identifier const& service, std::string const& out, DataCallback callback, Identifier const& system);
		void registerToException(ExceptionCallback callback, Identifier const& system);
		void unregisterFromException(ExceptionCallback callback, Identifier const& system);
		void registerToNewData(Identifier const& service, std::string const& out, OutputListener &listener, Identifier const& system);
		void unregisterFromNewData(Identifier const& service, std::string const& out, OutputListener &listener, Identifier const& system);
		void registerToException(ExceptionListener &listener, Identifier const& system);
		void unregisterFromException(ExceptionListener &listener, Identifier const& system);

		//const Identifier createSequence(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _system);
		//const Identifier createSynchronization(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _system);
		//void insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src, Identifier const& _system);
		//void append(Identifier const& _dst, Identifier const& _src, Identifier const& _system);
		//void link(Identifier const& _in, Identifier const& _out, Identifier const& _system);

	private:

		template< typename T >
		friend class SingletonHolder;

		Engine();
		~Engine();

		ServiceFactory			m_Factory;
		ProductionGraphs		m_Graphs;
		Typetable				m_Types;
		Timer					m_Timer;

	};

	inline Typetable const& Engine::types() const
	{
		return m_Types;
	}

	inline Timer const& Engine::timer() const
	{
		return m_Timer;
	}

	inline StringMap const& Engine::getAllowedTypes() const
	{
		return m_Types.getLookupTable();
	}

	inline long Engine::getTimestamp() const
	{
		return m_Timer.getTimestamp();
	}

}