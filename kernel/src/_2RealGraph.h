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

#include <list>

namespace _2Real
{

	/**
	*	system graphs, sequences and synchronizations are graphs
	*/

	class Runnable;
	typedef std::list< Runnable * >	RunnableList;

	class Identifier;

	class Graph
	{

	public:

		Graph();
		virtual ~Graph();

		const unsigned int childCount() const
		{
			return m_Children.size();
		}

		std::list< Identifier > childIDs() const;

		std::list< Runnable * > & children()
		{
			return m_Children;
		}

		std::list< Runnable * > const& children() const
		{
			return m_Children;
		}

		bool const contains(Identifier const& id) const;
		bool const isChild(Identifier const& id) const;

		Runnable & getChild(Identifier const& id);
		Runnable const& getChild(Identifier const& id) const;

		virtual void insertChild(Runnable &child, unsigned int const& index) = 0;
		virtual void removeChild(Identifier const& id) = 0;

	protected:

		RunnableList::iterator				iteratorId(Identifier const& id);
		RunnableList::const_iterator		iteratorId(Identifier const& id) const;
		RunnableList::iterator				iteratorPosition(unsigned int const& pos);
		RunnableList::const_iterator		iteratorPosition(unsigned int const& pos) const;

		RunnableList						m_Children;

	};

}