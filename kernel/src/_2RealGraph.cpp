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
#include "_2RealRunnableGraph.h"

namespace _2Real
{

	Graph::Graph()
	{
	}

	Graph::~Graph()
	{
	}

	std::list< unsigned int > Graph::childIDs() const
	{
		std::list< unsigned int > result;
		for (RunnableList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			result.push_back((*it)->id());
		}

		return result;
	}

	RunnableList::iterator Graph::iteratorId(unsigned int const& _id)
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->id() == _id)
			{
				return it;
			}
		}

		return m_Children.end();
	}

	RunnableList::const_iterator Graph::iteratorId(unsigned int const& _id) const
	{
		for (RunnableList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->id() == _id)
			{
				return it;
			}
		}

		return m_Children.end();
	}

	RunnableList::iterator Graph::iteratorPosition(unsigned int const& _pos)
	{
		RunnableList::iterator position = m_Children.begin();
		for (unsigned int count = 0; count < _pos; count++)
		{
			position++;
		}

		return position;
	}

	RunnableList::const_iterator Graph::iteratorPosition(unsigned int const& _pos) const
	{
		RunnableList::const_iterator position = m_Children.begin();
		for (unsigned int count = 0; count < _pos; count++)
		{
			position++;
		}

		return position;
	}

	const bool Graph::isChild(unsigned int const& _id) const
	{
		return (iteratorId(_id) != m_Children.end());
	}

	const bool Graph::isInGraph(unsigned int const& _id) const
	{
		RunnableList::const_iterator it = iteratorId(_id);
		if (it != m_Children.end())
		{
			return true;
		}
		else
		{
			for (it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if (!(*it)->hasParameters())
				{
					RunnableGraph *child = static_cast< RunnableGraph * >(*it);
					if (child->isChild(_id))
					{
						return true;
					}
				}
			}

			return false;
		}
	}

	Runnable *const Graph::getChild(unsigned int const& _id)
	{
		Runnable *child;
		RunnableList::iterator it = this->iteratorId(_id);
		
		if (it != m_Children.end())
		{
			child = *it;
			removeChild(child->id());
			return child;
		}
		else
		{
			for (it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if (!(*it)->hasParameters())
				{
					RunnableGraph *graph = static_cast< RunnableGraph * >(*it);
					if ((child = graph->getChild(_id)) != NULL)
					{
						return child;
					}
				}
			}
		}

		throw ChildNotFoundException();
	}

	Runnable const *const Graph::findChild(unsigned int const& _id) const
	{
		RunnableList::const_iterator it = this->iteratorId(_id);
		if (it != m_Children.end())
		{
			return *it;
		}
		else
		{
			for (it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if (!(*it)->hasParameters())
				{
					RunnableGraph *child = static_cast< RunnableGraph * >(*it);
					const Runnable *result = child->findChild(_id);
					if (result)
					{
						return result;
					}
				}
			}
		}

		throw ChildNotFoundException();
	}

	Runnable *const Graph::findChild(unsigned int const& _id)
	{
		RunnableList::iterator it = this->iteratorId(_id);
		
		if (it != m_Children.end())
		{
			return *it;
		}
		else
		{
			for (it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if (!(*it)->hasParameters())
				{
					RunnableGraph *child = static_cast< RunnableGraph * >(*it);
					Runnable *result = child->findChild(_id);
					if (result)
					{
						return result;
					}
				}
			}
		}

		throw ChildNotFoundException();
	}

}