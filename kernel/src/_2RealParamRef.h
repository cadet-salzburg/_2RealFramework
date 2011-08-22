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

#include "_2RealAbstractRef.h"

namespace _2Real
{

	/**
	*
	*/

	template< typename T >
	class ParamRef : public AbstractRef
	{

	public:

		/**
		*	attempt to extract from an any ptr
		*/
		void extractFrom(AbstractRef::SharedAny _any) throw(...);

		/**
		*	create copy of value, transform into any pointer
		*/
		SharedAny getAny();

	private:

		friend class ServiceContext;

		/**
		*	
		*/
		ParamRef(T &_value);

		/**
		*	
		*/
		ParamRef(ParamRef const& _src) throw(...);

		/**
		*	
		*/
		ParamRef& operator=(ParamRef const& _src) throw (...);

		/**
		*	
		*/
		~ParamRef();

		/**
		*	reference to member variable of a service
		*	extract() will overwrite this.
		*/
		T		&m_Value;

	};

}

#include "_2RealParamRef.cpp"