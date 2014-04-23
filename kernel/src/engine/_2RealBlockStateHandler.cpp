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

#include "engine/_2RealBlockStateHandler.h"
#include "engine/_2RealBlockState_I.h"
#include "engine/_2RealSignalResponse.h"
#include "engine/_2RealUpdateTrigger_I.h"
#include "engine/_2RealBlockImpl.h"

namespace _2Real
{
	BlockStateHandler::BlockStateHandler( std::shared_ptr< ThreadpoolImpl_I > threads, std::shared_ptr< BlockIo > service ) :
		mThreads( threads ),
		mState( createBlock( BlockState::PRE_SETUP, nullptr ) ),
		mResponse( nullptr ),
		mIsActionInProcess( false ),
		mServiceObj( service ),
		mId( threads->createId() )
	{
	}

	void BlockStateHandler::update()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onUpdateSignalReceived();
		carryOut( response );
	}

	std::future< BlockResult > BlockStateHandler::destroy()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onEngineShutdownReceived();
		return carryOut( response );
	}

	std::future< BlockResult > BlockStateHandler::setup()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onSetupSignalReceived();
		return carryOut( response );
	}

	std::future< BlockResult > BlockStateHandler::singlestep()
	{	
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onSingleUpdateSignalReceived();
		return carryOut( response );
	}

	std::future< BlockResult > BlockStateHandler::shutdown()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onShutdownSignalReceived();
		return carryOut( response );
	}

	std::future< BlockResult > BlockStateHandler::startRunning( std::shared_ptr< UpdateTrigger_I > trigger )
	{
#ifdef _DEBUG
		assert( trigger );
#endif
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onStartRunning();
		// store trigger for later, otherwise it's null
		response->updateTrigger = trigger;
		return carryOut( response );
	}

	std::future< BlockResult > BlockStateHandler::stopRunning()
	{
		mMutex.lock();
		std::shared_ptr< SignalResponse > response = mState->onStopRunning();
		return carryOut( response );
	}

	void BlockStateHandler::onActionComplete()
	{
		mMutex.lock();
		mIsActionInProcess = false;
		finalizeStateTransition();

		if ( !mQueuedResponses.empty() )
		{
			mResponse = mState->nextAction( mQueuedResponses );
			assert( mResponse.get() );
			mIsActionInProcess = true;
			mMutex.unlock();
			enqueueJob();
		}
		else
		{
			mMutex.unlock();
		}
	}

	std::future< BlockResult > BlockStateHandler::carryOut( std::shared_ptr< SignalResponse > response )
	{
		if ( mIsActionInProcess )
		{
			if ( response->shouldBeQueued )
				mQueuedResponses.push_back( response );
			else
				// return 'failure'
				response->result.set_value( BlockResult::IGNORED );

			mMutex.unlock();
		}
		else
		{
			mResponse = response;
			mIsActionInProcess = true;
			mMutex.unlock();

			enqueueJob();
		}

		return response->result.get_future();
	}

	void BlockStateHandler::finalizeStateTransition()
	{
		// change state if follow up state differs
		BlockState next = mResponse->followupState;
		if ( !mState->operator==( next ) )
			mState = createBlock( next, mResponse );

		// return success
		mResponse->result.set_value( BlockResult::CARRIED_OUT );
		mResponse.reset();
	}

	void BlockStateHandler::enqueueJob()
	{
		std::function< void() > job;
		switch ( mResponse->action )
		{
		case BlockAction::DO_SETUP:
			job = std::bind( &BlockIo::doSetup, mServiceObj.get() );
			break;
		case BlockAction::DO_UPDATE:
			job = std::bind( &BlockIo::doUpdate, mServiceObj.get() );
			break;
		case BlockAction::DO_SHUTDOWN:
			job = std::bind( &BlockIo::doShutdown, mServiceObj.get() );
			break;
		default:
			job = std::bind( &BlockIo::doNothing, mServiceObj.get() );
			break;
		}

		std::function< void() > callback = std::bind( &BlockStateHandler::onActionComplete, this );
		std::shared_ptr< ThreadpoolImpl_I > threads = mThreads.lock();
#ifdef _DEBUG
			assert( threads );
#endif
		threads->enqueueJob( mId, job, callback );
	}

	std::shared_ptr< BlockState_I > BlockStateHandler::createBlock( const BlockState id, std::shared_ptr< SignalResponse > response )
	{
		switch ( id )
		{
		case BlockState::PRE_SETUP:
			return std::shared_ptr< BlockState_I >( new PreSetupState( id ) );
		case BlockState::POST_SETUP:
			return std::shared_ptr< BlockState_I >( new PostSetupState( id ) );
		case BlockState::POST_SETUP_RUNNING:
			return std::shared_ptr< BlockState_I >( new PostSetupState_Running( id, response->updateTrigger->registerToUpdate( std::bind( &BlockStateHandler::update, this ) ) ) );
		case BlockState::POST_SHUTDOWN:
			return std::shared_ptr< BlockState_I >( new PostShutdownState( id ) );
		default:
			return std::shared_ptr< BlockState_I >();
		}
	}
}
