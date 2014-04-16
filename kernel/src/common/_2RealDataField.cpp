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

#include "common/_2RealDataField.h"
#include "common/_2RealCustomDataItem.h"

namespace _2Real
{
	DataField::DataField( const std::string fieldName, DataItem value ) :
		mName( std::move( fieldName ) ),
		mValue( std::move( value ) )
	{ 
	}

	DataField::DataField( DataField const& other ) :
		mName( other.mName ),
		mValue( other.mValue )
	{ 
	}

	DataField::DataField( DataField && other ) :
		mName( std::move( other.mName ) ),
		mValue( std::move( other.mValue ) )
	{
	}

	DataField& DataField::operator=( DataField const& other )
	{
		if ( this == &other )
			return *this;

		mName = other.mName;
		mValue = other.mValue;

		return *this;
	}

	DataField& DataField::operator=( DataField && other )
	{
		if ( this == &other )
			return *this;

		mName = std::move( other.mName );
		mValue = std::move( other.mValue );

		return *this;
	}

	std::string DataField::getName() const
	{
		return mName;
	}

	DataItem const& DataField::getValue() const
	{
		return mValue;
	}

	DataItem & DataField::getValue()
	{
		return mValue;
	}

	void DataField::setValue( DataItem value )
	{
		mValue = std::move( value );
	}
}
