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

#include "engine/_2RealSharedServiceIoSlotMetainfo.h"
#include "engine/_2RealSharedTypeMetainfo.h"
#include "engine/_2RealTypeCollection.h"
#include "engine/_2RealTypeMetainfoVisitor.h"
#include "engine/_2RealHumanReadableNameVisitor.h"
#include "engine/_2RealId.h"

namespace _2Real
{

	std::shared_ptr< SharedServiceIoSlotMetainfo > SharedServiceIoSlotMetainfo::make( std::shared_ptr< const MetainfoId > id, std::shared_ptr< const TypeCollection > types, std::string const& name )
	{
		std::shared_ptr< const MetainfoId > ioslotId = MetainfoId::create( id, MetainfoType::IOSLOT, name );
		std::shared_ptr< SharedServiceIoSlotMetainfo > ioslotInfo( new SharedServiceIoSlotMetainfo( ioslotId, types ) );

		return ioslotInfo;
	}

	SharedServiceIoSlotMetainfo::SharedServiceIoSlotMetainfo( std::shared_ptr< const MetainfoId > id, std::shared_ptr< const TypeCollection > types ) :
		mId( id ),
		mDescription( "" ),
		mInitialValue(),
		mIsMulti( false ),
		mTypes( types )
	{
		
	}

	void SharedServiceIoSlotMetainfo::setMulti( const bool isMulti )
	{
		mIsMulti = isMulti;
	}

	void SharedServiceIoSlotMetainfo::setDescription( std::string const& description )
	{
		mDescription = description;
	}

	void SharedServiceIoSlotMetainfo::setDatatypeAndInitialValue( DataItem const& initialValue )
	{
		mInitialValue = initialValue;
	}

	std::string SharedServiceIoSlotMetainfo::getName() const
	{
		return mId->getName();
	}

	std::string SharedServiceIoSlotMetainfo::getDescription() const
	{
		return mDescription;
	}

	DataItem const& SharedServiceIoSlotMetainfo::getInitialValue() const
	{
		return mInitialValue;
	}

	bool SharedServiceIoSlotMetainfo::isMulti() const
	{
		return mIsMulti;
	}

	std::string SharedServiceIoSlotMetainfo::getDatatype() const
	{
		return boost::apply_visitor( HumanReadableNameVisitor(), mInitialValue );
	}

	std::shared_ptr< const TMetainfo > SharedServiceIoSlotMetainfo::getTypeMetainfo() const
	{
		return boost::apply_visitor( TypeMetainfoVisitor( mTypes.lock() ), mInitialValue );
	}

}