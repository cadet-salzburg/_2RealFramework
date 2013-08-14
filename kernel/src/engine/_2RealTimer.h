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

#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealEvent.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	class Logger;
	class EngineImpl;
	template< typename T >
	class Callback;

	class Timer
	{

	public:

		Timer( EngineImpl *, const unsigned long, const bool = false );
		~Timer();

		void start();
		void stop();

		void receiveTimerSignal( Poco::Timer &t );
		void registerToTimerSignal( AbstractCallback< long > *callback );
		void unregisterFromTimerSignal( AbstractCallback< long > *callback );

	private:

		Poco::AbstractTimerCallback			*mCallback;
		Poco::Timer							mTimer;
		Poco::Timestamp						mTimestamp;

		EngineImpl							*const mEngineImpl;

		mutable Poco::FastMutex				mAccess;
		CallbackEvent< long >				mTimerSignal;

		long								mUpdateCount;
		long								mSkippedCount;
		Poco::Timestamp						mDebugTime;

		bool								mLog;

	};
}