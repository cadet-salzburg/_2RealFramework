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

#include "Poco/Mutex.h"

namespace _2Real
{

	/**
	*	system graphs, sequences and synchronizations are graphs
	*	graph = pretty much something that can contain runnables
	*/

	class RunnableManager;
	class GraphOperation;

	typedef std::list< RunnableManager * >	RunnableList;

	class Identifier;

	class Graph
	{

	public:

		Graph();
		virtual ~Graph();

		unsigned int childCount() const;

		bool isContained(Identifier const& runnableId) const;
		bool isChild(Identifier const& runnableId) const;

		RunnableManager & getChild(Identifier const& childId);
		RunnableManager const& getChild(Identifier const& childId) const;
		RunnableManager & getContained(Identifier const& runnableId);
		RunnableManager const& getContained(Identifier const& runnableId) const;

		void queueOperation(RunnableManager &child, unsigned int position);
		void queueOperation(Identifier const& childId);

	protected:

		/**
		*	causes all desired operations to be carried out
		*/
		bool executeOperations();

		void insertChild(RunnableManager &child, unsigned int position);
		void removeChild(Identifier const& childId);

		RunnableList::iterator				iteratorId(Identifier const& childId);
		RunnableList::const_iterator		iteratorId(Identifier const& childId) const;
		RunnableList::iterator				iteratorPosition(unsigned int childPosition);
		RunnableList::const_iterator		iteratorPosition(unsigned int childPosition) const;

		RunnableList						m_Children;

	private:

		/**
		*	graphs can have a list of objects that are to remove or inserted;
		*	these operations are queued up
		*/
		std::list< GraphOperation * >	m_Operations;

		/**
		*	mutex to guarantee save access to children
		*/
		Poco::FastMutex					m_OperationsMutex;

		/**
		*	mutex used for synchronizing access to children
		*/
		Poco::FastMutex					m_GraphMutex;

	};

}