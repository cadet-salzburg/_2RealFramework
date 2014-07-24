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

#include "engine/_2RealOutletImpl.h"
#include "engine/_2RealBlockImpl.h"
#include "engine/_2RealIoSlotMetainfoImpl.h"
#include "engine/_2RealId.h"

namespace _2Real
{

	std::shared_ptr< OutletImpl > OutletImpl::createFromMetainfo( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const IoSlotMetainfoImpl > meta )
	{
		std::shared_ptr< const InstanceId > outletId = InstanceId::create( meta->getId(), parent->getId(), InstanceType::IOSLOT, meta->getId()->getName() );
		std::shared_ptr< OutletImpl > outlet( new OutletImpl( parent, meta, outletId ) );

		return outlet;
	}

	OutletImpl::OutletImpl( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const IoSlotMetainfoImpl > meta, std::shared_ptr< const InstanceId > id ) :
		DataSource_I(),
		std::enable_shared_from_this< OutletImpl >(),
		mParent( parent ),
		mMetainfo( meta ),
		mId( id ),
		mTmpValue( new DataItem( meta->getInitialValue() ) ),
		mValue( new DataItem( meta->getInitialValue() ) ),
		mWasDiscarded( false )
	{
	}

	void OutletImpl::discard()
	{
		mWasDiscarded = true;
	}

	std::shared_ptr< const IoSlotMetainfoImpl > OutletImpl::getMetainfo() const
	{
		return mMetainfo.lock();
	}

	std::shared_ptr< const InstanceId > OutletImpl::getId() const
	{
		return mId;
	}

	std::shared_ptr< BlockImpl > OutletImpl::getParent()
	{
		return mParent.lock();
	}

	DataItem & OutletImpl::getValue()
	{
		return *mValue.get();
	}

	std::shared_ptr< const DataItem > OutletImpl::getData() const
	{
		return mValue;
	}

	void OutletImpl::update()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		if ( mWasDiscarded )
		{
			// nothing to do
			mWasDiscarded = false;
		}
		else
		{
			mTmpValue = mValue;
			mValue.reset( new DataItem( mMetainfo.lock()->getInitialValue() ) );
			mUpdated( mTmpValue );
		}
	}

	void OutletImpl::unlink()
	{
		mRemoved( std::static_pointer_cast< DataSource_I >( shared_from_this() ) );
	}

	boost::signals2::connection OutletImpl::registerToUpdate( boost::signals2::signal< void( std::shared_ptr< const DataItem > )>::slot_type listener ) const
	{
		return mUpdated.connect( listener );
	}

	boost::signals2::connection OutletImpl::registerToRemoved( boost::signals2::signal< void( std::shared_ptr< const DataSource_I > )>::slot_type listener ) const
	{
		return mRemoved.connect( listener );
	}
}
