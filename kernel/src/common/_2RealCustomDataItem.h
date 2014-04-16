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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealException.h"
#include "common/_2RealDataField.h"

namespace _2Real
{
	class CustomTypeMetainfo;
	class MetainfoId;

	class CustomDataItem
	{

		friend class CustomTypeMetainfoImpl;
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

				throw NotFound( fieldName );
			}
			catch ( boost::bad_get const& e )
			{
				throw TypeMismatch( fieldName );
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

				throw NotFound( fieldName );
			}
			catch ( boost::bad_get const& e )
			{
				throw TypeMismatch( fieldName );
			}
		}

	private:

		typedef std::vector< DataField >			DataFields;
		DataFields									mDataFields;
		std::shared_ptr< const MetainfoId >			mTypeMetainfo;

	};
}
