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

#include "engine/_2RealStateMachine.h"
#include "engine/_2RealAbstractBlockState.h"
#include "engine/_2RealStateData.h"
#include "engine/_2RealUpdateTrigger.h"

namespace _2Real
{
	StateMachine::StateMachine( std::shared_ptr< Threadpool > threads ) :
		mThreads( threads ),
		mState( AbstractBlockState::create( State::PRE_SETUP ) )
	{
	}

	void StateMachine::startRunning( std::shared_ptr< UpdateTrigger > trigger, std::shared_ptr< AbstractCallback_T< void > > cb )
	{
#ifdef _DEBUG
		assert( trigger );
#endif

		std::lock_guard< std::mutex > lock( mMutex );
		std::shared_ptr< SignalResponse > response = mState->onStartRunning( cb );
		carryOut( response );
	}

	void StateMachine::stopRunning( std::shared_ptr< AbstractCallback_T< void > > cb )
	{
		std::lock_guard< std::mutex > lock( mMutex );
		std::shared_ptr< SignalResponse > response = mState->onStopRunning( cb );
		carryOut( response );
	}

	void StateMachine::update()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onUpdateSignalReceived();
		carryOut( response );
	}

	void StateMachine::setup( std::shared_ptr< AbstractCallback_T< void > > cb )
	{
		std::lock_guard< std::mutex > lock( mMutex );
		std::shared_ptr< SignalResponse > response = mState->onSetupSignalReceived( cb );
		carryOut( response );
	}

	void StateMachine::singleUpdate( std::shared_ptr< AbstractCallback_T< void > > cb )
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onSingleUpdateSignalReceived( cb );
		carryOut( response );
	}

	void StateMachine::shutdown( std::shared_ptr< AbstractCallback_T< void > > cb )
	{
		std::lock_guard< std::mutex > lock( mMutex );
		std::shared_ptr< SignalResponse > response = mState->onShutdownSignalReceived( cb );
		carryOut( response );
	}

	void StateMachine::destroy( std::shared_ptr< AbstractCallback_T< void > > cb )
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onEngineShutdownReceived( cb );
		carryOut( response );
	}

	void StateMachine::onActionRequestComplete()
	{
		mMutex.lock();
		mRequest.reset();	// <- kills request
		onActionComplete();	// <- unlocks the mutex
	}

	void StateMachine::onActionComplete()
	{
		// now there's definitely no more action in process
		mIsActionInProcess = false;

		finalizeTransition();

		// immediately process the next action
		// this might be critical if an engine shutdown gets delayed inifitely?
		// however, as it currently stands, the only other queued action is
		// a 'stopRunning', which is a no-operation
		if ( !mQueuedResponses.empty() )
		{
			mResponse = mQueuedResponses.front();
			mQueuedResponses.pop_front();

			if ( mResponse->action == Action::DO_NOTHING )
			{
				onActionComplete();
			}
			else
			{
				// as of now, an action is in process
				mIsActionInProcess = true;
				mMutex.unlock();

				mRequest.reset( new ActionRequest );
				CbPtr cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::onActionRequestComplete ) );
				std::shared_ptr< Threadpool > threads = mThreads.lock();
#ifdef _DEBUG
				assert( threads );
#endif
				mRequest->start( threads, cb );
			}
		}

		mMutex.unlock();
	}

	void StateMachine::carryOut( std::shared_ptr< SignalResponse > response )
	{
		if ( mIsActionInProcess )
		{
			// finish_current is the only thing that will not be ignored
			if ( response->shouldWait )
				mQueuedResponses.push_back( response );

			mMutex.unlock();
		}
		else
		{
			mResponse = response;

			if ( response->action == Action::DO_NOTHING )
			{
				// nothing actually needs to be done -> just finalize the state change
				onActionComplete();
			}
			else
			{
				mIsActionInProcess = true;
				mMutex.unlock();

				mRequest.reset( new ActionRequest );
				CbPtr cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::onActionRequestComplete ) );
				std::shared_ptr< Threadpool > threads = mThreads.lock();
#ifdef _DEBUG
				assert( threads );
#endif
				mRequest->start( threads, cb );
			}
		}
	}

	void StateMachine::finalizeTransition()
	{
		// this function is called only when the mutex is locked
		// it basically performs some signals registration / unregistration

		State::Id current = mState->getId();
		State::Id next = mResponse->followupState;

		if ( current == next )
			return;

		if ( current == State::POST_SETUP_RUNNING )
		{
			CbPtr cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::update ) );
			mUpdateTrigger->unregisterFromUpdate( cb );
			mUpdateTrigger.reset();
		}

		if ( next == State::POST_SETUP_RUNNING )
		{
			mUpdateTrigger = mResponse->updateTrigger;
			CbPtr cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::update ) );
			mUpdateTrigger->registerToUpdate( cb );
		}

		// this object is no longer needed
		mResponse.reset();

		mState = AbstractBlockState::create( next );
	}
}
