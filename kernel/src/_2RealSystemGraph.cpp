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

#include "_2RealSystemGraph.h"
#include "_2RealRunnable.h"
#include "_2RealRunnableGraph.h"
#include "_2RealExceptionHandler.h"
#include "_2RealEngine.h"
#include "_2RealException.h"

#include <sstream>

namespace _2Real
{
	SystemGraph::SystemGraph(Identifier const& id, StringMap const& allowedTypes) :
		Graph(),
		Entity(id),
		m_Engine(Engine::instance()),
		m_Plugins(*this),
		m_AllowedTypes(allowedTypes),
		m_ExceptionHandler(id),
		m_Threads(10, 20, 1000, 0, id.name()),
		m_Logfile("")
	{
	}

	void SystemGraph::setLogfile(std::string const& file)
	{
		if (m_Logstream.is_open())
		{
			m_Logstream.close();
		}

		m_Logfile = file;
		startLogging();
	}

	void SystemGraph::startLogging()
	{
		if (!m_Logfile.empty())
		{
			m_Logstream.open(m_Logfile);
		}

		if (m_Logstream.is_open())
		{
			m_Logstream << m_Engine.timer().getTimestamp() << identifier() << std::endl;
		}
	}

	SystemGraph::~SystemGraph()
	{
		if (childCount() > 0)
		{
			shutdown();
		}
	}

	void SystemGraph::shutdown()
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			(*it)->stop();
		}

		m_Threads.joinAll();
		m_Threads.stopAll();

		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			delete *it;
		}

		m_Children.clear();
		//m_Threads.clear();
	}

	void SystemGraph::registerExceptionCallback(ExceptionCallback callback)
	{
		m_ExceptionHandler.registerExceptionCallback(callback);
	}

	void SystemGraph::unregisterExceptionCallback(ExceptionCallback callback)
	{
		m_ExceptionHandler.unregisterExceptionCallback(callback);
	}

	void SystemGraph::registerExceptionListener(ExceptionListener &listener)
	{
		m_ExceptionHandler.registerExceptionListener(listener);
	}

	void SystemGraph::unregisterExceptionListener(ExceptionListener &listener)
	{
		m_ExceptionHandler.unregisterExceptionListener(listener);
	}

	void SystemGraph::insertChild(Runnable &child, unsigned int const& index)
	{
		m_Children.push_back(&child);
		//m_Threads.addCapacity(1);
	}

	void SystemGraph::removeChild(Identifier const& id)
	{
		RunnableList::iterator it = iteratorId(id);
		m_Children.erase(it);
	
		//m_Threads.addCapacity(1);
	}

	void SystemGraph::startAll()
	{
		//todo
	}

	void SystemGraph::stopAll()
	{
		//todo
	}

	void SystemGraph::handleException(Runnable &child, Exception &exception)
	{
		//m_ExceptionHandler.handleException(exception, child.identifier());

		//Graph *subgraph = child->father();
		//stopChild(subgraph->id());
	}

	void SystemGraph::startChild(Identifier const& id)
	{
	//	RunnableList::iterator it = iteratorId(id);

	//	if (it == m_Children.end())
	//	{
	//		std::ostringstream msg;
	//		msg << "internal error: child " << id.name() << " not found in graph";
	//		throw _2Real::Exception(msg.str());
	//	}

	//	Runnable *child = *it;
	//	child->checkConfiguration();
	//	child->start(false);

	//	m_Threads.start(*it);
	}

	void SystemGraph::stopChild(Identifier const& id)
	{
	//	RunnableList::iterator it = iteratorId(id);
	//	
	//	if (it == m_Children.end())
	//	{
	//		std::ostringstream msg;
	//		msg << "internal error: child " << id.name() << " not found in graph";
	//		throw _2Real::Exception(msg.str());
	//	}

	//	(*it)->stop();
	//	m_Threads.join(id);
	}

	const Identifier SystemGraph::installPlugin(std::string const& name, std::string const& classname)
	{
		return m_Plugins.install(name, classname);
	}

	const bool SystemGraph::contains(Identifier const& id) const
	{
		return ((id.isPlugin() && m_Plugins.contains(id)) || Graph::contains(id));
	}

}