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

#pragma once

#include "common/_2RealStdIncludes.h"
#include "engine/_2RealInletImpl_I.h"
#include "common/_2RealEvent_T.h"

namespace _2Real
{
	class InletImpl;
	class IoSlotMetainfoImpl;
	class InstanceId;

	class MultiInletImpl : public InletImpl_I, public std::enable_shared_from_this< MultiInletImpl >
	{

	public:

		static std::shared_ptr< InletImpl_I > createFromMetainfo( std::shared_ptr< BlockImpl >, std::shared_ptr< const IoSlotMetainfoImpl > );

		MultiInletImpl() = delete;
		MultiInletImpl( MultiInletImpl const& other ) = delete;
		MultiInletImpl( MultiInletImpl && other ) = delete;
		MultiInletImpl& operator=( MultiInletImpl const& other ) = delete;
		MultiInletImpl& operator=( MultiInletImpl && other ) = delete;

		~MultiInletImpl() = default;

		std::deque< std::shared_ptr< InletImpl > >::size_type getSize() const;
		bool isEmpty() const;
		std::shared_ptr< InletImpl > operator[]( const uint32_t );
		std::shared_ptr< InletImpl > add_front();
		std::shared_ptr< InletImpl > add_back();

		void remove( std::shared_ptr< InletImpl > );

		bool									isMultiInlet() const;
		std::shared_ptr< const InstanceId >		getId() const;
		std::shared_ptr< BlockImpl >			getParent();
		void									update();

		template< typename TCallable >
		void registerToSubinletAdded( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< InletImpl > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< InletImpl > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< InletImpl > >( callable, callback ) );
			mInletAdded.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromSubinletAdded( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< InletImpl > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< InletImpl > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< InletImpl > >( callable, callback ) );
			mInletAdded.removeListener( listener );
		}

		template< typename TCallable >
		void registerToSubinletRemoved( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< InletImpl > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< InletImpl > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< InletImpl > >( callable, callback ) );
			mInletRemoved.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromSubinletRemoved( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< InletImpl > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< InletImpl > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< InletImpl > >( callable, callback ) );
			mInletRemoved.removeListener( listener );
		}

	private:

		MultiInletImpl( std::shared_ptr< BlockImpl >, std::shared_ptr< const IoSlotMetainfoImpl >, std::shared_ptr< const InstanceId > );

		std::weak_ptr< BlockImpl >						mParent;
		std::weak_ptr< const IoSlotMetainfoImpl >		mMetainfo;
		std::shared_ptr< const InstanceId >			mId;

		mutable std::mutex									mMutex;
		std::deque< std::shared_ptr< InletImpl > >				mInlets;

		Event_T< std::shared_ptr< InletImpl > >					mInletAdded;
		Event_T< std::shared_ptr< InletImpl > >					mInletRemoved;

	};

}