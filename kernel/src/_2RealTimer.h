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

#include "_2RealTimeBasedTrigger.h"
#include "_2RealThreadPool.h"
#include "_2RealPoco.h"

#include <iostream>

namespace _2Real
{

	class Timer
	{

	public:

		Timer();
		~Timer();

		void receiveTimerSignal( Poco::Timer &t );
		void registerToTimerSignal( AbstractTimeBasedTrigger &trigger ) const;
		void unregisterFromTimerSignal( AbstractTimeBasedTrigger &trigger ) const;
		void registerToTimerSignal( ThreadPool &pool ) const;
		void unregisterFromTimerSignal( ThreadPool &pool ) const;

	private:

		Poco::AbstractTimerCallback			*m_Callback;
		Poco::Timer							m_Timer;
		Poco::Timestamp						m_Timestamp;
		mutable Poco::BasicEvent< long >	m_TimerSignal;

		mutable Poco::FastMutex				m_TestMutex;

#ifdef _2REAL_DEBUG
		long								m_UpdateCount;
		long								m_SkippedCount;
		Poco::Timestamp						m_DebugTime;
#endif

	};

	inline Timer::Timer() :
		m_Callback(nullptr),
		m_Timer(0, 1),	//attempt to get the best resolution possible, even if this won't work in practice
		m_Timestamp(),
		m_TimerSignal()
#ifdef _2REAL_DEBUG
		, m_UpdateCount(0)
		, m_SkippedCount(0)
		, m_DebugTime()
#endif
	{
		m_Callback = new Poco::TimerCallback< Timer >( *this, &Timer::receiveTimerSignal );
		m_Timer.start( *m_Callback, Poco::Thread::PRIO_HIGHEST );
	}

	inline Timer::~Timer()
	{
		m_Timer.stop();
		delete m_Callback;
	}

	inline void Timer::receiveTimerSignal(Poco::Timer &t)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TestMutex);

		long elapsed = (long)m_Timestamp.elapsed();
		m_TimerSignal.notify( this, elapsed );
		m_Timestamp.update();

#ifdef _2REAL_DEBUG
		m_UpdateCount++;
		m_SkippedCount += t.skipped();
		if ((m_UpdateCount + m_SkippedCount) >= 20000)
		{
			std::cout << "updated " << m_UpdateCount << " times, skipped " << m_SkippedCount << " times, elapsed: " << m_DebugTime.elapsed() << std::endl;
			m_UpdateCount = 0;
			m_SkippedCount = 0;
			m_DebugTime.update();
		}
#endif

	}

	inline void Timer::registerToTimerSignal( AbstractTimeBasedTrigger &trigger ) const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TestMutex);

		m_TimerSignal += Poco::delegate( &trigger, &AbstractTimeBasedTrigger::tryTriggerUpdate );
	}

	inline void Timer::unregisterFromTimerSignal( AbstractTimeBasedTrigger &trigger ) const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TestMutex);

		m_TimerSignal -= Poco::delegate( &trigger, &AbstractTimeBasedTrigger::tryTriggerUpdate );
	}

	inline void Timer::registerToTimerSignal(ThreadPool &pool) const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TestMutex);

		m_TimerSignal += Poco::delegate(&pool, &ThreadPool::update);
	}

	inline void Timer::unregisterFromTimerSignal(ThreadPool &pool) const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TestMutex);

		m_TimerSignal -= Poco::delegate(&pool, &ThreadPool::update);
	}

}