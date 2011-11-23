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
#include "_2RealExceptionHandler.h"

#include <iostream>

namespace _2Real
{
	SystemGraph::SystemGraph(Identifier const& _id, ExceptionHandler *const _handler) :
		Entity(_id),
		Graph(),
		m_ExceptionHandler(_handler)
	{
	}

	SystemGraph::~SystemGraph()
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			(*it)->stop();
		}

		for (std::map< unsigned int, Poco::Thread * >::iterator it = m_Threads.begin(); it != m_Threads.end(); it++)
		{
			it->second->join();
			delete it->second;
		}

		delete m_ExceptionHandler;

		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			std::string name;
			try
			{
				name = (*it)->name();
				std::cout << "deleting runnable: " << name << std::endl;
				delete *it;
			}
			catch (...)
			{
				std::cout << "error on runnable destruction: " << name << std::endl;
			}
		}
	}

	void SystemGraph::insertChild(Runnable *const child, unsigned int const& index)
	{
		//system graph -> order does not matter
		m_Children.push_back(child);
	}

	void SystemGraph::removeChild(unsigned int const& id)
	{
		RunnableList::iterator it = iteratorId(id);
		m_Children.erase(it);
	}

	void SystemGraph::startAll()
	{
	}

	void SystemGraph::stopAll()
	{
	}

	void SystemGraph::handleException(Runnable *const child, Exception &e)
	{
		m_ExceptionHandler->handleException(e);
		Graph *subgraph = child->father();

	}

	void SystemGraph::startChild(unsigned int const& id)
	{
		RunnableList::iterator it = this->iteratorId(id);

		if (it == m_Children.end())
		{
			//throw ChildNotFoundException(name(), id.name());
		}

		Runnable *child = *it;
		std::map< unsigned int, Poco::Thread * >::iterator t = m_Threads.find(child->id());
		if (t != m_Threads.end())
		{
			return;
		}

		Poco::Thread *thread = new Poco::Thread();
		child->checkConfiguration();
		child->start(false);
		thread->start(*child);
		m_Threads.insert(std::pair< unsigned int, Poco::Thread * >(child->id(), thread));
	}

	void SystemGraph::stopChild(unsigned int const& _id)
	{
		RunnableList::iterator it = this->iteratorId(_id);
		if (it == m_Children.end())
		{
			throw Exception("could not start container, " + name() + " does not contain this container");
		}
		if (!(*it))
		{
			throw Exception(std::string("internal error - null pointer stored in system graph: ").append(this->name()));
		}

		Runnable *child = *it;
		std::map< unsigned int, Poco::Thread * >::iterator thread = m_Threads.find(child->id());
		if (thread != m_Threads.end())
		{
			child->stop();
			thread->second->join();
			delete thread->second;
			m_Threads.erase(thread);
		}
	}

}