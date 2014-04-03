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
#include "engine/_2RealSharedServiceIoSlotMetainfo.h"

namespace _2Real
{

	MultiInlet::MultiInlet( std::shared_ptr< const SharedServiceIoSlotMetainfo > meta ) :
		AbstractInlet( meta ), std::enable_shared_from_this< MultiInlet >(), mInfo( nullptr )
	{
		//mInfo = clone( mMetainfo, meta.)
		//mInfo->setMultiInlet( false );
	}

	void MultiInlet::update()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		for ( auto it : mInlets )
		{
			it->update();
		}
	}

	uint32_t MultiInlet::getSize() const
	{
		return mInlets.size();
	}

	bool MultiInlet::isEmpty() const
	{
		return mInlets.empty();
	}

	std::shared_ptr< Inlet > MultiInlet::operator[]( const uint32_t index )
	{
		return mInlets[ index ];
	}

	std::shared_ptr< Inlet > MultiInlet::add_front()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		mInlets.push_front( std::shared_ptr< Inlet >( new Inlet( mInfo ) ) );
		return mInlets.front();
	}

	std::shared_ptr< Inlet > MultiInlet::add_back()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		mInlets.push_back( std::shared_ptr< Inlet >( new Inlet( mInfo ) ) );
		return mInlets.back();
	}

}
