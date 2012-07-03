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

#include "helpers/_2RealPoco.h"
#include "app/_2RealCallbacksInternal.h"

namespace _2Real
{
	class Timer
	{

	public:

		Timer();
		~Timer();

		void receiveTimerSignal( Poco::Timer &t );
		void registerToTimerSignal( AbstractCallback< long > &callback );
		void unregisterFromTimerSignal( AbstractCallback< long > &callback );

	private:

		Poco::AbstractTimerCallback			*m_Callback;
		Poco::Timer							m_Timer;
		Poco::Timestamp						m_Timestamp;

		mutable Poco::FastMutex				m_Access;
		CallbackEvent< long >				m_TimerSignal;

#ifdef _2REAL_DEBUG
		long								m_UpdateCount;
		long								m_SkippedCount;
		Poco::Timestamp						m_DebugTime;
#endif

	};
}