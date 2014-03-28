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

#include "engine/_2RealData.h"
#include "helpers/_2RealException.h"

namespace _2Real
{
	std::ostream& operator<<( std::ostream& out, CustomDataItem const& val )
	{
		for ( auto const& it : val.mDataFields )
			// TODO: visitor for formatting
			out << it.getName() << " : " << it.getValue() << std::endl;
		return out;
	}

	CustomDataItem::CustomDataItem() :
		mTypename( "undefined type" ),
		mDataFields()
	{
		std::cout << "custom data item  def ctor" << std::endl;
	}

	CustomDataItem::CustomDataItem( CustomDataItem const& other ) :
		mTypename( other.mTypename ),
		mDataFields( other.mDataFields )
	{
		std::cout << "custom data item copy ctor" << std::endl;
		std::cout << mDataFields.size() << " : " << other.mDataFields.size() << std::endl;
	}

	CustomDataItem::CustomDataItem( CustomDataItem && other ) :
		mTypename( std::move( other.mTypename ) ),
		mDataFields( std::move( other.mDataFields ) )
	{
		std::cout << "custom data item move ctor" << std::endl;
		std::cout << mDataFields.size() << " : " << other.mDataFields.size() << std::endl;
	}

	CustomDataItem& CustomDataItem::operator=( CustomDataItem const& other )
	{
		if ( this == &other )
			return *this;

		mTypename = other.mTypename;
		mDataFields = other.mDataFields;
		std::cout << "custom data item copy assignment" << std::endl;
		std::cout << mDataFields.size() << " : " << other.mDataFields.size() << std::endl;

		return *this;
	}

	CustomDataItem& CustomDataItem::operator=( CustomDataItem && other )
	{
		if ( this == &other )
			return *this;

		mTypename = std::move( other.mTypename );
		mDataFields = std::move( other.mDataFields );
		std::cout << "custom data item move assignment" << std::endl;
		std::cout << mDataFields.size() << " : " << other.mDataFields.size() << std::endl;

		return *this;
	}

	std::string const& CustomDataItem::getName() const
	{
		return  mTypename;
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
				if ( boost::apply_visitor( HumanReadableNameVisitor(), value ) == boost::apply_visitor( HumanReadableNameVisitor(), it.getValue() ) )
				{
					it.setValue( std::move( value ) );
					return;
				}
				else
					throw _2Real::TypeMismatchException( fieldName );
			}
		}

		throw _2Real::NotFoundException( fieldName );
	}
}
