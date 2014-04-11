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

#include "engine/_2RealIoSlotMetainfo.h"
#include "engine/_2RealSharedTypeMetainfo.h"
#include "engine/_2RealTypeCollection.h"
#include "engine/_2RealTypeMetainfoVisitor.h"
#include "engine/_2RealHumanReadableNameVisitor.h"
#include "engine/_2RealId.h"

namespace _2Real
{

	std::shared_ptr< IoSlotMetainfo > IoSlotMetainfo::make( std::shared_ptr< const MetainfoId > id, std::shared_ptr< const TypeCollection > types, const std::string name )
	{
		std::shared_ptr< const MetainfoId > ioslotId = MetainfoId::create( id, MetainfoType::IOSLOT, name );
		std::shared_ptr< IoSlotMetainfo > ioslotInfo( new IoSlotMetainfo( ioslotId, types ) );

		return ioslotInfo;
	}

	IoSlotMetainfo::IoSlotMetainfo( std::shared_ptr< const MetainfoId > id, std::shared_ptr< const TypeCollection > types ) :
		mId( id ),
		mDescription( "" ),
		mInitialValue(),
		mIsMulti( false ),
		mTypes( types )
	{
		
	}

	std::shared_ptr< const MetainfoId > IoSlotMetainfo::getId() const
	{
		return mId;
	}

	std::string IoSlotMetainfo::getName() const
	{
		return mId->getName();
	}

	void IoSlotMetainfo::setMulti( const bool isMulti )
	{
		mIsMulti = isMulti;
	}

	void IoSlotMetainfo::setDescription( std::string const& description )
	{
		mDescription = description;
	}

	void IoSlotMetainfo::setDatatypeAndInitialValue( DataItem const& initialValue )
	{
		mInitialValue = initialValue;
	}

	std::string IoSlotMetainfo::getDescription() const
	{
		return mDescription;
	}

	DataItem const& IoSlotMetainfo::getInitialValue() const
	{
		return mInitialValue;
	}

	bool IoSlotMetainfo::isMulti() const
	{
		return mIsMulti;
	}

	std::string IoSlotMetainfo::getDatatype() const
	{
		return boost::apply_visitor( HumanReadableNameVisitor(), mInitialValue );
	}

	std::shared_ptr< const TMetainfo > IoSlotMetainfo::getTypeMetainfo() const
	{
		return boost::apply_visitor( TypeMetainfoVisitor( mTypes.lock() ), mInitialValue );
	}

}