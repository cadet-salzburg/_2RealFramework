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

#include "datatypes/_2RealAnyHolder.h"
#include "engine/_2RealTypeMetadataRef.h"
#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealIdentifiable.h"

namespace _2Real
{
	class CustomType
	{

	public:

		CustomType();
		explicit CustomType( TypeMetadataConstRef );
		CustomType( CustomType const& other );
		~CustomType();

		// urgh, this involves a copy -> for custom types composed of other custom types, get shared ptr w / get & the set on those
		template< typename TType >
		void set( std::string const& field, TType const& value )
		{
			setValueInternal( field, new AnyHolder< TType >( value ) );
		}

		template< typename TType >
		std::shared_ptr< const TType > get( std::string const& field ) const
		{
			AbstractAnyHolder const* value = getValueInternal( field );

			std::shared_ptr< const TType > result = extract< TType >( *value );
			return result;
		}

		template< typename TType >
		std::shared_ptr< TType > get( std::string const& field )
		{
			AbstractAnyHolder *value = getValueInternal( field );
			std::shared_ptr< TType > result = extract< TType >( *value );
			return result;
		}

		// called by getCurrentValueAsString on an inlet -> what the fuck
		void writeTo( std::ostringstream &out ) const;
		// called by hasChanged() on inlet handles
		bool isEqualTo( CustomType const& other ) const;

		bool operator==( CustomType const& other ) const { return false; }
		bool operator<( CustomType const& other ) const { return false; }

	private:

		friend class TypeConverter;

		void initField( std::string const& name, _2Real::AbstractAnyHolder *init );

		typedef std::map< std::string, _2Real::AbstractAnyHolder * >		DataFields;

		DataFields									mDataFields;
		std::shared_ptr< const TypeMetadata >		mMetadata;

		DataFields::iterator iter( std::string const& name );
		DataFields::const_iterator constIter( std::string const& name ) const;

		void setValueInternal( std::string const& field, AbstractAnyHolder *value );
		AbstractAnyHolder const* getValueInternal( std::string const& field ) const;
		AbstractAnyHolder * getValueInternal( std::string const& field );

	};

	//------------ since field are custom types, names & init have to be given.....

	template< >
	struct Name< CustomType >
	{
		static std::string humanReadableName()
		{
			return "CustomType";
		}
	};

	template< >
	struct Init< CustomType >
	{
		static CustomType defaultValue()
		{
			return CustomType();
		}
	};
}
