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

#include "common/_2RealCustomDataItem.h"
#include "engine/_2RealCustomTypeMetainfoImpl.h"
#include "engine/_2RealHumanReadableNameVisitor.h"
#include "common/_2RealException.h"
#include "engine/_2RealId.h"

namespace _2Real
{
	std::ostream& operator<<( std::ostream& out, CustomDataItem const& val )
	{
		for ( auto const& it : val.mDataFields )
			out << it.getName() << " : " << it.getValue();
		return out;
	}

	CustomDataItem::CustomDataItem() :
		mDataFields(),
		mTypeMetainfo()
	{
	}

	CustomDataItem::CustomDataItem( std::shared_ptr< const MetainfoId > type ) :
		mDataFields(),
		mTypeMetainfo( type )
	{
	}

	CustomDataItem::CustomDataItem( CustomDataItem const& other ) :
		mDataFields( other.mDataFields ),
		mTypeMetainfo( other.mTypeMetainfo )
	{
	}

	CustomDataItem::CustomDataItem( CustomDataItem && other ) :
		mDataFields( std::move( other.mDataFields ) ),
		mTypeMetainfo( std::move( other.mTypeMetainfo ) )
	{
	}

	CustomDataItem& CustomDataItem::operator=( CustomDataItem const& other )
	{
		if ( this == &other )
			return *this;

		mDataFields = other.mDataFields;
		mTypeMetainfo = other.mTypeMetainfo;

		return *this;
	}

	CustomDataItem& CustomDataItem::operator=( CustomDataItem && other )
	{
		if ( this == &other )
			return *this;

		mDataFields = std::move( other.mDataFields );
		mTypeMetainfo = std::move( other.mTypeMetainfo );

		return *this;
	}

	std::string CustomDataItem::getName() const
	{
		return mTypeMetainfo->getName();
	}

	void CustomDataItem::addField( DataField field )
	{
		// TODO: check if field exists
		mDataFields.push_back( std::move( field ) );
	}

	void CustomDataItem::set( std::string const& fieldName, DataItem value )
	{
		for ( auto &it : mDataFields )
		{
			if ( it.getName() == fieldName )
			{
				// TODO: multi-visitation
				std::string n0 = boost::apply_visitor( HumanReadableNameVisitor(), value );
				std::string n1 = boost::apply_visitor( HumanReadableNameVisitor(), it.getValue() );

				if ( n0 == n1 )
				{
					it.setValue( std::move( value ) );
					return;
				}
				else
					throw TypeMismatch( fieldName );
			}
		}

		throw NotFound( fieldName );
	}
}
