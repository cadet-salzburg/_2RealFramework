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

#include <iostream>
#include <sstream>

namespace _2Real
{
	SystemGraph::SystemGraph(Identifier const& id, Engine &engine) :
		Graph(),
		Entity(id),
		m_Engine(engine),
		m_ExceptionHandler(id),
		m_Plugins(*this)
		//m_Threads(10, 20, 1000, 0, id.name())
	{
	}

	SystemGraph::~SystemGraph()
	{
		Engine::instance()->getLogStream() << "system graph: destructor called" << std::endl;

		//for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		//{
		//	(*it)->stop();
		//}

		//Engine::instance()->getLogStream() << "system graph: joining threadpool" << std::endl;

		//m_Threads.joinAll();

		//Engine::instance()->getLogStream() << "system graph: deactivating threadpool" << std::endl;

		//m_Threads.stopAll();

		//Engine::instance()->getLogStream() << "system graph: deleting children: " << childCount() << std::endl;

		//for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		//{
		//	try
		//	{
		//		delete *it;
		//	}
		//	catch (...)
		//	{
		//		Engine::instance()->getLogStream() << "system graph: error on runnable destruction" << std::endl;
		//	}
		//}

		m_Children.clear();
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

}