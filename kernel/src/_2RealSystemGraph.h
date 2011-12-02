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

#include <map>
#include <list>
#include <fstream>

namespace _2Real
{

	class Engine;
	class ExceptionListener;

	class SystemGraph : public Graph, public Entity
	{

	public:

		SystemGraph(Identifier const& id, Engine &engine);
		~SystemGraph();

		void stopAll();
		void startAll();
		void startChild(Identifier const& child);
		void stopChild(Identifier const& child);
		void updateChild(Identifier const& child, unsigned int const& count);

		void insertChild(Runnable &child, unsigned int const& index);
		void removeChild(Identifier const& id);

		void handleException(Runnable &child, Exception &exception);
		void registerExceptionCallback(ExceptionCallback callback);
		void unregisterExceptionCallback(ExceptionCallback callback);
		void registerExceptionListener(ExceptionListener &listener);
		void unregisterExceptionListener(ExceptionListener &listener);

		PluginPool & plugins();
		PluginPool const& plugins() const;

		std::ofstream& getLogstream() const;
		const std::map< std::string, std::string > getAllowedTypes() const;

		const bool contains(Identifier const& id) const;

	private:

		//ThreadPool		m_Threads;
		PluginPool			m_Plugins;
		ExceptionHandler	m_ExceptionHandler;
		Engine				&m_Engine;

	};

	inline PluginPool & SystemGraph::plugins()
	{
		return m_Plugins;
	}

	inline PluginPool const& SystemGraph::plugins() const
	{
		return m_Plugins;
	}

}