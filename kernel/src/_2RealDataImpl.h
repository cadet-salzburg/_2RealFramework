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

#include <map>
#include <string>

#include "Poco/Any.h"
#include "Poco/SharedPtr.h"

namespace _2Real
{

	/**
	*
	*/

	class DataImpl
	{

	public:

		/**
		*	
		*/
		typedef Poco::SharedPtr< Poco::Any >	SharedAny;

		/**
		*	
		*/
		DataImpl();

		/**
		*	
		*/
		DataImpl(DataImpl const& _src);

		/**
		*	
		*/
		DataImpl& operator=(DataImpl const& _src);

		/**
		*	
		*/
		~DataImpl();

		/**
		*
		*/
		void remove(unsigned int const& _id);

		/**
		*
		*/
		const bool contains(unsigned int const& _id) const;

		/**
		*
		*/
		DataImpl::SharedAny getAny(unsigned int const& _id) const;

		/**
		*
		*/
		void insertAny(unsigned int const& _id, DataImpl::SharedAny &_any);

		/**
		*
		*/
		const unsigned int size() const;

		/**
		*
		*/
		void merge(DataImpl const& src);

	private:

		/**
		*	
		*/
		typedef std::map< const unsigned int, SharedAny >		AnyMap;

		/**
		*	
		*/
		typedef std::pair< const unsigned int, SharedAny >		NamedAny;

		/**
		*	
		*/
		AnyMap													m_Data;

	};

}
