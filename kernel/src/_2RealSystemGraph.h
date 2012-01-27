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

#include "_2RealEntity.h"
#include "_2RealGraph.h"
#include "_2RealExceptionHandler.h"
#include "_2RealThreadPool.h"
#include "_2RealPluginPool.h"
#include "_2RealTimer.h"

#include <map>
#include <list>
#include <fstream>

namespace _2Real
{

	class Data;
	typedef void (*DataCallback)(Data &data);
	typedef void (*StateChangeCallback)(std::string &stateName);

	class IStateChangeListener;
	class IExceptionListener;
	class IOutputListener;
	class Engine;
	class EngineData;
	class Typetable;

	class SystemGraph : public Graph, public Entity
	{

	public:

		SystemGraph(Identifier const& id);
		~SystemGraph();

		void				shutdown();

		const Identifier	loadPlugin(std::string const& idName, std::string const& classname);
		const Identifier	createService(std::string const& idName, Identifier const& pluginId, std::string const& serviceName);
		const Identifier	createSequence(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);
		const Identifier	createSynchronization(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);

		void				stopAll();
		void				startAll();
		void				startChild(Identifier const& runnableId);
		void				stopChild(Identifier const& runnableId);

		void				setUp(Identifier const& setupAbleId);
		const EngineData	getValue(Identifier const& id, std::string const& paramName) const;
		void				setValue(Identifier const& id, std::string const& paramName, EngineData const& value);
		void				setUpdateRate(Identifier const& id, float updatesPerSecond);
		void				linkSlots(Identifier const& serviceIn, std::string const& nameIn, Identifier const& serviceOut, std::string const& nameOut);

		void				add(Identifier const& runnable, Identifier const& parent, unsigned int index);
		void				append(Identifier const& runnable, Identifier const& parent);

		void				registerExceptionCallback(ExceptionCallback callback);
		void				unregisterExceptionCallback(ExceptionCallback callback);
		void				registerExceptionListener(IExceptionListener &listener);
		void				unregisterExceptionListener(IExceptionListener &listener);
		void				registerToNewData(Identifier const& serviceId, std::string const& outName, DataCallback callback);
		void				unregisterFromNewData(Identifier const& serviceId, std::string const& outName, DataCallback callback);
		void				registerToNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener);
		void				unregisterFromNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener);
		void				registerToStateChange(Identifier const& runnableId, StateChangeCallback callback);
		void				unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback);
		void				registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener);
		void				unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener);

		PooledThread&		getFreeThread();
		void				handleException(Runnable &runnable, Exception &exception);

		bool				isLoggingEnabled() const;
		void				setInstallDirectory(std::string const& directory);
		void				setLogfile(std::string const& file);
		std::ofstream &		getLogstream();
		const std::string	getInfoString(Identifier const& id) const;

	private:

		void				startLogging();

		Engine				&m_Engine;
		ThreadPool			m_Threads;
		PluginPool			m_Plugins;
		ExceptionHandler	m_ExceptionHandler;
		std::string			m_Logfile;
		std::ofstream		m_Logstream;

	};

	inline PooledThread& SystemGraph::getFreeThread()
	{
		return m_Threads.getFreeThread();
	}

	inline bool SystemGraph::isLoggingEnabled() const
	{
		return m_Logstream.is_open();
	}

	inline std::ofstream & SystemGraph::getLogstream()
	{
		return m_Logstream;
	}

	inline void SystemGraph::setInstallDirectory(std::string const& directory)
	{
		m_Plugins.setInstallDirectory(directory);
	}

	inline const std::string SystemGraph::getInfoString(Identifier const& id) const
	{
		return m_Plugins.getInfoString(id);
	}

}