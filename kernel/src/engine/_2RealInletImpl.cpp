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

#pragma warning( disable : 4996 )
#pragma warning( disable : 4702 )

#include "engine/_2RealInletImpl.h"
#include "engine/_2RealIoSlotMetainfoImpl.h"
#include "engine/_2RealBlockImpl.h"
#include "engine/_2RealLink.h"
#include "engine/_2RealId.h"
#include "engine/_2RealMultiInletImpl.h"

namespace _2Real
{
	std::shared_ptr< InletImpl > InletImpl::createFromMetainfo( std::shared_ptr< MultiInletImpl > parentInlet, std::shared_ptr< const IoSlotMetainfoImpl > meta )
	{
		std::shared_ptr< BlockImpl > block = parentInlet->getParent();
		std::shared_ptr< const InstanceId >	inletId =  InstanceId::create( meta->getId(), block->getId(), InstanceType::IOSLOT, meta->getId()->getName() );
		std::shared_ptr< InletImpl > inlet( new InletImpl( block, parentInlet, meta, inletId ) );

		std::shared_ptr< DataItem > initValue( new DataItem( meta->getInitialValue() ) );
		inlet->InSlot::setTmpValue( initValue );

		return inlet;
	}

	std::shared_ptr< InletImpl_I > InletImpl::createFromMetainfo( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const IoSlotMetainfoImpl > meta )
	{
		std::shared_ptr< const InstanceId >	inletId =  InstanceId::create( meta->getId(), parent->getId(), InstanceType::IOSLOT, meta->getId()->getName() );
		std::shared_ptr< InletImpl > inlet( new InletImpl( parent, meta, inletId ) );

		std::shared_ptr< DataItem > initValue( new DataItem( meta->getInitialValue() ) );
		inlet->InSlot::setTmpValue( initValue );

		return inlet;
	}

	std::shared_ptr< InletImpl_I > InletImpl::createFromMetainfo( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const IoSlotMetainfoImpl > meta, const uint64_t count )
	{
		std::shared_ptr< const InstanceId >	inletId = InstanceId::create( meta->getId(), parent->getId(), InstanceType::IOSLOT, meta->getId()->getName() + std::to_string( count ) );
		std::shared_ptr< InletImpl > inlet( new InletImpl( parent, meta, inletId ) );

		std::shared_ptr< DataItem > initValue( new DataItem( meta->getInitialValue() ) );
		inlet->InSlot::setTmpValue( initValue );

		return inlet;
	}

	InletImpl::InletImpl( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const IoSlotMetainfoImpl > meta, std::shared_ptr< const InstanceId > id ) :
		InSlot(),
		InletImpl_I(),
		DataSink_I(),
		std::enable_shared_from_this< InletImpl >(),
		mParent( parent ),
		mMetainfo( meta ),
		mId( id )
	{
	}

	InletImpl::InletImpl( std::shared_ptr< BlockImpl > parent, std::shared_ptr< MultiInletImpl > parentInlet, std::shared_ptr< const IoSlotMetainfoImpl > meta, std::shared_ptr< const InstanceId > id ) :
		InSlot(),
		InletImpl_I(),
		DataSink_I(),
		std::enable_shared_from_this< InletImpl >(),
		mParent( parent ),
		mMetainfo( meta ),
		mId( id ),
		mParentInlet( parentInlet )
	{
	}

	bool InletImpl::isMultiInlet() const
	{
		return mMetainfo.lock()->isMulti();
	}

	std::shared_ptr< const InstanceId > InletImpl::getId() const
	{
		return mId;
	}

	std::shared_ptr< BlockImpl > InletImpl::getParent()
	{
		return mParent.lock();
	}

	std::shared_ptr< const MultiInletImpl > InletImpl::getParentInlet() const
	{
		return mParentInlet.lock();
	}

	void InletImpl::update()
	{
		InSlot::update();
	}

	bool InletImpl::isLinked() const
	{
		return mDataConnection.connected();
	}

	void InletImpl::unlink()
	{
		mDataConnection.disconnect();
		mRemoved( std::static_pointer_cast< DataSink_I >( shared_from_this() ) );
	}

	void InletImpl::receiveData( std::shared_ptr< const DataItem > data )
	{
		InSlot::setTmpValue( data );
		mValueUpdated( shared_from_this() );
	}

	void InletImpl::setData( DataItem data )
	{
		InSlot::setTmpValueExt( data );
		mValueUpdated( shared_from_this() );
	}

	void InletImpl::linkTo( std::shared_ptr< DataSource_I > source )
	{
		mDataConnection = source->registerToUpdate( std::bind( &InletImpl::receiveData, this, std::placeholders::_1 ) );
		//source->registerToRemoved();
	}

	boost::signals2::connection InletImpl::registerToValueUpdated( boost::signals2::signal< void( std::shared_ptr< const InletImpl > ) >::slot_type listener ) const
	{
		return mValueUpdated.connect( listener );
	}

	boost::signals2::connection InletImpl::registerToRemoved( boost::signals2::signal< void( std::shared_ptr< const DataSink_I > ) >::slot_type listener ) const
	{
		return mRemoved.connect( listener );
	}
}
