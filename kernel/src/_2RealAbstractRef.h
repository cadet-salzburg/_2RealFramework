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

#include "Poco/Any.h"
#include "Poco/SharedPtr.h"

namespace _2Real
{

	class AbstractRef
	{

	public:

		/**
		*	
		*/
		typedef Poco::SharedPtr< Poco::Any >	SharedAny;

		/**
		*
		*/
		virtual void extractFrom(SharedAny _any) throw(...) = 0;

		/**
		*
		*/
		virtual void extractFrom(Poco::Any const& _any) throw(...) = 0;

		/**
		*	
		*/
		virtual SharedAny getAny() = 0;

		/**
		*	
		*/
		AbstractRef();

		/**
		*	
		*/
		AbstractRef(AbstractRef const& _src) throw(...);

		/**
		*	
		*/
		AbstractRef& operator=(AbstractRef const& _src) throw(...);

		/**
		*	
		*/
		~AbstractRef();



	};

}