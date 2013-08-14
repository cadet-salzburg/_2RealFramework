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

#include "engine/_2RealTimer.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealLogger.h"

namespace _2Real
{
	Timer::Timer( EngineImpl *engine, const unsigned long resolution, const bool verbose ) :
		mCallback( new Poco::TimerCallback< Timer >( *this, &Timer::receiveTimerSignal ) ),
		mTimer( 0, resolution ),
		mEngineImpl( engine ),
		mUpdateCount( 0 ),
		mSkippedCount( 0 ),
		mDebugTime(),
		mLog( verbose )
	{
	}

	Timer::~Timer()
	{
		mTimer.stop();
		delete mCallback;
	}

	void Timer::start()
	{
		mTimer.start( *mCallback, Poco::Thread::PRIO_NORMAL );
	}

	void Timer::stop()
	{
		mTimer.stop();
	}

	void Timer::receiveTimerSignal( Poco::Timer &t )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		long elapsed = ( long )mTimestamp.elapsed();
		mTimerSignal.notify( elapsed );
		mTimestamp.update();

		if ( mLog )
		{
			mUpdateCount++;
			mSkippedCount += t.skipped();
			//if ( ( mUpdateCount + mSkippedCount ) >= 20000 )
			//{
				std::ostringstream msg;
				msg << std::endl;
				msg << "-------------------------------------------------------------------------\n";
				msg << "TIMER: interval " << t.getPeriodicInterval() << std::endl;
				msg << "TIMER: updated " << mUpdateCount << " times" << std::endl;
				msg << "TIMER: skipped " << mSkippedCount << " times" << std::endl;
				msg << "TIMER: elapsed: " << mDebugTime.elapsed() << std::endl;
				msg << "-------------------------------------------------------------------------";
				mEngineImpl->getLogger()->addLine( msg.str() );

				mUpdateCount = 0;
				mSkippedCount = 0;
				mDebugTime.update();
			//}
		}
	}

	void Timer::registerToTimerSignal( AbstractCallback< long > *callback )
	{
		mTimerSignal.addListener( *callback );
	}

	void Timer::unregisterFromTimerSignal( AbstractCallback< long > *callback )
	{
		mTimerSignal.removeListener( *callback );
	}
}
