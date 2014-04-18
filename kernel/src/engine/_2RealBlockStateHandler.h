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
#include "common/_2RealSignals.h"

#include "engine/_2RealPreSetupState.h"
#include "engine/_2RealPostSetupState.h"
#include "engine/_2RealPostSetupState_Running.h"
#include "engine/_2RealPostShutdownState.h"

namespace _2Real
{
	class BlockState_I;
	class UpdateTrigger_I;
	class ThreadpoolImpl_I;
	struct SignalResponse;
	class BlockIo;

	class BlockStateHandler
	{

	public:

		BlockStateHandler( std::shared_ptr< ThreadpoolImpl_I >, std::shared_ptr< BlockIo > );

		std::future< BlockResult > setup();
		std::future< BlockResult > singlestep();
		std::future< BlockResult > shutdown();
		std::future< BlockResult > startRunning( std::shared_ptr< UpdateTrigger_I > );
		std::future< BlockResult > stopRunning();
		std::future< BlockResult > destroy();

		// trigger
		void update(); 	

		// thread pool
		void onActionComplete();

	private:

		std::shared_ptr< BlockState_I > createBlock( const BlockState, std::shared_ptr< SignalResponse > );

		// these functions are called when the mutex mMutex is locked
		std::future< BlockResult > carryOut( std::shared_ptr< SignalResponse > );
		void finalizeStateTransition();
		void enqueueJob();

		std::weak_ptr< ThreadpoolImpl_I >				mThreads;
		std::shared_ptr< BlockState_I >					mState;
		std::shared_ptr< BlockIo >						mServiceObj;

		std::shared_ptr< SignalResponse >				mResponse;
		bool											mIsActionInProcess;
		std::deque< std::shared_ptr< SignalResponse > >	mQueuedResponses;	
		mutable std::mutex								mMutex;
		
		ThreadpoolImpl_I::Id							mId;

	};

}