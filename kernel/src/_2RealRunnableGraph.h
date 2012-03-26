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

#include "_2RealRunnable.h"
#include "_2RealGraph.h"

#include "Poco/Event.h"
#include "Poco/Mutex.h"

namespace _2Real
{

	/**
	*	sequences & synchronizations are runnable graphs,
	*	i.e. graphs that have a run method
	*	this means that insert / remove operations might happen while the graph is running
	*/

	class RunnableGraph : public Runnable, public Graph
	{

	public:

		RunnableGraph(Identifier const& id, SystemImpl &system);
		virtual ~RunnableGraph();

		void setup();
		void shutdown();
		void insertChild(RunnableManager &child, unsigned int position);
		void removeChild(Identifier const& childId);
		void resetData();

		/**
		*	called by a thread once a child has finished its update
		*/
		void childFinished(Identifier const& childId);

		RunnableList const&	getChildren() const { return RunnableList(); }
		InputMap const&		getInlets() const { return InputMap(); }

		void prepareForAbort();

	protected:

		RunnableManager * getFirstChild();
		RunnableManager * getNextChild();

		Poco::Event			m_ChildrenFinished;
		void flagChildren(unsigned int count);

	private:

		Poco::FastMutex				m_Mutex;
		RunnableList::iterator		m_ListIterator;


		/**
		*	
		*/
		Poco::FastMutex				m_UpdateMutex;
		unsigned int				m_ChildrenInUpdate;

	};

}