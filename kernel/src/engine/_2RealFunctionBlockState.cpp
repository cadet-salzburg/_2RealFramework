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

#include "engine/_2RealFunctionBlockState.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealFunctionBlockStateManager.h"

#include <assert.h>

namespace _2Real
{

	bool FunctionBlockStateCreated::trySetUp( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateCreated::tryStart( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: created -> start" );
	}

	bool FunctionBlockStateCreated::tryStop( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: created -> stop" );
	}

	void FunctionBlockStateCreated::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateCreated::singleStep( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: created -> single step" );
	}

	void FunctionBlockStateCreated::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateCreated::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateCreated::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateCreated::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateCreated::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateCreated::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateCreated::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateInitialized::trySetUp( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateInitialized::tryStart( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateInitialized::tryStop( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateInitialized::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateInitialized::singleStep( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateInitialized::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateInitialized::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateInitialized::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateInitialized::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateInitialized::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateInitialized::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateInitialized::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateTriggering::trySetUp( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateTriggering::tryStart( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateTriggering::tryStop( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateTriggering::triggersAreOk( FunctionBlockStateManager &states ) const
	{
	}

	bool FunctionBlockStateTriggering::singleStep( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateTriggering::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateTriggering::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateTriggering::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateTriggering::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateTriggering::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateTriggering::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateTriggering::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateWaiting::trySetUp( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateWaiting::tryStart( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateWaiting::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateWaiting::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateWaiting::singleStep( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateWaiting::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateWaiting::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateWaiting::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateWaiting::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateWaiting::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateWaiting::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateWaiting::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateScheduled::trySetUp( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateScheduled::tryStart( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateScheduled::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateScheduled::singleStep( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateScheduled::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateScheduled::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateScheduled::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateScheduled::beginUpdate( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateScheduled::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateScheduled::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateScheduled::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateScheduled::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateUpdating::trySetUp( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateUpdating::tryStart( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateUpdating::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateUpdating::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateUpdating::singleStep( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateUpdating::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateUpdating::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateUpdating::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateUpdating::update( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateUpdating::finishUpdate( FunctionBlockStateManager &states ) const
	{
	}

	bool FunctionBlockStateUpdating::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateUpdating::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateStopped::trySetUp( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> set up" );
	}

	bool FunctionBlockStateStopped::tryStart( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> start" );
	}

	bool FunctionBlockStateStopped::tryStop( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> stop" );
	}

	void FunctionBlockStateStopped::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateStopped::singleStep( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> single step" );
	}

	void FunctionBlockStateStopped::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateStopped::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateStopped::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateStopped::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateStopped::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateStopped::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateStopped::shutDown( FunctionBlockStateManager &states ) const
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateShutDown::trySetUp( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> set up" );
	}

	bool FunctionBlockStateShutDown::tryStart( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> start" );
	}

	bool FunctionBlockStateShutDown::tryStop( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> stop" );
	}

	void FunctionBlockStateShutDown::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateShutDown::singleStep( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> single step" );
	}

	void FunctionBlockStateShutDown::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateShutDown::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateShutDown::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateShutDown::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateShutDown::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateShutDown::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
		return false;
	}

	void FunctionBlockStateShutDown::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateError::trySetUp( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateError::tryStart( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: error -> start" );
	}

	bool FunctionBlockStateError::tryStop( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: error -> stop" );
	}

	void FunctionBlockStateError::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateError::singleStep( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: error -> single step" );
	}

	void FunctionBlockStateError::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateError::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateError::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateError::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void FunctionBlockStateError::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool FunctionBlockStateError::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateError::shutDown( FunctionBlockStateManager &states ) const
	{
	}

}