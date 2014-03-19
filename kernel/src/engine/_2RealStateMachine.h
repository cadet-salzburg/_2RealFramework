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
#include "engine/_2RealStateData.h"
#include "engine/_2RealThreadpool.h"

#include <future>
#include "enums/_2RealBlockState.h"

namespace _2Real
{
	class AbstractBlockState;
	class UpdateTrigger;
	class Threadpool;
	struct SignalResponse;
	class AbstractSharedService;

	/*
	*	the state machine is a bit weird, and i know this.
	*	to put it bluntly, it was designed this way to keep the debugging easy for me
	*	seing how ease of finding multi-threading related errors > design
	*/

	class StateMachine
	{

	public:

		StateMachine( std::shared_ptr< Threadpool >, std::shared_ptr< AbstractSharedService > );

		~StateMachine();

		// user input
		std::future< BlockState > setup();
		std::future< BlockState > singlestep();
		std::future< BlockState > shutdown();
		std::future< BlockState > startRunning( std::shared_ptr< UpdateTrigger > );
		std::future< BlockState > stopRunning();

		// trigger of some sort
		void update(); 	
		// shutdown
		std::future< BlockState > destroy();

		// thread pool
		void onActionComplete();

		void noop() {}

	private:

		// these functions are called when the mutex mMutex is locked
		std::future< BlockState > carryOut( std::shared_ptr< SignalResponse > );
		void finalizeStateTransition();

		std::weak_ptr< Threadpool >						mThreads;

		std::shared_ptr< AbstractBlockState >			mState;
		std::shared_ptr< SignalResponse >				mResponse;

		std::shared_ptr< UpdateTrigger >				mUpdateTrigger;

		bool											mIsActionInProcess;
		std::deque< std::shared_ptr< SignalResponse > >	mQueuedResponses;	
		mutable std::mutex								mMutex;

		std::shared_ptr< AbstractSharedService >		mServiceObj;	
		Threadpool::Id									mId;

		uint64_t										mSkippedCounter;
		uint64_t										mUpdatedCounter;

	};
}