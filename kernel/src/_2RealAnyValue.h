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

#include "_2RealAbstractValue.h"

#include "Poco/Any.h"
#include "Poco/SharedPtr.h"

namespace _2Real
{

	/**
	*
	*/

	typedef Poco::SharedPtr< Poco::Any >	SharedAnyPtr;

	template< typename T >
	class AnyValue : public AbstractValue
	{

	public:

		AnyValue(std::string const& _name, T &_value);
		AnyValue(AnyValue const& _src);
		AnyValue& operator=(AnyValue const& _src);
		~AnyValue();

		/**
		*	attempt to extract from anyptr
		*/
		void extract(SharedAnyPtr &_anyPtr);
		
		/**
		*	create copy of value, transform into any pointer
		*/
		SharedAnyPtr& sharedPtr();

		T getCopy();

	private:

		T		&m_Value;

	};

}

#include "_2RealAnyValue.cpp"