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

#include "helpers/_2RealAny.h"
#include "helpers/_2RealAnyHolder.h"
#include <limits>

#undef min
#undef max

namespace _2Real
{
	// ARGH move to 'TypeInitializers.h'

	template< typename TType >
	TType init();

	template< >
	inline int init< int >()
	{
		return 0;
	}

	template< >
	inline std::string init< std::string >()
	{
		return std::string( "" );
	}

	template< >
	inline float init< float >()
	{
		return 0.f;
	}

	///////////////////////////////////////////////////////////////

	class FieldDescriptor
	{
	public:
		FieldDescriptor() {}
		virtual ~FieldDescriptor() {}
		virtual AbstractAnyHolder * createAnyHolder() const = 0;
	};

	// this is like an any holder
	// maybe i should not create a new one each time?

	template< typename TType >
	class FieldDescriptor_t : public FieldDescriptor
	{

	public:

		FieldDescriptor_t() : FieldDescriptor(), mInitValue() {}
		FieldDescriptor_t( TType const& initValue ) : FieldDescriptor(), mInitValue( initValue ) {}
		AbstractAnyHolder * createAnyHolder() const
		{
			// TODO: range
			return new AnyHolder< TType >( mInitValue );
		}

	private:

		TType				mInitValue;

	};
}
