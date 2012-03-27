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

#include "_2RealGraph.h"
#include "_2RealExceptionHandler.h"
#include "_2RealThreadPool.h"
#include "_2RealCallbacks.h"
//#include "_2RealPluginPool.h"
//#include "_2RealTimer.h"
//
//#include <map>
//#include <list>
//#include <fstream>

#include "Poco/Path.h"
#include "Poco/Timer.h"

namespace _2Real
{

	class EngineImpl;
	class EngineData;
	class PluginPool;
	class Runnable;
	class UpdateTriggers;

	class SystemImpl : public Graph
	{

	public:

		SystemImpl();
		~SystemImpl();

		/**
		*	kills all runnables
		*/
		void				clear();

		/**
		*	returns time (in microseconds) since the system's creation
		*/
		const long				getElapsedTime() const;

		Poco::Timestamp	const&	getTimestamp() const;

		const Identifier	createService(Identifier const& pluginId, std::string const& serviceName, UpdateTriggers const& triggers);
		const Identifier	createService(std::string const& idName, Identifier const& pluginId, std::string const& serviceName, UpdateTriggers const& triggers);
		//const Identifier	createSequence(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);
		//const Identifier	createSynchronization(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB);

		void				setUp(Identifier const& runnableId);
		const EngineData	getValue(Identifier const& id, std::string const& paramName) const;
		std::string const&	getParameterKey(Identifier const& id, std::string const& paramName) const;
		void				setValue(Identifier const& id, std::string const& paramName, EngineData const& value);
		void				sendValue(Identifier const& id, std::string const& paramName, EngineData const& value);
		void				linkSlots(Identifier const& serviceIn, std::string const& nameIn, Identifier const& serviceOut, std::string const& nameOut);

		//void				add(Identifier const& runnable, Identifier const& parent, unsigned int index);
		//void				append(Identifier const& runnable, Identifier const& parent);

		//void				registerExceptionCallback(ExceptionCallback callback);
		//void				unregisterExceptionCallback(ExceptionCallback callback);
		//void				registerExceptionListener(IExceptionListener &listener);
		//void				unregisterExceptionListener(IExceptionListener &listener);
		void				registerToNewData(Identifier const& serviceId, std::string const& outName, DataCallback callback);
		void				unregisterFromNewData(Identifier const& serviceId, std::string const& outName, DataCallback callback);
		//void				registerToNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener);
		//void				unregisterFromNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener);
		//void				registerToStateChange(Identifier const& runnableId, StateChangeCallback callback);
		//void				unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback);
		//void				registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener);
		//void				unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener);

		void				handleException(Runnable &runnable, Exception &exception);

		//bool				isLoggingEnabled() const;
		//void				setLogfile(Poco::Path const& filepath);
		//std::ofstream&	getLogstream();
		//void				startLogging();

	private:

		/**
		*	a ref to the engine
		*/
		EngineImpl			&m_Engine;

		/**
		*	ref to system's plugin pool
		*/
		PluginPool			&m_PluginPool;

		Poco::Timestamp		m_Timestamp;
		ExceptionHandler	m_ExceptionHandler;
		
		//Poco::Path		m_Logfile;
		//std::ofstream		m_Logstream;

	};

}