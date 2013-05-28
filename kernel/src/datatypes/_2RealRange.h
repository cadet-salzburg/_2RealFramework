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

#include <limits>

#undef min
#undef max

namespace _2Real
{
	// ranges for range checking

	// string: not really cool
	// numeric types: makes sense
	// vectors: yeah, if types are numeric

	template< typename TType >
	class Range_t
	{
	public:
		Range_t() {}
		Range_t( TType const& min, TType const& max ) {}
		virtual ~Range_t() {}
		virtual bool isInRange() 
	private:
		TType		min;
		TType		max;
	};

	template< typename TType >
	class NullRange_t : public Range_t< TType >
	{
	public:
		NullRange_t() : Range_t() {}
	};

	template< typename TType >
	Range_t< TType > minmax()
	{
		return NullRange_t< TType >();
	}

	template< >
	inline Range_t< int > minmax< int >()
	{
		return Range_t< int >( std::numeric_limits< int >::min(), std::numeric_limits< int >::max() );
	}
}
