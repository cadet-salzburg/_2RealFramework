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

#include "datatypes/_2RealTypes.h"
#include "datatypes/_2RealFieldDescriptor.h"
#include "datatypes/_2RealCustomData.h"

namespace _2Real
{
	template< typename TType >
	struct DataField;

	template< >
	struct DataField< char >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( char init )
		{
			return new FieldDescriptor_t< char >( init );
		}
	};

	template< >
	struct DataField< unsigned char >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( unsigned char init )
		{
			return new FieldDescriptor_t< unsigned char >( init );
		}
	};

	template< >
	struct DataField< int >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( int init )
		{
			return new FieldDescriptor_t< int >( init );
		}
	};

	template< >
	struct DataField< unsigned int >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( unsigned int init )
		{
			return new FieldDescriptor_t< unsigned int >( init );
		}
	};

	template< >
	struct DataField< float >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( float init )
		{
			return new FieldDescriptor_t< float >( init );
		}
	};

	template< >
	struct DataField< double >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( double init )
		{
			return new FieldDescriptor_t< double >( init );
		}
	};

	template< >
	struct DataField< bool >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( bool init )
		{
			return new FieldDescriptor_t< bool >( init );
		}
	};

	template< >
	struct DataField< std::string >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& init )
		{
			return new FieldDescriptor_t< std::string >( init );
		}
	};

	template< typename TType >
	struct DataField< std::vector< TType > >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::vector< TType > const& init )
		{
			return new FieldDescriptor_t< std::vector< TType > >( init );
		}
	};

	template< >
	struct DataField< CustomType >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( CustomType const& init )
		{
			return new FieldDescriptor_t< CustomType >( init, nullptr );
		}
	};

	template< typename TType >
	struct DataField
	{
	public:
		static FieldDescriptor * createFieldDescriptor( TType const& init )
		{
			return nullptr;
		}
	};
}
