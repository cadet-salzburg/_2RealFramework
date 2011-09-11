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

#include "_2RealAbstractContainer.h"

#include <list>
#include <map>

namespace _2Real
{

	/**
	*	currently represents sequence containers, synchronization containers &
	*	the top level containers (nirvana)
	*	concrete behaviour is defined by the type() of the container
	*	doing this seemed like a good idea at some point, some weeks ago
	*	TODO: create individual classes. this class is a huge mess, and it pisses me of :/
	*/

	class Container : public AbstractContainer
	{

	public:

		/**
		*	
		*/
		Container(Id *const _id, ApplicationCallback *const _output) throw(...);

		/**
		*	this one should not be called, ever
		*/
		Container(Container const& _src) throw(...);

		/**
		*	this one should not be called, ever
		*/
		Container& operator=(Container const& _src) throw(...);

		/**
		*	calls destructor of all children
		*/
		~Container();

		/**
		*	config calls config of all children
		*/
		void configure() throw(...);

		/**
		*	
		*/
		void run() throw(...);

		/**
		*	
		*/
		void update() throw(...);

		/**
		*	
		*/
		void shutdown() throw(...);

		/**
		*	calls setup function of service when called for the first time
		*/
		void checkConfiguration() throw(...);

		/**
		*	returns nr of children
		*/
		const unsigned int childCount() const;

		/**
		*	
		*/
		IDs childIDs() const;

		/**
		*	
		*/
		std::list< AbstractContainer * > children();

		/**
		*	inserts child before child + _id, throw exception if _id not found among children
		*/
		void add(AbstractContainer *const _child, unsigned int const& _index) throw(...);

		/**
		*	removes child. completely stops production graph, breaks all IO connections & removes all listeners
		*/
		void remove(unsigned int const& _id) throw(...);

		/**
		*	returns pointer to container if located somewhere in tree, null otherwise
		*	also performs remove!
		*/
		AbstractContainer *const get(unsigned int const& _id);

		/**
		*	returns pointer to container if located somewhere in tree, null otherwise
		*/
		AbstractContainer const *const find(unsigned int const& _id) const;

		/**
		*	returns pointer to container if located somewhere in tree, null otherwise
		*/
		AbstractContainer *const find(unsigned int const& _id);

		/**
		*	
		*/
		void startChild(unsigned int const& _id) throw(...);

		/**
		*
		*/
		void stopChild(unsigned int const& _id) throw(...);

		/**
		*	
		*/
		IDs setupParamIDs() const throw(...);

		/**
		*	
		*/
		IDs inputSlotIDs() const throw(...);

		/**
		*	
		*/
		IDs outputSlotIDs() const throw(...);

		/**
		*	
		*/
		std::list< ServiceSlot * > inputSlots() throw(...);

		/**
		*	
		*/
		std::list< ServiceSlot * > outputSlots() throw(...);

	private:

		/**
		*	returns iterator to child
		*/
		ContainerList::iterator child(unsigned int const& _id);

		/**
		*	returns const iterator to child
		*/
		ContainerList::const_iterator child(unsigned int const& _id) const;

		/**
		*	returns iterator to position
		*/
		ContainerList::iterator position(unsigned int const& _index);

		/**
		*	returns const iterator to position
		*/
		ContainerList::const_iterator position(unsigned int const& _index) const;

		/**
		*	child container list
		*/
		ContainerList								m_Children;

		/**
		*	threads. used by nirvana. stupid poco threadpool wont let me
		*	join individual threads.
		*/
		std::map< unsigned int, Poco::Thread * >	m_Threads;

		/**
		*	threadpool. used by sync containers. i hate life.
		*/
		Poco::ThreadPool							m_ThreadPool;

	};

}