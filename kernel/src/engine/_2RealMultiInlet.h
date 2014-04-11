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

#include "helpers/_2RealStdIncludes.h"
#include "engine/_2RealAbstractInlet.h"
#include "helpers/_2RealEvent_T.h"

namespace _2Real
{
	class Inlet;
	class IoSlotMetainfo;
	class InstanceId;

	class MultiInlet : public AbstractInlet, public std::enable_shared_from_this< MultiInlet >
	{

	public:

		static std::shared_ptr< AbstractInlet > createFromMetainfo( std::shared_ptr< Block >, std::shared_ptr< const IoSlotMetainfo > );

		MultiInlet() = delete;
		MultiInlet( MultiInlet const& other ) = delete;
		MultiInlet( MultiInlet && other ) = delete;
		MultiInlet& operator=( MultiInlet const& other ) = delete;
		MultiInlet& operator=( MultiInlet && other ) = delete;

		~MultiInlet() = default;

		std::deque< std::shared_ptr< Inlet > >::size_type getSize() const;
		bool isEmpty() const;
		std::shared_ptr< Inlet > operator[]( const uint32_t );
		std::shared_ptr< Inlet > add_front();
		std::shared_ptr< Inlet > add_back();

		void remove( std::shared_ptr< Inlet > );

		void update();

		template< typename TCallable >
		void registerToSubinletAdded( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< Inlet > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< Inlet > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< Inlet > >( callable, callback ) );
			mInletAdded.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromSubinletAdded( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< Inlet > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< Inlet > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< Inlet > >( callable, callback ) );
			mInletAdded.removeListener( listener );
		}

		template< typename TCallable >
		void registerToSubinletRemoved( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< Inlet > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< Inlet > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< Inlet > >( callable, callback ) );
			mInletRemoved.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromSubinletRemoved( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< Inlet > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< Inlet > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< Inlet > >( callable, callback ) );
			mInletRemoved.removeListener( listener );
		}

	private:

		MultiInlet( std::shared_ptr< Block >, std::shared_ptr< const IoSlotMetainfo >, std::shared_ptr< const InstanceId > );

		mutable std::mutex									mMutex;
		std::deque< std::shared_ptr< Inlet > >				mInlets;

		Event_T< std::shared_ptr< Inlet > >					mInletAdded;
		Event_T< std::shared_ptr< Inlet > >					mInletRemoved;

	};

}