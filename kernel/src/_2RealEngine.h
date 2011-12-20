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
#include "_2RealSystemPool.h"
#include "_2RealTimer.h"

namespace _2Real
{

	class Data;
	class RunnableError;

	typedef void (*ExceptionCallback)(RunnableError &exception);
	typedef void (*DataCallback)(Data &data);

	class Identifier;
	class IOutputListener;
	class IExceptionListener;
	
	class Engine
	{

	public:

		static Engine & instance();

		Typetable const&		getTypes() const;
		Timer const&			getTimer() const;
		long					getTimestamp() const;

		const Identifier		createSystem(std::string const& idName);
		const Identifier		loadPlugin(std::string const& idName, std::string const& className, Identifier const& system);
		const Identifier		createService(std::string const& idName, Identifier const& plugin, std::string const& serviceName, Identifier const& system);
		const Identifier		createSequence(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB, Identifier const& system);
		const Identifier		createSynchronization(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB, Identifier const& system);
		
		//void					destroy(Identifier const& id, Identifier const& systemId);
		void					destroySystem(Identifier const& system);

		void					setSystemLogfile(std::string const& file, Identifier const& systemId);
		void					setSystemDirectory(std::string const& directory, Identifier const& systemId);

		const std::string		getInfo(Identifier const& plugin, Identifier const& systemId) const;
		
		void					setup(Identifier const& setupAble, Identifier const& systemId);
		void					setUpdateRate(Identifier const& runnable, float updatesPerSecond, Identifier const& systemId);
		void					setValue(Identifier const& id, std::string const& paramName, EngineData const& value, Identifier const& systemId);
		const EngineData		getValue(Identifier const& id, std::string const& name, Identifier const& systemId) const;

		void					linkSlots(Identifier const& idIn, std::string const& nameIn, Identifier const& idOut, std::string const& nameOut, Identifier const& systemId);
		void					start(Identifier const& runnable, Identifier const& systemId);
		void					startAll(Identifier const& systemId);
		void					stop(Identifier const& runnable, Identifier const& systemId);
		void					stopAll(Identifier const& systemId);
		void					registerToNewData(Identifier const& service, std::string const& outName, DataCallback callback, Identifier const& systemId);
		void					unregisterFromNewData(Identifier const& service, std::string const& outName, DataCallback callback, Identifier const& systemId);
		void					registerToException(ExceptionCallback callback, Identifier const& systemId);
		void					unregisterFromException(ExceptionCallback callback, Identifier const& systemId);
		void					registerToNewData(Identifier const& service, std::string const& outName, IOutputListener &listener, Identifier const& systemId);
		void					unregisterFromNewData(Identifier const& service, std::string const& outName, IOutputListener &listener, Identifier const& systemId);
		void					registerToException(IExceptionListener &listener, Identifier const& systemId);
		void					unregisterFromException(IExceptionListener &listener, Identifier const& systemId);

		void					add(Identifier const& runnable, Identifier const& parent, unsigned int index, Identifier const& system);
		void					append(Identifier const& runnable, Identifier const& parent, Identifier const& system);

	private:

		template< typename T >
		friend class SingletonHolder;

		Engine();
		~Engine();

		ServiceFactory			m_Factory;
		SystemPool		m_Graphs;
		Typetable				m_Types;
		Timer					m_Timer;

	};

	inline Typetable const& Engine::getTypes() const
	{
		return m_Types;
	}

	inline Timer const& Engine::getTimer() const
	{
		return m_Timer;
	}

	inline long Engine::getTimestamp() const
	{
		return m_Timer.getTimestamp();
	}

}