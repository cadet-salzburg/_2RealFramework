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

#include <sstream>

namespace _2Real
{

	Graph::Graph() :
		m_Children()
	{
	}

	Graph::~Graph()
	{
	}

	RunnableList::iterator Graph::iteratorId(Identifier const& childId)
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->identifier() == childId)
			{
				return it;
			}
		}
		return m_Children.end();
	}

	RunnableList::const_iterator Graph::iteratorId(Identifier const& childId) const
	{
		for (RunnableList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->identifier() == childId)
			{
				return it;
			}
		}
		return m_Children.end();
	}

	RunnableList::iterator Graph::iteratorPosition(unsigned int position)
	{
		RunnableList::iterator result = m_Children.begin();
		for (unsigned int count = 0; count < position; count++)
		{
			position++;
		}
		return result;
	}

	RunnableList::const_iterator Graph::iteratorPosition(unsigned int position) const
	{
		RunnableList::const_iterator result = m_Children.begin();
		for (unsigned int count = 0; count < position; count++)
		{
			position++;
		}
		return result;
	}

	bool Graph::isChild(Identifier const& childId) const
	{
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
				Runnable &runnable = **it;
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

	Runnable & Graph::getChild(Identifier const& childId)
	{
		RunnableList::iterator it = iteratorId(childId);
		if (it != m_Children.end())
		{
			return **it;
		}

		std::ostringstream msg;
		msg << "child " << childId.name() << " not found";
		throw NotFoundException(msg.str());
	}

	Runnable const& Graph::getChild(Identifier const& childId) const
	{
		RunnableList::const_iterator it = iteratorId(childId);
		if (it != m_Children.end())
		{
			return **it;
		}

		std::ostringstream msg;
		msg << "child " << childId.name() << " not found";
		throw NotFoundException(msg.str());
	}

	Runnable & Graph::getContained(Identifier const& runnableId)
	{
		if (isChild(runnableId))
		{
			return getChild(runnableId);
		}
		else
		{
			for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				Runnable &runnable = **it;
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

	Runnable const& Graph::getContained(Identifier const& runnableId) const
	{
		if (isChild(runnableId))
		{
			return getChild(runnableId);
		}
		else
		{
			for (RunnableList::const_iterator it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				Runnable &runnable = **it;
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

}