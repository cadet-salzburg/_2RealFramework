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

#include "helpers/_2RealUnusedArg.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	template< typename TType >
	class ADeleter
	{
	public:
		virtual ~ADeleter() {}
		virtual void operator()( TType const*& val ) = 0;
		virtual ADeleter * clone() const = 0;
	};

	template< typename TType >
	class NullDeleter : public ADeleter< TType >
	{
	public:
		void operator()( TType const*& val )
		{
			UNUSED( val );
		}
		ADeleter * clone() const
		{
			return new NullDeleter< TType >;
		}
	};

	template< typename TType >
	class Deleter : public ADeleter< TType >
	{
	public:
		void operator()( TType const*& val )
		{
			delete val;
			val = nullptr;
		}
		ADeleter * clone() const
		{
			return new Deleter< TType >;
		}
	};

	template< typename TType >
	class ArrayDeleter : public ADeleter< TType >
	{
	public:
		void operator()( TType const*& val )
		{
			delete [] val;
			val = nullptr;
		}
		ADeleter * clone() const
		{
			return new ArrayDeleter< TType >;
		}
	};
}