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
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename T > struct Init;

	template< >
	struct Init< char >
	{
		static int defaultValue()
		{
			return static_cast< char >( 0 );
		}
	};

	template< >
	struct Init< unsigned char >
	{
		static int defaultValue()
		{
			return static_cast< char >( 0 );
		}
	};

	template< >
	struct Init< unsigned int >
	{
		static int defaultValue()
		{
			return static_cast< unsigned int >( 0 );
		}
	};

	template< >
	struct Init< int >
	{
		static int defaultValue()
		{
			return 0;
		}
	};

	template< >
	struct Init< std::string >
	{
		static std::string defaultValue()
		{
			return std::string();
		}
	};

	template< typename TType >
	struct Init< std::vector< TType > >
	{
		static std::vector< TType > defaultValue()
		{
			return std::vector< TType >();
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename T > struct Name;

	template< >
	struct Name< char >
	{
		static std::string humanReadableName()
		{
			return "char";
		}
	};

	template< >
	struct Name< unsigned char >
	{
		static std::string humanReadableName()
		{
			return "uchar";
		}
	};

	template< >
	struct Name< int >
	{
		static std::string humanReadableName()
		{
			return "int";
		}
	};

	template< >
	struct Name< unsigned int >
	{
		static std::string humanReadableName()
		{
			return "uint";
		}
	};

	template< >
	struct Name< float >
	{
		static std::string humanReadableName()
		{
			return "float";
		}
	};

	template< >
	struct Name< double >
	{
		static std::string humanReadableName()
		{
			return "double";
		}
	};

	template< >
	struct Name< std::string >
	{
		static std::string humanReadableName()
		{
			return "string";
		}
	};

	template< typename TType >
	struct Name< std::vector< TType > >
	{
		static std::string humanReadableName()
		{
			return std::string( "vector of " ).append( Name< TType >::humanReadableName() );
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class FieldDescriptor
	{
	public:
		FieldDescriptor() {}
		virtual ~FieldDescriptor() {}
		virtual AbstractAnyHolder * createAnyHolder() const = 0;
		virtual std::string getTypename() const = 0;
	};

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

		std::string getTypename() const
		{
			return Name< TType >::humanReadableName();
		}

	private:

		TType				mInitValue;

	};
}
