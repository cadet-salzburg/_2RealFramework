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
#include "engine/_2RealAbstractStateManager.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealTimer.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	class TimeBasedTrigger
	{

	public:

		TimeBasedTrigger( Timer *timer, AbstractStateManager *mgr, const long timeslice ) :
			mTimer( timer ),
			mUpdateManager( mgr ),
			mCondition( false ),
			mDesiredTime( timeslice ),
			mElapsedTime( 0 )
		{
#ifdef _DEBUG
			assert( mTimer );
			assert( mUpdateManager );
#endif
			AbstractCallback< long > *cb = new MemberCallback< TimeBasedTrigger, long >( *this, &TimeBasedTrigger::tryTriggerUpdate );
			mTimer->registerToTimerSignal( *cb );
			mUpdateManager->addTrigger( *this );
		}

		~TimeBasedTrigger()
		{
			AbstractCallback< long > *cb = new MemberCallback< TimeBasedTrigger, long >( *this, &TimeBasedTrigger::tryTriggerUpdate );
			mTimer->unregisterFromTimerSignal( *cb );
			mUpdateManager->removeTrigger( *this );
		}

		void tryTriggerUpdate( long time )
		{
			mElapsedTime += time;
			if ( !mCondition.isFulfilled() && mElapsedTime >= mDesiredTime )
			{
				mElapsedTime = 0;
				mCondition.set( true );
				mUpdateManager->tryTriggerTime();
			}
		}

		void resetTime() { mElapsedTime = 0; }
		bool isFulfilled() const { return mCondition.isFulfilled(); }
		void set( const bool fulfilled ) { mCondition.set( fulfilled ); }

	private:

		Timer						*const mTimer;
		AbstractStateManager		*const mUpdateManager;
		UpdateCondition				mCondition;
		long						mDesiredTime;
		long						mElapsedTime;

	};

}