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

#include "_2RealRunnableGraph.h"
#include "_2RealRunnableManager.h"

#include <iostream>

namespace _2Real
{
	RunnableGraph::RunnableGraph(Identifier const& id, SystemGraph &system) :
		Runnable(id, system),
		Graph(),
		m_ListIterator(m_Children.begin()),
		m_ChildrenInUpdate(0)
	{
	}

	RunnableGraph::~RunnableGraph()
	{
		if (childCount() > 0)
		{
			shutdown();
		}
	}

	void RunnableGraph::setup()
	{
		//for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		//{
		//	(*it)->setup();
		//}
	}

	void RunnableGraph::shutdown()
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			(*it)->shutdown();
		}

		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			delete *it;
		}

		m_Children.clear();
	}

	void RunnableGraph::removeChild(Identifier const& childId)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		RunnableList::iterator it = iteratorId(childId);

		if (it == m_ListIterator)
		{
			//increase the stored iterator so that the child can safely be erased from the list
			m_ListIterator++;
		}

		if ((*it)->isUpdating())
		{
			//that's one less to wait for
			m_ChildrenInUpdate--;
			//in theory, this one could be the last one that is being waited for
			if (m_ChildrenInUpdate == 0)
			{
				m_ChildrenFinished.set();
			}
		}

		m_Children.erase(it);
	}

	void RunnableGraph::insertChild(RunnableManager &child, unsigned int index)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		RunnableList::iterator it = iteratorPosition(index);

		m_Children.insert(it, &child);

		child.getManagedRunnable().setFather(*this);

		if (it == m_ListIterator)
		{
			//the new child will be updated next
			m_ListIterator--;
		}

		if ((*it)->isUpdating())
		{
			//that's one more to wait for
			m_ChildrenInUpdate++;

			//i need to handle this at some point...
		}
	}

	//keep in mind, this can happen at any time, not just after an update
	void RunnableGraph::childFinished(Identifier const& childId)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		//get the child in question out of its updating state
		RunnableManager *child = *iteratorId(childId);
		child->updateComplete();

		m_ChildrenInUpdate--;
		if (m_ChildrenInUpdate == 0)
		{
			m_ChildrenFinished.set();
		}
	}

	bool RunnableGraph::updateFirstChild(PooledThread &thread)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		if (m_Children.empty())
		{
			return false;
		}

		RunnableManager *first = *(m_Children.begin());

		m_ListIterator = m_Children.begin();
		m_ListIterator++;

		first->update(thread);
		m_ChildrenInUpdate++;

		return true;
	}

	bool RunnableGraph::updateNextChild(PooledThread &thread)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		if (m_ListIterator == m_Children.end())
		{
			return false;
		}

		RunnableManager *child = *m_ListIterator;
		m_ListIterator++;

		child->update(thread);
		m_ChildrenInUpdate++;

		return true;
	}
}