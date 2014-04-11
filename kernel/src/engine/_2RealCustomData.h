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

#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealData.h"

namespace _2Real
{
	class DataField
	{

	public:

		DataField( std::string fieldName, DataItem value ) :
			mName( std::move( fieldName ) ),
			mValue( std::move( value ) )
		{ 
			//std::cout << "data field init ctor" << std::endl;
		}

		DataField( DataField const& other ) :
			mName( other.mName ),
			mValue( other.mValue )
		{ 
			//std::cout << "data field copy ctor" << std::endl;
		}

		DataField( DataField && other ) :
			mName( std::move( other.mName ) ),
			mValue( std::move( other.mValue ) )
		{
			//std::cout << "data field move ctor" << std::endl;
		}

		DataField& operator=( DataField const& other )
		{
			if ( this == &other )
				return *this;

			mName = other.mName;
			mValue = other.mValue;

			//std::cout << "data field copy assignment" << std::endl;

			return *this;
		}

		DataField& operator=( DataField && other )
		{
			if ( this == &other )
				return *this;

			mName = std::move( other.mName );
			mValue = std::move( other.mValue );

			//std::cout << "data field move assignment" << std::endl;

			return *this;
		}

		~DataField() = default;

		std::string const& getName() const
		{
			return mName;
		}

		DataItem const& getValue() const
		{
			return mValue;
		}

		DataItem & getValue()
		{
			return mValue;
		}

		void setValue( DataItem value )
		{
			mValue = std::move( value );
		}

	private:

		std::string		mName;
		DataItem		mValue;

	};

	class SharedTypeMetainfo;
	class MetainfoId;

	class CustomDataItem
	{

		friend class SharedTypeMetainfo;
		friend class TypeMetainfoVisitor;
		friend class HumanReadableNameVisitor;

	public:

		// creates invalid custom data item
		CustomDataItem();
		// created by framework
		explicit CustomDataItem( std::shared_ptr< const MetainfoId > );
		CustomDataItem( CustomDataItem const& other );
		CustomDataItem( CustomDataItem && other );
		CustomDataItem& operator=( CustomDataItem const& other );
		CustomDataItem& operator=( CustomDataItem && other );
		~CustomDataItem() = default;

		std::string	getName() const;

		void set( std::string const& fieldName, DataItem value );
		void addField( DataField field );

		friend std::ostream& operator<<( std::ostream& out, CustomDataItem const& val );

		template< typename TData >
		TData const& getValue( std::string const& fieldName ) const
		{
			try
			{
				for ( auto &it : mDataFields )
				{
					if ( it.getName() == fieldName )
						return boost::get< TData >( it.getValue() );
				}

				throw _2Real::NotFound( fieldName );
			}
			catch ( boost::bad_get const& e )
			{
				throw _2Real::TypeMismatch( fieldName );
			}
		}

		template< typename TData >
		TData & getValue( std::string const& fieldName )
		{
			try
			{
				for ( auto &it : mDataFields )
				{
					if ( it.getName() == fieldName )
						return boost::get< TData >( it.getValue() );
				}

				throw _2Real::NotFound( fieldName );
			}
			catch ( boost::bad_get const& e )
			{
				throw _2Real::TypeMismatch( fieldName );
			}
		}

	private:

		typedef std::vector< DataField >			DataFields;
		DataFields									mDataFields;
		std::shared_ptr< const MetainfoId >			mTypeMetainfo;

	};
}
