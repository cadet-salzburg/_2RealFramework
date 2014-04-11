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
#include "engine/_2RealInSlot.h"
#include "helpers/_2RealEvent_T.h"
#include "engine/_2RealDataSink.h"

namespace _2Real
{
	class IoSlotMetainfo;
	class Link;
	class DataSource;
	class TMetainfo;
	class InstanceId;
	class MultiInlet;

	class Inlet : private InSlot, public AbstractInlet, public DataSink, public std::enable_shared_from_this< Inlet >
	{

	public:

		static std::shared_ptr< Inlet > createFromMetainfo( std::shared_ptr< MultiInlet >, std::shared_ptr< const IoSlotMetainfo > );
		static std::shared_ptr< AbstractInlet > createFromMetainfo( std::shared_ptr< Block >, std::shared_ptr< const IoSlotMetainfo > );
		static std::shared_ptr< AbstractInlet > createFromMetainfo( std::shared_ptr< Block >, std::shared_ptr< const IoSlotMetainfo >, const uint64_t );

		Inlet() = delete;
		Inlet( Inlet const& other ) = delete;
		Inlet( Inlet && other ) = delete;
		Inlet& operator=( Inlet const& other ) = delete;
		Inlet& operator=( Inlet && other ) = delete;

		~Inlet() = default;

		void update();

		void receiveData( std::shared_ptr< const DataItem > );
		void setData( DataItem );

		std::shared_ptr< Link > linkTo( std::shared_ptr< DataSource > );

		using InSlot::getValue;

		template< typename TCallable >
		void registerToValueUpdated( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< Inlet > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< Inlet > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< Inlet > >( callable, callback ) );
			mUpdateEvent.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromValueUpdated( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< Inlet > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< Inlet > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< Inlet > >( callable, callback ) );
			mUpdateEvent.removeListener( listener );
		}

		std::shared_ptr< MultiInlet > getParentInlet();

	private:

		Inlet( std::shared_ptr< Block >, std::shared_ptr< const IoSlotMetainfo >, std::shared_ptr< const InstanceId > );
		Inlet( std::shared_ptr< Block >, std::shared_ptr< MultiInlet >, std::shared_ptr< const IoSlotMetainfo >, std::shared_ptr< const InstanceId > );

		std::weak_ptr< MultiInlet >						mParentInlet;
		std::shared_ptr< Link >							mLink;
		Event_T< std::shared_ptr< Inlet > >				mUpdateEvent;

	};
}