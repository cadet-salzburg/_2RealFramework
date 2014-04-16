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

#include "common/_2RealStdIncludes.h"
#include "engine/_2RealSignalResponse.h"
#include "engine/_2RealThreadpoolImpl_I.h"

#include <future>
#include "common/_2RealBlockState.h"

namespace _2Real
{
	class BlockState_I;
	class UpdateTrigger_I;
	class ThreadpoolImpl_I;
	struct SignalResponse;

	/*
	*	the state machine is a bit weird, and i know this.
	*	to put it bluntly, it was designed this way to keep the debugging easy for me
	*	seing how ease of finding multi-threading related errors > design
	*/

	class BlockIo;

	class BlockStateHandler
	{

	public:

		BlockStateHandler( std::shared_ptr< ThreadpoolImpl_I >, std::shared_ptr< BlockIo > );

		~BlockStateHandler();

		// user input
		std::future< BlockState > setup();
		std::future< BlockState > singlestep();
		std::future< BlockState > shutdown();
		std::future< BlockState > startRunning( std::shared_ptr< UpdateTrigger_I > );
		std::future< BlockState > stopRunning();

		// trigger of some sort
		void update(); 	
		// shutdown
		std::future< BlockState > destroy();

		// thread pool
		void onActionComplete();

	private:

		// these functions are called when the mutex mMutex is locked
		std::future< BlockState > carryOut( std::shared_ptr< SignalResponse > );
		void finalizeStateTransition();

		std::weak_ptr< ThreadpoolImpl_I >						mThreads;

		std::shared_ptr< BlockState_I >					mState;
		std::shared_ptr< SignalResponse >				mResponse;

		std::shared_ptr< UpdateTrigger_I >				mUpdateTrigger;

		bool											mIsActionInProcess;
		std::deque< std::shared_ptr< SignalResponse > >	mQueuedResponses;	
		mutable std::mutex								mMutex;

		std::shared_ptr< BlockIo >						mServiceObj;	
		ThreadpoolImpl_I::Id									mId;

		uint64_t										mSkippedCounter;
		uint64_t										mUpdatedCounter;

	};
}