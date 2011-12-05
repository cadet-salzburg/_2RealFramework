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

	Graph::Graph()
	{
	}

	Graph::~Graph()
	{
	}

	RunnableList::iterator Graph::iteratorId(Identifier const& id)
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->identifier() == id)
			{
				return it;
			}
		}

		return m_Children.end();
	}

	RunnableList::const_iterator Graph::iteratorId(Identifier const& id) const
	{
		for (RunnableList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->identifier() == id)
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

	const bool Graph::isChild(Identifier const& id) const
	{
		return (iteratorId(id) != m_Children.end());
	}

	const bool Graph::contains(Identifier const& id) const
	{
		if (isChild(id))
		{
			return true;
		}
		else
		{
			RunnableList::const_iterator it;
			for (it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				if (!(*it)->isService())
				{
					RunnableGraph *child = static_cast< RunnableGraph * >(*it);
					if (child->contains(id))
					{
						return true;
					}
				}
			}

			return false;
		}
	}

	Runnable & Graph::getChild(Identifier const& id)
	{
		RunnableList::iterator it = iteratorId(id);
		if (it != m_Children.end())
		{
			return **it;
		}
		else
		{
			for (it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if (!(*it)->isService())
				{
					RunnableGraph *child = static_cast< RunnableGraph * >(*it);
					if (child->contains(id))
					{
						return child->getChild(id);
					}
				}
			}
		}

		std::ostringstream msg;
		msg << "child " << id.name() << " not found in graph";
		throw NotFoundException(msg.str());
	}

	Runnable const& Graph::getChild(Identifier const& id) const
	{
		RunnableList::const_iterator it = iteratorId(id);
		if (it != m_Children.end())
		{
			return **it;
		}
		else
		{
			for (it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if ((*it)->type() != "service")
				{
					RunnableGraph *child = static_cast< RunnableGraph * >(*it);
					if (child->contains(id))
					{
						return child->getChild(id);
					}
				}
			}
		}

		std::ostringstream msg;
		msg << "child " << id.name() << " not found in graph";
		throw NotFoundException(msg.str());
	}

}