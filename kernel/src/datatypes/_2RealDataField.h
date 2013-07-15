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
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const char init )
		{
			Field *f = new SimpleField( name, Name< char >::humanReadableName() );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< char >( init, field );
		}
	};

	template< >
	struct DataField< unsigned char >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const unsigned char init )
		{
			Field *f = new SimpleField( name, Name< unsigned char >::humanReadableName() );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< unsigned char >( init, field );
		}
	};

	template< >
	struct DataField< int >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const int init )
		{
			Field *f = new SimpleField( name, Name< int >::humanReadableName() );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< int >( init, field );
		}
	};

	template< >
	struct DataField< unsigned int >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const unsigned int init )
		{
			Field *f = new SimpleField( name, Name< unsigned int >::humanReadableName() );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< unsigned int >( init, field );
		}
	};

	template< >
	struct DataField< float >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const float init )
		{
			Field *f = new SimpleField( name, Name< float >::humanReadableName() );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< float >( init, field );
		}
	};

	template< >
	struct DataField< double >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const double init )
		{
			Field *f = new SimpleField( name, Name< double >::humanReadableName() );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< double >( init, field );
		}
	};

	template< >
	struct DataField< bool >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, const bool init )
		{
			Field *f = new SimpleField( name, Name< bool >::humanReadableName() );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< bool >( init, field );
		}
	};

	template< >
	struct DataField< std::string >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, std::string const& init )
		{
			Field *f = new SimpleField( name, Name< std::string >::humanReadableName() );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< std::string >( init, field );
		}
	};

	template< typename TType >
	struct DataField< std::vector< TType > >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, std::vector< TType > const& init )
		{
			Field *f = new SimpleField( name, Name< std::vector< TType > >::humanReadableName() );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< std::vector< TType > >( init, field );
		}
	};

	template< >
	struct DataField< CustomType >
	{
	public:
		static FieldDescriptor * createFieldDescriptor( std::string const& name, std::string const& type, CustomType const& init, Fields const& fields )
		{
			Field *f = new ComplexField( name, type, fields );
			std::shared_ptr< const Field > field( f );
			return new FieldDescriptor_t< CustomType >( init, field );
		}
	};
}
