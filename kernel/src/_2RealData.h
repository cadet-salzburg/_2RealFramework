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

#include "_2RealVariableName.h"

#include <map>
#include <string>

#include "Poco/Any.h"
#include "Poco/SharedPtr.h"

namespace _2Real
{

	/**
	*
	*/

	typedef Poco::SharedPtr< Poco::Any >	SharedAnyPtr;

	template< typename Key >
	class Data
	{

	public:

		Data();
		Data(Data const& _src);
		Data& operator=(Data const& _src);
		~Data();

		/**
		*
		*/
		const bool remove(Key const& _name);

		/**
		*
		*/
		const bool contains(Key const& _name) const;

		/**
		*
		*/
		SharedAnyPtr getAny(Key const& _name) const;

		/**
		*
		*/
		void insertAny(Key const& _name, SharedAnyPtr &_any);

		/**
		*
		*/
		unsigned int const& size() const;

	private:

		typedef std::map< Key, SharedAnyPtr >	NamedAnyMap;
		typedef std::pair< Key, SharedAnyPtr >	NamedAny;

		NamedAnyMap		m_DataMap;

	};

}

#include "_2RealData.cpp"