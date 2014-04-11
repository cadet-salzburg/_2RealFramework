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
#include "engine/_2RealSharedService.h"
#include "engine/_2RealBlock.h"

namespace _2Real
{
	StateMachine::StateMachine( std::shared_ptr< Threadpool > threads, std::shared_ptr< BlockIo > service ) :
		mThreads( threads ),
		mState( AbstractBlockState::create( BlockState::PRE_SETUP ) ),
		mResponse( nullptr ),
		mUpdateTrigger( nullptr ),
		mIsActionInProcess( false ),
		mServiceObj( service ),
		mId( threads->createId() ),
		mSkippedCounter( 0 ),
		mUpdatedCounter( 0 )
	{
	}

	StateMachine::~StateMachine()
	{
	}

	void StateMachine::update()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onUpdateSignalReceived();
		carryOut( response );
	}

	std::future< BlockState > StateMachine::destroy()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onEngineShutdownReceived();
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::setup()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onSetupSignalReceived();
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::singlestep()
	{	
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onSingleUpdateSignalReceived();
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::shutdown()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onShutdownSignalReceived();
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::startRunning( std::shared_ptr< UpdateTrigger > trigger )
	{
#ifdef _DEBUG
		assert( trigger );
#endif
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onStartRunning();
		response->updateTrigger = trigger; // store trigger for later
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::stopRunning()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onStopRunning();
		return carryOut( response );
	}

	void StateMachine::onActionComplete()
	{
		mMutex.lock();
		// now there's definitely no more action in process
		mIsActionInProcess = false;
		// this is where the state change takes place
		finalizeStateTransition();

		// immediately process the next action
		// ( this might be critical if an engine shutdown gets delayed inifitely ? )
		// however, as it currently stands, the only other queued action is
		// a 'stopRunning', which is a no-operation
		if ( !mQueuedResponses.empty() )
		{
			mResponse = mQueuedResponses.front();
			mQueuedResponses.pop_front();
			// as of now, an action is in process
			mIsActionInProcess = true;
			mMutex.unlock();

			// now, there's all the time in the world enqueue the job
			std::function< void() > job;
			switch ( mResponse->action )
			{
			case Action::DO_SETUP:
				job = std::bind( &BlockIo::doSetup, mServiceObj.get() );
				break;
			case Action::DO_UPDATE:
				job = std::bind( &BlockIo::doUpdate, mServiceObj.get() );
				break;
			case Action::DO_SHUTDOWN:
				job = std::bind( &BlockIo::doShutdown, mServiceObj.get() );
				break;
			default:
				job = std::bind( &BlockIo::doNothing, mServiceObj.get() );
				break;
			}

			std::function< void() > callback = std::bind( &StateMachine::onActionComplete, this );
			std::shared_ptr< Threadpool > threads = mThreads.lock();
#ifdef _DEBUG
			assert( threads );
#endif
			threads->enqueueJob( mId, job, callback );
		}
		else
		{
			mMutex.unlock();
		}
	}

	std::future< BlockState > StateMachine::carryOut( std::shared_ptr< SignalResponse > response )
	{
		// there are 2 'states' ( action vs. no action ) at work here
		// which i could have integrated into the state machine...
		// however, i prefer the way it's currently done, for ease of debugging

		// ... which might bite me in the ass just about now

		if ( mIsActionInProcess )
		{
			// store for later, if the shouldWait flag is set
			// else: immediately set the promise to the current state
			// then unlock mutex & stop
			if ( response->shouldWait )
				mQueuedResponses.push_back( response );
			else
			{
				// store the amount of skipped updates
				if ( response->action == Action::DO_UPDATE && mState->getId() == BlockState::POST_SETUP_RUNNING )
				{
					++mSkippedCounter;
				}

				response->isFinished.set_value( mState->getId() );
			}

			mMutex.unlock();
		}
		else
		{
			// keep the response around, since it holds the std::promise
			mResponse = response;
			// allright, now there's definitely an action in process
			mIsActionInProcess = true;

			// store the amount of executed updates
			if ( response->action == Action::DO_UPDATE && mState->getId() == BlockState::POST_SETUP_RUNNING )
			{
				++mUpdatedCounter;
			}

			mMutex.unlock();

			// now, there's all the time in the world enqueue the job
			std::function< void() > job;
			switch ( mResponse->action )
			{
			case Action::DO_SETUP:
				job = std::bind( &BlockIo::doSetup, mServiceObj.get() );
				break;
			case Action::DO_UPDATE:
				job = std::bind( &BlockIo::doUpdate, mServiceObj.get() );
				break;
			case Action::DO_SHUTDOWN:
				job = std::bind( &BlockIo::doShutdown, mServiceObj.get() );
				break;
			default:
				job = std::bind( &BlockIo::doNothing, mServiceObj.get() );
				break;
			}

			// callback to the state machine
			std::function< void() > callback = std::bind( &StateMachine::onActionComplete, this );

			// acquire threadpool & enqueue
			std::shared_ptr< Threadpool > threads = mThreads.lock();
#ifdef _DEBUG
			assert( threads );
#endif
			threads->enqueueJob( mId, job, callback );
		}

		return response->isFinished.get_future();
	}

	void StateMachine::finalizeStateTransition()
	{
		// this function is called only when the mutex is locked
		// it basically performs some signals registration / unregistration
		BlockState current = mState->getId();
		BlockState next = mResponse->followupState;

		if ( current == next && current == BlockState::POST_SETUP_RUNNING )
		{
			// check if they're the same?

			// need to handle this pretty much as if it were a signal coming in
			bool wasAlreadyFulfilled = mUpdateTrigger->reset();

			if ( wasAlreadyFulfilled )
			{
				// so now i'd have to do a new update;
				// however, only queue if absolutely no other signals are stored ( only other signals are stops sigs )
				if ( mQueuedResponses.empty() )
				{
					std::shared_ptr< SignalResponse > response( new SignalResponse );
					response->action = Action::DO_UPDATE;
					response->followupState = BlockState::POST_SETUP_RUNNING;
					response->shouldWait = true;
					mQueuedResponses.push_back( response );
				}
			}
		}
		else
		{
			// if an update trigger is stored, reset ( receive no more update signals )
			if ( mUpdateTrigger )
			{
				mUpdateTrigger->disable();		// disconnects from all inlets
				std::shared_ptr< AbstractCallback_T< void > > cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::update ) );
				mUpdateTrigger->unregisterFromUpdate( cb );
				mUpdateTrigger.reset();
			}

			// register to new update trigger ( receive update signals )
			if ( mResponse->updateTrigger )
			{
				mUpdateTrigger = mResponse->updateTrigger;
				mUpdateTrigger->enable();
				std::shared_ptr< AbstractCallback_T< void > > cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::update ) );
				mUpdateTrigger->registerToUpdate( cb );
			}

			mState = AbstractBlockState::create( next );
		}

		mResponse->isFinished.set_value( next );
		mResponse.reset();		// obj no longer needed
	}
}
