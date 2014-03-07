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

namespace _2Real
{
	StateMachine::StateMachine( std::shared_ptr< Threadpool > threads, std::shared_ptr< AbstractSharedService > service ) :
		mThreads( threads ),
		mState( AbstractBlockState::create( BlockState::PRE_SETUP ) ),
		mResponse( nullptr ),
		mUpdateTrigger( nullptr ),
		mIsActionInProcess( false ),
		mServiceObj( service ),
		mId( threads->createId() )
	{
	}

	StateMachine::~StateMachine()
	{
	}

	/*
	void StateMachine::startRunning( std::shared_ptr< UpdateTrigger > trigger, std::function< void() > const& cb )
	{
#ifdef _DEBUG
		assert( trigger );
#endif

		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onStartRunning();
		response->onFininshed = cb;
		response->updateTrigger = trigger; // store trigger for later
		carryOut( response );
	}

	void StateMachine::stopRunning( std::function< void() > const& cb )
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onStopRunning();
		response->onFininshed = cb;
		carryOut( response );
	}

	void StateMachine::update()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onUpdateSignalReceived();
		// !no callback to execute here!
		carryOut( response );
	}

	void StateMachine::setup( std::function< void() > const& cb )
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onSetupSignalReceived();
		response->onFininshed = cb;

		std::cout << "carrying out the response" << std::endl;
		carryOut( response );
	}

	void StateMachine::singleUpdate( std::function< void() > const& cb )
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onSingleUpdateSignalReceived();
		response->onFininshed = cb;
		carryOut( response );
	}

	void StateMachine::shutdown( std::function< void() > const& cb )
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onShutdownSignalReceived();
		response->onFininshed = cb;
		carryOut( response );
	}

	void StateMachine::destroy( std::function< void() > const& cb )
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onEngineShutdownReceived();
		response->onFininshed = cb;
		carryOut( response );
	}
	*/

	std::future< BlockState > StateMachine::setup()
	{
		mMutex.lock();
		std::cout << "SM: setup" << std::endl;
		std::shared_ptr< SignalResponse > response = mState->onSetupSignalReceived();
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::singlestep()
	{	
		mMutex.lock();
		std::cout << "SM: update" << std::endl;
		std::shared_ptr< SignalResponse > response = mState->onSingleUpdateSignalReceived();
		return carryOut( response );
	}

	std::future< BlockState > StateMachine::shutdown()
	{
		mMutex.lock();
		std::cout << "SM: shutdown" << std::endl;
		std::shared_ptr< SignalResponse > response = mState->onShutdownSignalReceived();
		return carryOut( response );
	}

	void StateMachine::onActionComplete()
	{
		mMutex.lock();

		std::cout << "SM: action complete" << std::endl;
		// now there's definitely no more action in process
		mIsActionInProcess = false;
		// this is where the state change takes place
		finalizeStateTransition();
		std::cout << "SM: state transitioned" << std::endl;

		// immediately process the next action
		// ( this might be critical if an engine shutdown gets delayed inifitely ? )
		// however, as it currently stands, the only other queued action is
		// a 'stopRunning', which is a no-operation
		if ( !mQueuedResponses.empty() )
		{
			std::cout << "SM: processing queued actions" << std::endl;
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
				std::cout << "SM: queued action is setup" << std::endl;
				job = std::bind( &AbstractSharedService::setup, mServiceObj );
				break;
			case Action::DO_UPDATE:
				std::cout << "SM: queued action is update" << std::endl;
				job = std::bind( &AbstractSharedService::update, mServiceObj );
				break;
			case Action::DO_SHUTDOWN:
				std::cout << "SM: queued action is shutdown" << std::endl;
				job = std::bind( &AbstractSharedService::shutdown, mServiceObj );
				break;
			default:
				std::cout << "SM: queued action is no-op" << std::endl;
				job = std::bind( &StateMachine::noop, this );
				break;
			}

			std::function< void() > callback = std::bind( &StateMachine::onActionComplete, this );
			std::shared_ptr< Threadpool > threads = mThreads.lock();
#ifdef _DEBUG
			assert( threads );
#endif
			threads->enqueueJob( mId, job, callback );
			std::cout << "SM: queued job enqueued" << std::endl;
		}
		else
		{
			std::cout << "SM: no queued actions, returning" << std::endl;
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
			std::cout << "SM: action in progress, storing response for later" << std::endl;

			// store for later, if the shouldWait flag is set
			// else: immediately set the promise to the current state
			// then unlock mutex & stop
			if ( response->shouldWait )
				mQueuedResponses.push_back( response );
			else
				response->isFinished.set_value( mState->getId() );

			mMutex.unlock();
		}
		else
		{
			std::cout << "SM: no action in progress, immediately processing response" << std::endl;

			// keep the response around, since it holds the std::promise
			mResponse = response;
			// allright, now there's definitely an action in process
			mIsActionInProcess = true;

			mMutex.unlock();

			// now, there's all the time in the world enqueue the job
			std::function< void() > job;
			switch ( mResponse->action )
			{
			case Action::DO_SETUP:
				std::cout << "SM: new action is setup" << std::endl;
				job = std::bind( &AbstractSharedService::setup, mServiceObj );
				break;
			case Action::DO_UPDATE:
				std::cout << "SM: new action is update" << std::endl;
				job = std::bind( &AbstractSharedService::update, mServiceObj );
				break;
			case Action::DO_SHUTDOWN:
				std::cout << "SM: new action is shutdown" << std::endl;
				job = std::bind( &AbstractSharedService::shutdown, mServiceObj );
				break;
			default:
				std::cout << "SM: new action is no-op" << std::endl;
				job = std::bind( &StateMachine::noop, this );
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

			std::cout << "SM: new job enqueued" << std::endl;
		}

		return response->isFinished.get_future();
	}

	void StateMachine::finalizeStateTransition()
	{
		// this function is called only when the mutex is locked
		// it basically performs some signals registration / unregistration
		BlockState current = mState->getId();
		BlockState next = mResponse->followupState;

		switch ( current )
		{
		case _2Real::BlockState::PRE_SETUP:
			std::cout << "current is pre setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP:
			std::cout << "current is post setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP_RUNNING:
			std::cout << "current is post setup - running state" << std::endl;
			break;
		case _2Real::BlockState::POST_SHUTDOWN:
			std::cout << "current is post shutdown state" << std::endl;
			break;
		}

		switch ( next )
		{
		case _2Real::BlockState::PRE_SETUP:
			std::cout << "next is pre setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP:
			std::cout << "next is post setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP_RUNNING:
			std::cout << "next is post setup - running state" << std::endl;
			break;
		case _2Real::BlockState::POST_SHUTDOWN:
			std::cout << "next is post shutdown state" << std::endl;
			break;
		}

		if ( current == next )
		{
			// of both states are the same, nothing needs to be done
		}
		else
		{
			mState = AbstractBlockState::create( next );
		}

/////		// TMP disable
		//else if ( current == BlockState::RUNNING )
		//{
		//	std::shared_ptr< AbstractCallback_T< void > > cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::update ) );
		//	mUpdateTrigger->unregisterFromUpdate( cb );
		//	mUpdateTrigger.reset();

		//	mState = AbstractBlockState::create( next );
		//}
		//else if ( next == BlockState::RUNNING )
		//{
		//	mUpdateTrigger = mResponse->updateTrigger;
		//	std::shared_ptr< AbstractCallback_T< void > > cb( new MemberCallback_T< StateMachine, void >( this, &StateMachine::update ) );
		//	mUpdateTrigger->registerToUpdate( cb );

		//	mState = AbstractBlockState::create( next );
		//}
/////		// TMP disable

		mResponse->isFinished.set_value( next );
		mResponse.reset();		// obj no longer needed
	}
}
