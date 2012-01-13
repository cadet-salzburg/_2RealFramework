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

#include "_2RealGraph.h"
#include "_2RealException.h"
#include "_2RealRunnableGraph.h"
#include "_2RealRunnableManager.h"

#include <sstream>

namespace _2Real
{

	class GraphOperation
	{

	public:

		GraphOperation(Graph &graph);
		virtual ~GraphOperation() = 0;
		virtual bool tryExecuteOperation() = 0;

	protected:

		Graph &m_Graph;

	};

	GraphOperation::GraphOperation(Graph &graph) :
		m_Graph(graph)
	{
	}

	GraphOperation::~GraphOperation()
	{
	}

	class ChildInsertion : public GraphOperation
	{

	public:

		ChildInsertion(Graph &graph, RunnableManager &child, unsigned int index) :
			GraphOperation(graph),
			m_Runnable(child),
			m_Position(index)
		{
		}

		~ChildInsertion()
		{
		}

		bool tryExecuteOperation()
		{
			if (m_Runnable.getManagedRunnable().isRoot())
			{
				//m_Graph.insertChild(m_Runnable, m_Position);
				return true;
			}
			else
			{
				return false;
			}
		}

	private:

		RunnableManager		&m_Runnable;
		unsigned int		m_Position;

	};

	class ChildRemoval : public GraphOperation
	{

	public:

		ChildRemoval(Graph &graph, Identifier const& childId) :
			GraphOperation(graph),
			m_ChildId(childId)
		{
		}

		~ChildRemoval()
		{
		}

		bool tryExecuteOperation()
		{
			//m_Graph.removeChild(m_ChildId);
			return true;
		}

	private:

		Identifier			m_ChildId;

	};

	////////////////////////////////////////////////////////////////

	Graph::Graph() :
		m_Children(),
		m_Operations()
	{
	}

	Graph::~Graph()
	{
		for (std::list< GraphOperation * >::iterator it = m_Operations.begin(); it != m_Operations.end(); it++)
		{
			delete *it;
		}

		m_Operations.clear();
	}

	unsigned int Graph::childCount() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_GraphMutex);

		return m_Children.size();
	}

	bool Graph::executeOperations()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_OperationsMutex);

		for (std::list< GraphOperation * >::iterator it = m_Operations.begin(); it != m_Operations.end(); )
		{
			//if op can't be carried out yet, stop & return false (i try to perserve the ordering of ops, so this is the only way)
			//the op can't be carried out if it's an insertion, and the object to be inserted is still inside a graph other than nirvana
			if (!((*it)->tryExecuteOperation()))
			{
				//causes the runnable graph to continue blocking
				return false;
			}
			else
			{
				//op succeeded, so let's throw it out of the list
				std::list< GraphOperation * >::iterator tmp = it;
				it++;
				m_Operations.erase(tmp);
				delete *tmp;
			}
		}

		return true;
	}

	void Graph::removeChild(Identifier const& childId)
	{
		RunnableList::iterator it = iteratorId(childId);
		RunnableManager *child = *it;
		m_Children.erase(it);

		//note that right now, the child has tecnically no father,
		//its inside of the other container's queue
	}

	void Graph::insertChild(RunnableManager &child, unsigned int index)
	{
		RunnableList::iterator it = iteratorPosition(index);
		m_Children.insert(it, &child);
		child.getManagedRunnable().setFather(*this);
	}

	RunnableList::iterator Graph::iteratorId(Identifier const& childId)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_GraphMutex);

		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->getManagedId() == childId)
			{
				return it;
			}
		}
		return m_Children.end();
	}

	RunnableList::const_iterator Graph::iteratorId(Identifier const& childId) const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_GraphMutex);

		for (RunnableList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->getManagedId() == childId)
			{
				return it;
			}
		}
		return m_Children.end();
	}

	RunnableList::iterator Graph::iteratorPosition(unsigned int position)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_GraphMutex);

		RunnableList::iterator result = m_Children.begin();
		position++;
		for (unsigned int count = 0; count < position; count++)
		{
			position++;
		}

		return result;
	}

	RunnableList::const_iterator Graph::iteratorPosition(unsigned int position) const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_GraphMutex);

		RunnableList::const_iterator result = m_Children.begin();
		position++;
		for (unsigned int count = 0; count < position; count++)
		{
			position++;
		}
		return result;
	}

	bool Graph::isChild(Identifier const& childId) const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_GraphMutex);

		return (iteratorId(childId) != m_Children.end());
	}

	bool Graph::isContained(Identifier const& runnableId) const
	{
		if (isChild(runnableId))
		{
			return true;
		}
		else
		{
			for (RunnableList::const_iterator it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				Runnable &runnable = (*it)->getManagedRunnable();
				if (!runnable.isService())
				{
					RunnableGraph &graph = static_cast< RunnableGraph & >(runnable);
					if (graph.isContained(runnableId))
					{
						return true;
					}
				}
			}

			return false;
		}
	}

	RunnableManager & Graph::getChild(Identifier const& childId)
	{
		RunnableList::iterator it = iteratorId(childId);
		if (it != m_Children.end())
		{
			return **it;
		}

		std::ostringstream msg;
		msg << "child " << childId.name() << " not found in graph";
		throw NotFoundException(msg.str());
	}

	RunnableManager const& Graph::getChild(Identifier const& childId) const
	{
		RunnableList::const_iterator it = iteratorId(childId);
		if (it != m_Children.end())
		{
			return **it;
		}

		std::ostringstream msg;
		msg << "child " << childId.name() << " not found in graph";
		throw NotFoundException(msg.str());
	}

	RunnableManager & Graph::getContained(Identifier const& runnableId)
	{
		if (isChild(runnableId))
		{
			return getChild(runnableId);
		}
		else
		{
			for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				Runnable &runnable = (*it)->getManagedRunnable();
				if (!runnable.isService())
				{
					RunnableGraph &graph = static_cast< RunnableGraph & >(runnable);
					if (graph.isContained(runnableId))
					{
						return graph.getContained(runnableId);
					}
				}
			}
		}

		std::ostringstream msg;
		msg << "runnable " << runnableId.name() << " not found in graph";
		throw NotFoundException(msg.str());
	}

	RunnableManager const& Graph::getContained(Identifier const& runnableId) const
	{
		if (isChild(runnableId))
		{
			return getChild(runnableId);
		}
		else
		{
			for (RunnableList::const_iterator it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				Runnable &runnable = (*it)->getManagedRunnable();
				if (!runnable.isService())
				{
					RunnableGraph &graph = static_cast< RunnableGraph & >(runnable);
					if (graph.isContained(runnableId))
					{
						return graph.getContained(runnableId);
					}
				}
			}
		}

		std::ostringstream msg;
		msg << "runnable " << runnableId.name() << " not found in graph";
		throw NotFoundException(msg.str());
	}

	void Graph::queueOperation(Identifier const& childId)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);
		
		GraphOperation *op = new ChildRemoval(*this, childId);
		m_Operations.push_back(op);
	}

	void Graph::queueOperation(RunnableManager &child, unsigned int index)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);
		
		GraphOperation *op = new ChildInsertion(*this, child, index);
		m_Operations.push_back(op);
	}

}