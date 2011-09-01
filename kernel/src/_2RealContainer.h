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

namespace _2Real
{

	/**
	*
	*/

	class Container : public AbstractContainer
	{

	private:

		/**
		*
		*/
		typedef std::list< AbstractContainer *const >	ChildList;
		
	public:

		/**
		*	
		*/
		Container(IdentifierImpl *const _id) throw(...);

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
		*
		*/
		const unsigned int childCount() const;

		/**
		*	inserts child before child + _id, throw exception if _id not found among children
		*/
		void addBefore(AbstractContainer *const child, unsigned int const& _id) throw(...);

		/**
		*	adds child in the last position
		*/
		void append(AbstractContainer *const child);

		/**
		*	adds child in the last position
		*/
		void remove(unsigned int const& _id) throw(...);

		/**
		*	returns pointer to child, removes from children & appends to _father's children
		*/
		AbstractContainer *const getChild(unsigned int const& _id, Container *const _father);

		/**
		*	returns pointer to child without removing it, NULL otherwise
		*/
		AbstractContainer *const getChild(unsigned int const& _id);

		/**
		*	
		*/
		IdentifierList setupParams() const throw(...);

		/**
		*	
		*/
		IdentifierList inputParams() const throw(...);

		/**
		*	
		*/
		IdentifierList outputParams() const throw(...);

	private:

		/**
		*	returns iterator to child
		*/
		ChildList::iterator findChild(unsigned int const& _id);

		/**
		*
		*/
		ChildList						m_Children;

	};

}