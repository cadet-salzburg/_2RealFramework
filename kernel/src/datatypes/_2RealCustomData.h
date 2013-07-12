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
#include "datatypes/_2RealAbstractAnyHolder.h"
#include <map>

namespace _2Real
{
	class TypeMetadata;

	namespace bundle
	{
		class TypeMetainfo;
	}

	namespace app
	{
		class TypeMetainfo;
	}

	class CustomType
	{

	public:

		explicit CustomType( bundle::TypeMetainfo const& meta );
		explicit CustomType( app::TypeMetainfo const& meta );
		explicit CustomType( TypeMetadata const& meta );

		virtual ~CustomType();

		CustomType() {}
		CustomType( CustomType const& other );
		//CustomType& operator=( CustomType const& other )
		//{
		//	return *this;
		//};



	public:

		void initFrom( TypeMetadata const& meta );
		void cloneFrom( CustomType const& other );

		template< typename TType >
		void set( std::string const& field, TType const& value )
		{
			// makes a copy of the value!
			setValueInternal( field, new AnyHolder< TType >( value ) );
		}

		template< typename TType >
		std::shared_ptr< const TType > get( std::string const& field ) const
		{
			AbstractAnyHolder const* value = getValueInternal( field );
			// may throw
			std::shared_ptr< const TType > result = extract< TType >( *value );
			return result;
		}

		template< typename TType >
		std::shared_ptr< TType > get( std::string const& field )
		{
			AbstractAnyHolder *value = getValueInternal( field );
			// may throw
			std::shared_ptr< TType > result = extract< TType >( *value );
			return result;
		}

		// called by getCurrentValueAsString on an inlet -> what the fuck
		void writeTo( std::ostringstream &out ) const;
		// called by hasChanged() on inlet handles
		bool isEqualTo( CustomType const& other ) const;

		bool operator==( CustomType const& other ) const { return false; }
		bool operator<( CustomType const& other ) const { return false; }

	protected:

		void initField( std::string const& name, _2Real::AbstractAnyHolder *init );

		typedef std::map< std::string, _2Real::AbstractAnyHolder * >		DataFields;
		DataFields															mDataFields;

		DataFields::iterator iter( std::string const& name );
		DataFields::const_iterator constIter( std::string const& name ) const;

	private:

		void setValueInternal( std::string const& field, AbstractAnyHolder *value );
		AbstractAnyHolder const* getValueInternal( std::string const& field ) const;
		AbstractAnyHolder * getValueInternal( std::string const& field );

	};

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
