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
#include "engine/_2RealIoSlot.h"
#include "engine/_2RealData.h"
#include "helpers/_2RealEvent_T.h"

#include <mutex>

namespace _2Real
{
	/*
	*	base for parameters & ( normal ) inlets
	*	implements listeners & values
	*/
	class InSlot : public IoSlot
	{

	public:

		InSlot();
		virtual ~InSlot() {}

		void setTmpValue( std::shared_ptr< DataItem > );
		std::shared_ptr< const DataItem > getTmpValue() const;
		DataItem & getValue();
		DataItem const& getValue() const;

		template< typename TCallable >
		void registerToValueUpdated( TCallable *callable, void ( TCallable::*callback )( const unsigned int ) )
		{
			std::shared_ptr< AbstractCallback_T< const unsigned int > > listener( new MemberCallback_T< TCallable, const unsigned int >( callable, callback ) );
			mValueUpdatedEvent.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromValueUpdated( TCallable *callable, void ( TCallable::*callback )( const unsigned int ) )
		{
			std::shared_ptr< AbstractCallback_T< const unsigned int > > listener( new MemberCallback_T< TCallable, const unsigned int >( callable, callback ) );
			mValueUpdatedEvent.removeListener( listener );
		}

		void update();

	private:

		mutable std::mutex					mMutex;
		std::shared_ptr< DataItem >			mTmpValue;
		std::shared_ptr< DataItem >			mValue;

		Event_T< const unsigned int >		mValueUpdatedEvent;

	};
}