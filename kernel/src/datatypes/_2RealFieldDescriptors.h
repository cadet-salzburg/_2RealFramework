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

#include "datatypes/_2RealFieldDescriptor.h"
#include "datatypes/_2RealDataField.h"
#include "datatypes/_2RealCustomData.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	template< typename TType >
	struct FieldDesc;

	template< >
	struct FieldDesc< char >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const char init )
		{
			DataFieldRef field =  DataField::createSimpleField( name, Name< char >::humanReadableName() );
			return new FieldDescriptor_t< char >( init, field );
		}
	};

	template< >
	struct FieldDesc< unsigned char >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const unsigned char init )
		{
			DataFieldRef field =  DataField::createSimpleField( name, Name< unsigned char >::humanReadableName() );
			return new FieldDescriptor_t< unsigned char >( init, field );
		}
	};

	template< >
	struct FieldDesc< int >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const int init )
		{
			DataFieldRef field =  DataField::createSimpleField( name, Name< int >::humanReadableName() );
			return new FieldDescriptor_t< int >( init, field );
		}
	};

	template< >
	struct FieldDesc< unsigned int >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const unsigned int init )
		{
			DataFieldRef field =  DataField::createSimpleField( name, Name< unsigned int >::humanReadableName() );
			return new FieldDescriptor_t< unsigned int >( init, field );
		}
	};

	template< >
	struct FieldDesc< float >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const float init )
		{
			DataFieldRef field =  DataField::createSimpleField( name, Name< float >::humanReadableName() );
			return new FieldDescriptor_t< float >( init, field );
		}
	};

	template< >
	struct FieldDesc< double >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const double init )
		{
			DataFieldRef field =  DataField::createSimpleField( name, Name< double >::humanReadableName() );
			return new FieldDescriptor_t< double >( init, field );
		}
	};

	template< >
	struct FieldDesc< bool >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const bool init )
		{
			DataFieldRef field =  DataField::createSimpleField( name, Name< bool >::humanReadableName() );
			return new FieldDescriptor_t< bool >( init, field );
		}
	};

	template< >
	struct FieldDesc< std::string >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, std::string const& init )
		{
			DataFieldRef field =  DataField::createSimpleField( name, Name< std::string >::humanReadableName() );
			return new FieldDescriptor_t< std::string >( init, field );
		}
	};

	template< typename TType >
	struct FieldDesc< std::vector< TType > >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, std::vector< TType > const& init )
		{
			DataFieldRef field =  DataField::createSimpleField( name, Name< std::vector< TType > >::humanReadableName() );
			return new FieldDescriptor_t< std::vector< TType > >( init, field );
		}
	};

	template< >
	struct FieldDesc< CustomType >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, std::pair< std::string, std::string > const& type, CustomType const& init, DataFields const& fields )
		{
			DataFieldRef field =  DataField::createComplexField( name, type, fields );
			return new FieldDescriptor_t< CustomType >( init, field );
		}
	};
}
