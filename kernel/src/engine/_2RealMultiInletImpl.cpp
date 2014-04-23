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

#include "engine/_2RealMultiInletImpl.h"
#include "engine/_2RealInletImpl.h"
#include "engine/_2RealBlockImpl.h"
#include "engine/_2RealIoSlotMetainfoImpl.h"
#include "engine/_2RealId.h"

namespace _2Real
{
	class SubinletByAddress
	{
	public:
		explicit SubinletByAddress( std::shared_ptr< const InletImpl > inlet ) : mBaseline( inlet ) { assert( inlet.get() ); }
		bool operator()( std::shared_ptr< const InletImpl > val )
		{
			assert( val.get() );
			return mBaseline.get() == val.get();
		}
	private:
		std::shared_ptr< const InletImpl > mBaseline;
	};

	std::shared_ptr< InletImpl_I > MultiInletImpl::createFromMetainfo( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const IoSlotMetainfoImpl > meta )
	{
		std::shared_ptr< const InstanceId > inletId = InstanceId::create( meta->getId(), parent->getId(), InstanceType::IOSLOT, meta->getId()->getName() );
		std::shared_ptr< MultiInletImpl > inlet( new MultiInletImpl( parent, meta, inletId ) );

		return inlet;
	}

	MultiInletImpl::MultiInletImpl( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const IoSlotMetainfoImpl > meta, std::shared_ptr< const InstanceId > id ) :
		InletImpl_I(),
		std::enable_shared_from_this< MultiInletImpl >(),
		mParent( parent ),
		mMetainfo( meta ),
		mId( id )
	{
	}

	bool MultiInletImpl::isMultiInlet() const
	{
		return mMetainfo.lock()->isMulti();
	}

	std::shared_ptr< const InstanceId > MultiInletImpl::getId() const
	{
		return mId;
	}

	std::shared_ptr< BlockImpl > MultiInletImpl::getParent()
	{
		return mParent.lock();
	}

	void MultiInletImpl::update()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		for ( auto it : mInlets )
		{
			it->update();
		}
	}

	std::vector< std::shared_ptr< InletImpl > >::size_type MultiInletImpl::getSize() const
	{
		std::lock_guard< std::mutex > lock( mMutex );
		return mInlets.size();
	}

	bool MultiInletImpl::isEmpty() const
	{
		std::lock_guard< std::mutex > lock( mMutex );
		return mInlets.empty();
	}

	std::shared_ptr< InletImpl > MultiInletImpl::operator[]( const uint32_t idx )
	{
		std::lock_guard< std::mutex > lock( mMutex );
		return mInlets[ idx ];
	}

	std::shared_ptr< const InletImpl > MultiInletImpl::operator[]( const uint32_t idx ) const
	{
		std::lock_guard< std::mutex > lock( mMutex );
		return mInlets[ idx ];
	}

	std::shared_ptr< InletImpl > MultiInletImpl::add_front()
	{
		std::lock_guard< std::mutex > lock( mMutex );
		std::shared_ptr< InletImpl > inlet = InletImpl::createFromMetainfo( shared_from_this(), mMetainfo.lock() );
		mInlets.push_front( inlet );
		mSubinletAdded( inlet );
		return mInlets.front();
	}

	std::shared_ptr< InletImpl > MultiInletImpl::add_back()
	{
		std::lock_guard< std::mutex > lock( mMutex );
		std::shared_ptr< InletImpl > inlet = InletImpl::createFromMetainfo( shared_from_this(), mMetainfo.lock() );
		mInlets.push_back( inlet );
		mSubinletAdded( inlet );
		return mInlets.back();
	}

	void MultiInletImpl::remove( std::shared_ptr< InletImpl > inlet )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		auto it = std::find_if( mInlets.begin(), mInlets.end(), SubinletByAddress( inlet ) );
		if ( it == mInlets.end() )
			throw NotFound( inlet->getId()->getName() );

		mInlets.erase( it );
		mSubinletRemoved( inlet );
	}

	boost::signals2::connection MultiInletImpl::registerToSubinletAdded( boost::signals2::signal< void( std::shared_ptr< const InletImpl > ) >::slot_type listener ) const
	{
		return mSubinletAdded.connect( listener );
	}

	boost::signals2::connection MultiInletImpl::registerToSubinletRemoved( boost::signals2::signal< void( std::shared_ptr< const InletImpl > ) >::slot_type listener ) const
	{
		return mSubinletRemoved.connect( listener );
	}

}
