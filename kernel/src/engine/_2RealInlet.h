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
	class SharedServiceIoSlotMetainfo;
	class Link;
	class DataSource;
	class TMetainfo;
	class InstanceId;

	class Inlet : private InSlot, public AbstractInlet, public DataSink, public std::enable_shared_from_this< Inlet >
	{

	public:

		explicit Inlet( std::shared_ptr< const SharedServiceIoSlotMetainfo > );

		// just for the sake of readability; these are public anyway
		using AbstractInlet::getName;
		using AbstractInlet::isMultiInlet;

		// virtual method of AbstractInlet
		void update();

		// virtual method of DataSink
		void receiveData( std::shared_ptr< const DataItem > );

		// exposed methods of InSlot
		using InSlot::getValue;

		// linking, yay
		std::shared_ptr< Link > linkTo( std::shared_ptr< DataSource > );

		std::shared_ptr< const TMetainfo > getTypeMetainfo() const;

		template< typename TCallable >
		void registerToValueUpdated( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< const InstanceId > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< const InstanceId > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const InstanceId > >( callable, callback ) );
			mValueUpdatedEvent.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromValueUpdated( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< const InstanceId > ) )
		{
			std::shared_ptr< AbstractCallback_T<  std::shared_ptr< const InstanceId > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const InstanceId > >( callable, callback ) );
			mValueUpdatedEvent.removeListener( listener );
		}

	private:

		std::shared_ptr< const InstanceId >				mId;
		std::shared_ptr< Link >							mLink;
		Event_T< std::shared_ptr< const InstanceId > >	mValueUpdatedEvent;

	};
}