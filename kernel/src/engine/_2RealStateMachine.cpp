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
		//std::cout << "SM: setup" << std::endl;
		std::shared_ptr< SignalResponse > response = mState->onSetupSignalReceived();
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::singlestep()
	{	
		mMutex.lock();
		//std::cout << "SM: update" << std::endl;
		std::shared_ptr< SignalResponse > response = mState->onSingleUpdateSignalReceived();
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::shutdown()
	{
		mMutex.lock();
		//std::cout << "SM: shutdown" << std::endl;
		std::shared_ptr< SignalResponse > response = mState->onShutdownSignalReceived();
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::startRunning( std::shared_ptr< UpdateTrigger > trigger )
	{
#ifdef _DEBUG
		assert( trigger );
#endif
		mMutex.lock();
		//std::cout << "SM: start running" << std::endl;
		std::shared_ptr< SignalResponse > response = mState->onStartRunning();
		response->updateTrigger = trigger; // store trigger for later
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::stopRunning()
	{
		mMutex.lock();
		//std::cout << "SM: stop running" << std::endl;
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
					//std::cout << "SM: skipped " << mSkippedCounter << std::endl;
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
				//std::cout << "SM: skipped " << mUpdatedCounter << std::endl;
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

		//switch ( current )
		//{
		//case _2Real::BlockState::PRE_SETUP:
		//	std::cout << "current is pre setup state" << std::endl;
		//	break;
		//case _2Real::BlockState::POST_SETUP:
		//	std::cout << "current is post setup state" << std::endl;
		//	break;
		//case _2Real::BlockState::POST_SETUP_RUNNING:
		//	std::cout << "current is post setup - running state" << std::endl;
		//	break;
		//case _2Real::BlockState::POST_SHUTDOWN:
		//	std::cout << "current is post shutdown state" << std::endl;
		//	break;
		//}

		//switch ( next )
		//{
		//case _2Real::BlockState::PRE_SETUP:
		//	std::cout << "next is pre setup state" << std::endl;
		//	break;
		//case _2Real::BlockState::POST_SETUP:
		//	std::cout << "next is post setup state" << std::endl;
		//	break;
		//case _2Real::BlockState::POST_SETUP_RUNNING:
		//	std::cout << "next is post setup - running state" << std::endl;
		//	break;
		//case _2Real::BlockState::POST_SHUTDOWN:
		//	std::cout << "next is post shutdown state" << std::endl;
		//	break;
		//}

		if ( current == next )
		{
			// of both states are the same, nothing needs to be done
		}
		else
		{
			if ( mUpdateTrigger )
			{
				std::shared_ptr< AbstractCallback_T< void > > cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::update ) );
				mUpdateTrigger->unregisterFromUpdate( cb );
				mUpdateTrigger.reset();
			}

			if ( mResponse->updateTrigger )
			{
				std::shared_ptr< AbstractCallback_T< void > > cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::update ) );
				mUpdateTrigger = mResponse->updateTrigger;
				mUpdateTrigger->registerToUpdate( cb );
			}

			mState = AbstractBlockState::create( next );
		}

		mResponse->isFinished.set_value( next );
		mResponse.reset();		// obj no longer needed
	}
}
