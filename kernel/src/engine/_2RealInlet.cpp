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

#include "engine/_2RealInlet.h"
#include "engine/_2RealIoSlotMetainfo.h"
#include "engine/_2RealBlock.h"
#include "engine/_2RealLink.h"
#include "engine/_2RealId.h"
#include "engine/_2RealMultiInlet.h"

namespace _2Real
{
	std::shared_ptr< Inlet > Inlet::createFromMetainfo( std::shared_ptr< MultiInlet > parentInlet, std::shared_ptr< const IoSlotMetainfo > meta )
	{
		std::shared_ptr< Block > block = parentInlet->getParent();
		std::shared_ptr< const InstanceId >	inletId =  InstanceId::create( meta->getId(), block->getId(), InstanceType::IOSLOT, meta->getId()->getName() );
		std::shared_ptr< Inlet > inlet( new Inlet( block, parentInlet, meta, inletId ) );

		std::shared_ptr< DataItem > initValue( new DataItem( meta->getInitialValue() ) );
		inlet->InSlot::setTmpValue( initValue );

		return inlet;
	}

	std::shared_ptr< AbstractInlet > Inlet::createFromMetainfo( std::shared_ptr< Block > parent, std::shared_ptr< const IoSlotMetainfo > meta )
	{
		std::shared_ptr< const InstanceId >	inletId =  InstanceId::create( meta->getId(), parent->getId(), InstanceType::IOSLOT, meta->getId()->getName() );
		std::shared_ptr< Inlet > inlet( new Inlet( parent, meta, inletId ) );

		std::shared_ptr< DataItem > initValue( new DataItem( meta->getInitialValue() ) );
		inlet->InSlot::setTmpValue( initValue );

		return inlet;
	}

	std::shared_ptr< AbstractInlet > Inlet::createFromMetainfo( std::shared_ptr< Block > parent, std::shared_ptr< const IoSlotMetainfo > meta, const uint64_t count )
	{
		std::shared_ptr< const InstanceId >	inletId = InstanceId::create( meta->getId(), parent->getId(), InstanceType::IOSLOT, meta->getId()->getName() + std::to_string( count ) );
		std::shared_ptr< Inlet > inlet( new Inlet( parent, meta, inletId ) );

		std::shared_ptr< DataItem > initValue( new DataItem( meta->getInitialValue() ) );
		inlet->InSlot::setTmpValue( initValue );

		return inlet;
	}

	Inlet::Inlet( std::shared_ptr< Block > parent, std::shared_ptr< const IoSlotMetainfo > meta, std::shared_ptr< const InstanceId > id ) :
		InSlot(),
		AbstractInlet( parent, meta, id ),
		DataSink(),
		std::enable_shared_from_this< Inlet >()
	{
	}

	Inlet::Inlet( std::shared_ptr< Block > parent, std::shared_ptr< MultiInlet > parentInlet, std::shared_ptr< const IoSlotMetainfo > meta, std::shared_ptr< const InstanceId > id ) :
		InSlot(),
		AbstractInlet( parent, meta, id ),
		DataSink(),
		std::enable_shared_from_this< Inlet >(),
		mParentInlet( parentInlet )
	{
	}

	std::shared_ptr< MultiInlet > Inlet::getParentInlet()
	{
		return mParentInlet.lock();
	}

	void Inlet::update()
	{
		InSlot::update();
	}

	void Inlet::receiveData( std::shared_ptr< const DataItem > data )
	{
		InSlot::setTmpValue( data );
		mUpdateEvent.notify( shared_from_this() );
	}

	void Inlet::setData( DataItem data )
	{
		InSlot::setTmpValueExt( data );
		mUpdateEvent.notify( shared_from_this() );
	}

	std::shared_ptr< Link > Inlet::linkTo( std::shared_ptr< DataSource > source )
	{
		// inlet keeps tracks of it's links
		// ? might be problematic if the load - save thing comes back ?
		mLink.reset( new Link( source, std::static_pointer_cast< DataSink >( shared_from_this() ) ) );
		return mLink;
	}
}
