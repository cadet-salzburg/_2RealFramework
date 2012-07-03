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
#include "helpers/_2RealSingletonHolder.h"

#include <iostream>

namespace _2Real
{
	Timer::Timer() :
		m_Callback( nullptr ),
		m_Timer( 0, 1 )				//attempt to get the best resolution possible, even if this won't work in practice
#ifdef _2REAL_DEBUG
		, m_UpdateCount( 0 )
		, m_SkippedCount( 0 )
		, m_DebugTime()
#endif
	{
		m_Callback = new Poco::TimerCallback< Timer >( *this, &Timer::receiveTimerSignal );
		m_Timer.start( *m_Callback, Poco::Thread::PRIO_HIGHEST );
	}

	Timer::~Timer()
	{
		m_Timer.stop();
		delete m_Callback;
	}

	void Timer::receiveTimerSignal( Poco::Timer &t )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		long elapsed = ( long )m_Timestamp.elapsed();
		m_TimerSignal.notify( elapsed );
		m_Timestamp.update();

#ifdef _2REAL_DEBUG
		m_UpdateCount++;
		m_SkippedCount += t.skipped();
		if ( ( m_UpdateCount + m_SkippedCount ) >= 20000 )
		{
			std::cout << "updated " << m_UpdateCount << " times, skipped " << m_SkippedCount << " times, elapsed: " << m_DebugTime.elapsed() << std::endl;
			m_UpdateCount = 0;
			m_SkippedCount = 0;
			m_DebugTime.update();
		}
#endif

	}

	void Timer::registerToTimerSignal( AbstractCallback< long > &callback )
	{
		m_TimerSignal.addListener( callback );
	}

	void Timer::unregisterFromTimerSignal( AbstractCallback< long > &callback )
	{
		m_TimerSignal.removeListener( callback );
	}
}