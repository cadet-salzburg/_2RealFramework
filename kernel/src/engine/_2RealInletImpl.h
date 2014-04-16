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
#include "engine/_2RealInSlot.h"
#include "common/_2RealEvent_T.h"
#include "engine/_2RealDataSink_I.h"

namespace _2Real
{
	class IoSlotMetainfoImpl;
	class Link;
	class DataSource_I;
	class TypeMetainfoImpl_I;
	class InstanceId;
	class MultiInletImpl;

	class InletImpl : private InSlot, public InletImpl_I, public DataSink_I, public std::enable_shared_from_this< InletImpl >
	{

	public:

		static std::shared_ptr< InletImpl > createFromMetainfo( std::shared_ptr< MultiInletImpl >, std::shared_ptr< const IoSlotMetainfoImpl > );
		static std::shared_ptr< InletImpl_I > createFromMetainfo( std::shared_ptr< BlockImpl >, std::shared_ptr< const IoSlotMetainfoImpl > );
		static std::shared_ptr< InletImpl_I > createFromMetainfo( std::shared_ptr< BlockImpl >, std::shared_ptr< const IoSlotMetainfoImpl >, const uint64_t );

		InletImpl() = delete;
		InletImpl( InletImpl const& other ) = delete;
		InletImpl( InletImpl && other ) = delete;
		InletImpl& operator=( InletImpl const& other ) = delete;
		InletImpl& operator=( InletImpl && other ) = delete;

		~InletImpl() = default;

		bool									isMultiInlet() const;
		std::shared_ptr< const InstanceId >		getId() const;
		std::shared_ptr< BlockImpl >			getParent();
		void									update();

		void receiveData( std::shared_ptr< const DataItem > );
		void setData( DataItem );

		std::shared_ptr< Link > linkTo( std::shared_ptr< DataSource_I > );

		using InSlot::getValue;

		template< typename TCallable >
		void registerToValueUpdated( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< InletImpl > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< InletImpl > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< InletImpl > >( callable, callback ) );
			mUpdateEvent.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromValueUpdated( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< InletImpl > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< InletImpl > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< InletImpl > >( callable, callback ) );
			mUpdateEvent.removeListener( listener );
		}

		std::shared_ptr< MultiInletImpl > getParentInlet();

	private:

		InletImpl( std::shared_ptr< BlockImpl >, std::shared_ptr< const IoSlotMetainfoImpl >, std::shared_ptr< const InstanceId > );
		InletImpl( std::shared_ptr< BlockImpl >, std::shared_ptr< MultiInletImpl >, std::shared_ptr< const IoSlotMetainfoImpl >, std::shared_ptr< const InstanceId > );

		std::weak_ptr< BlockImpl >						mParent;
		std::weak_ptr< const IoSlotMetainfoImpl >		mMetainfo;
		std::shared_ptr< const InstanceId >			mId;

		std::weak_ptr< MultiInletImpl >						mParentInlet;
		std::shared_ptr< Link >							mLink;
		Event_T< std::shared_ptr< InletImpl > >				mUpdateEvent;

	};
}