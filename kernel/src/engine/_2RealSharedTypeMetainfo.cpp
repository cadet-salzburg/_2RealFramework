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
#include "engine/_2RealId.h"
#include "engine/_2RealHumanReadableNameVisitor.h"
#include "engine/_2RealTypeMetainfoVisitor.h"
#include "engine/_2RealTypeCollection.h"

namespace _2Real
{
	////////////////////////////////////

	std::shared_ptr< BasicTypeMetainfo > BasicTypeMetainfo::make( std::shared_ptr< TypeCollection > types, const DataItem item )
	{
		std::string name = boost::apply_visitor( HumanReadableNameVisitor(), item );
		std::shared_ptr< const MetainfoId > basictypeId = MetainfoId::create( nullptr, MetainfoType::TYPE, name );
		std::shared_ptr< BasicTypeMetainfo > basictypeInfo( new BasicTypeMetainfo( basictypeId, types, item ) );
		return basictypeInfo;
	}

	BasicTypeMetainfo::BasicTypeMetainfo( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types, DataItem val ) :
		mId( id ),
		mTypes( types ),
		mTemplate( val )
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

	std::shared_ptr< const MetainfoId > BasicTypeMetainfo::getId() const
	{
		return mId;
	}

	std::string BasicTypeMetainfo::getName() const
	{
		return mId->getName();
	}

	////////////////////////////////////

	std::shared_ptr< SharedTypeMetainfo > SharedTypeMetainfo::make( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types, std::string const& name )
	{
		std::shared_ptr< const MetainfoId > sharedtypeId = MetainfoId::create( id, MetainfoType::TYPE, name );
		std::shared_ptr< SharedTypeMetainfo > sharedtypeInfo( new SharedTypeMetainfo( sharedtypeId, types ) );
		sharedtypeInfo->mTemplate = CustomDataItem( sharedtypeId );
		return sharedtypeInfo;
	}

	SharedTypeMetainfo::SharedTypeMetainfo( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types ) :
		TMetainfo(),
		mId( id ),
		mDescription( "no description available" ),
		mTemplate(),
		mTypes( types )
	{
	}

	void SharedTypeMetainfo::unregisterFromTypeCollection()
	{
		std::shared_ptr< TypeCollection > types = mTypes.lock();
		if ( types.get() != nullptr ) types->typeRemoved( mId->toString() );
	}

	bool SharedTypeMetainfo::isBasicType() const
	{
		return false;
	}

	std::string SharedTypeMetainfo::getName() const
	{
		return mId->getName();
	}

	std::string SharedTypeMetainfo::getDescription() const
	{
		return mDescription;
	}

	std::shared_ptr< const MetainfoId > SharedTypeMetainfo::getId() const
	{
		return mId;
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
	}

	std::vector< std::pair< std::string, std::shared_ptr< const TMetainfo > > > SharedTypeMetainfo::getDataFields() const
	{
		std::vector< std::pair< std::string, std::shared_ptr< const TMetainfo > > > result;
			
		for ( auto const& it : mTemplate.mDataFields )
			result.push_back( std::make_pair( it.getName(), boost::apply_visitor( TypeMetainfoVisitor( mTypes.lock() ), it.getValue() ) ) );

		return result;
	}

}