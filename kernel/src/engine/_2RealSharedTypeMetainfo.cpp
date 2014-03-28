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

#include "engine/_2RealSharedTypeMetainfo.h"

namespace _2Real
{

	BasicTypeMetainfo::BasicTypeMetainfo( DataItem val ) :
		mTemplate( val ),
		mTypeDescriptor()
	{
	}

	DataItem BasicTypeMetainfo::makeData() const
	{
		return mTemplate;
	}

	bool BasicTypeMetainfo::isBasicType() const
	{
		return true;
	}

	////////////////////////////////////

	SharedTypeMetainfo::SharedTypeMetainfo( const std::string name ) :
		mName( name ),
		mDescription( "no description available" ),
		mTemplate(),
		mTypeDescriptor()
	{
	}

	bool SharedTypeMetainfo::isBasicType() const
	{
		return false;
	}

	std::string SharedTypeMetainfo::getName() const
	{
		return mName;
	}

	std::string SharedTypeMetainfo::getDescription() const
	{
		return mDescription;
	}

	DataItem SharedTypeMetainfo::makeData() const
	{
		return mTemplate;
	}

	CustomDataItem SharedTypeMetainfo::makeCustomData() const
	{
		return CustomDataItem( mTemplate );
	}

	void SharedTypeMetainfo::setDescription( const std::string description )
	{
		mDescription = description;
	}

	void SharedTypeMetainfo::addField( std::string fieldName, DataItem value )
	{
		mTemplate.addField( DataField( std::move( fieldName ), std::move( value ) ) );
		// TODO: update descr.
	}

	void SharedTypeMetainfo::cloneFrom( SharedTypeMetainfo const& other )
	{
		if ( this == &other )
			return;

		mDescription = other.mDescription;
		mTemplate = other.mTemplate;
		mTypeDescriptor->cloneFrom( *other.mTypeDescriptor.get() );
	}

}