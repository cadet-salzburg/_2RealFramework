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

	void RunnableGraph::flagChildren(unsigned int count)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_UpdateMutex);

		m_ChildrenInUpdate = count;
	}

	void RunnableGraph::childFinished(Identifier const& childId)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_UpdateMutex);

		//get the child in question out of its updating state
		RunnableManager *child = *iteratorId(childId);
		child->updateComplete();

		//if all children are finished, signal that the update is complete
		m_ChildrenInUpdate--;
		if (m_ChildrenInUpdate == 0)
		{
			m_ChildrenFinished.set();
		}
	}
}