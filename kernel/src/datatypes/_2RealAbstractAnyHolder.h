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

#ifdef _UNIX
	#include <typeinfo>
#else
	#include <typeinfo.h>
#endif

#include <string>
#include <sstream>

namespace _2Real
{
	class AbstractAnyHolder
	{

	public:

		virtual ~AbstractAnyHolder() {}
		virtual std::string getHumanReadableName() const = 0;
		virtual std::type_info const& getTypeinfo() const = 0;
		virtual void writeTo( std::ostream &out ) const = 0;
		virtual void readFrom( std::istream &in ) = 0;
		virtual bool isEqualTo( AbstractAnyHolder const& other ) const = 0;
		virtual bool isLessThan( AbstractAnyHolder const& other ) const = 0;
		virtual AbstractAnyHolder* clone() const = 0;
		virtual AbstractAnyHolder* create() const = 0;
		virtual void set( AbstractAnyHolder const& other ) = 0;

	};
}
