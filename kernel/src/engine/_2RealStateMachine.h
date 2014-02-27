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

namespace _2Real
{
	class AbstractBlockState;
	class UpdateTrigger;
	class ActionRequest;
	class Threadpool;
	struct SignalResponse;

	/*
	*	the state machine is a bit weird, and i know this.
	*	to put it bluntly, it was designed this way to keep the debugging easy for me
	*	seing how ease of finding multi-threading related errors > design
	*/

	class StateMachine
	{

	public:

		StateMachine( std::shared_ptr< Threadpool > );

		// user signals
		void startRunning( std::shared_ptr< UpdateTrigger >, CbPtr );
		void stopRunning( CbPtr );
		void setup( CbPtr );
		void singleUpdate( CbPtr );
		void shutdown( CbPtr );

		// trigger of some sort
		void update(); 
		
		// shutdown
		void destroy( CbPtr );

		void onActionRequestComplete();

	private:

		void carryOut( std::shared_ptr< SignalResponse > );
		void onActionComplete();
		void finalizeTransition();

		std::weak_ptr< Threadpool >						mThreads;

		std::shared_ptr< AbstractBlockState >			mState;
		std::shared_ptr< ActionRequest >				mRequest;
		std::shared_ptr< SignalResponse >				mResponse;

		std::shared_ptr< UpdateTrigger >				mUpdateTrigger;

		bool											mIsActionInProcess;
		std::deque< std::shared_ptr< SignalResponse > >	mQueuedResponses;	
		mutable std::mutex								mMutex;

	};
}