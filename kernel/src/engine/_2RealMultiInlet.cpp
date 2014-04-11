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

#include "engine/_2RealMultiInlet.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealBlock.h"
#include "engine/_2RealIoSlotMetainfo.h"
#include "engine/_2RealId.h"

namespace _2Real
{

	struct PtrCmp : public std::unary_function< std::string, bool >
	{
		explicit PtrCmp( std::shared_ptr< Inlet > inlet ) : mInlet( inlet ) { assert( mInlet ); }

		bool operator()( std::shared_ptr< Inlet > other )
		{
			assert( other );
			return mInlet.get() == other.get();
		}

		std::shared_ptr< Inlet > mInlet;
	};

	std::shared_ptr< AbstractInlet > MultiInlet::createFromMetainfo( std::shared_ptr< Block > parent, std::shared_ptr< const IoSlotMetainfo > meta )
	{
		std::shared_ptr< const InstanceId > inletId = InstanceId::create( meta->getId(), parent->getId(), InstanceType::IOSLOT, meta->getId()->getName() );
		std::shared_ptr< MultiInlet > inlet( new MultiInlet( parent, meta, inletId ) );

		return inlet;
	}

	MultiInlet::MultiInlet( std::shared_ptr< Block > parent, std::shared_ptr< const IoSlotMetainfo > meta, std::shared_ptr< const InstanceId > id ) :
		AbstractInlet( parent, meta, id ),
		std::enable_shared_from_this< MultiInlet >()
	{
	}

	void MultiInlet::update()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		for ( auto it : mInlets )
		{
			it->update();
		}
	}

	std::vector< std::shared_ptr< Inlet > >::size_type MultiInlet::getSize() const
	{
		std::lock_guard< std::mutex > lock( mMutex );
		return mInlets.size();
	}

	bool MultiInlet::isEmpty() const
	{
		std::lock_guard< std::mutex > lock( mMutex );
		return mInlets.empty();
	}

	std::shared_ptr< Inlet > MultiInlet::operator[]( const uint32_t idx )
	{
		std::lock_guard< std::mutex > lock( mMutex );
		return mInlets[ idx ];
	}

	std::shared_ptr< Inlet > MultiInlet::add_front()
	{
		std::lock_guard< std::mutex > lock( mMutex );
		std::shared_ptr< Inlet > inlet = Inlet::createFromMetainfo( shared_from_this(), AbstractInlet::mMetainfo.lock() );
		mInlets.push_front( inlet );
		mInletAdded.notify( inlet );
		return mInlets.front();
	}

	std::shared_ptr< Inlet > MultiInlet::add_back()
	{
		std::lock_guard< std::mutex > lock( mMutex );
		std::shared_ptr< Inlet > inlet = Inlet::createFromMetainfo( shared_from_this(), AbstractInlet::mMetainfo.lock() );
		mInlets.push_back( inlet );
		mInletAdded.notify( inlet );
		return mInlets.back();
	}

	void MultiInlet::remove( std::shared_ptr< Inlet > inlet )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		auto it = std::find_if( mInlets.begin(), mInlets.end(), PtrCmp( inlet ) );
		if ( it == mInlets.end() )
			throw NotFound( inlet->getId()->getName() );

		mInlets.erase( it );
		mInletRemoved.notify( inlet );
	}

}
