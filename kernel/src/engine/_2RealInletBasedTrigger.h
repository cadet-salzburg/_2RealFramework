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

#include "engine/_2RealAbstractUpdateTrigger.h"
#include "engine/_2RealTimestampedData.h"
#include "engine/_2RealAbstractStateManager.h"
#include "engine/_2RealAbstractIOManager.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	struct ValidData
	{
		bool operator()( TimestampedData const& newData, TimestampedData const& oldData )
		{
			// nullptr in timestamped data -> not valid, return false
			return ( newData.value.get() != nullptr );
		}
	};

	struct NewerTimestamp
	{
		bool operator()( TimestampedData const& newData, TimestampedData const& oldData )
		{
			return ( newData.timestamp > oldData.timestamp );
		}
	};

	class AbstractInletBasedTrigger
	{

	public:

		AbstractInletBasedTrigger( BasicInletIO *inlet, AbstractStateManager *mgr ) :
			mCondition( false ),
			mInletIO( inlet ),
			mUpdateManager( mgr ),
			mLastData()
		{
		}

		AbstractInletBasedTrigger& operator=( AbstractInletBasedTrigger const& other )
		{
			mLastData = other.mLastData;
			return *this;
		}

		bool operator==( AbstractInletBasedTrigger const& other ) const
		{
			return ( mInletIO == other.mInletIO );
		}

		virtual ~AbstractInletBasedTrigger() {}

		virtual bool tryFulfillCondition( TimestampedData const&, const bool ) = 0;
		virtual void tryTriggerUpdate() = 0;

		void resetData()
		{
			mLastData = TimestampedData();
		}

		bool isFulfilled() const { return mCondition.isFulfilled(); }
		void set( const bool fulfilled ) { mCondition.set( fulfilled ); }

	protected:

		UpdateCondition			mCondition;
		BasicInletIO			*const mInletIO;
		AbstractStateManager	*const mUpdateManager;
		TimestampedData			mLastData;

	};

	template< typename TCond >
	class InletBasedTrigger : public AbstractInletBasedTrigger
	{

	public:

		InletBasedTrigger( BasicInletIO *inlet, AbstractStateManager *mgr, const bool isOr ) :
			AbstractInletBasedTrigger( inlet, mgr ),
			mIsOr( isOr )
		{
			mUpdateManager->addTrigger( *this, mIsOr );
			mInletIO->setTrigger( this );
		}

		~InletBasedTrigger()
		{
			mUpdateManager->removeTrigger( *this, mIsOr );
			mInletIO->removeTrigger( this );

			// this has to be done, otherwise removing a basic inlet from a multiinlet might lead to a deadlock
			// if the block owning the inlet was shutdown already, nothing should happen
			mUpdateManager->tryTriggerInlet();
		}

		bool tryFulfillCondition( TimestampedData const& data, const bool reTry )
		{
			// if retry flag->ignore cond, else: check cond
			if ( ( reTry || !mCondition.isFulfilled() ) && mConditionCheck( data, mLastData ) )
			{
				mCondition.set( true );
				mLastData = data;
				return true;
			}
			else return false;
		}

		void tryTriggerUpdate()
		{
			if ( mCondition.isFulfilled() )
				mUpdateManager->tryTriggerInlet();
		}

	private:

		bool				mIsOr;
		TCond				mConditionCheck;

	};

}