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

namespace _2Real
{

	void FunctionBlockStateCreated::setUp( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateCreated::start( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: created -> start" );
	}

	bool FunctionBlockStateCreated::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateCreated::tryHalt( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: created -> stop" );
	}

	bool FunctionBlockStateCreated::tryStop( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void FunctionBlockStateInitialized::setUp( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateInitialized::start( FunctionBlockStateManager &states ) const
	{
	}

	bool FunctionBlockStateInitialized::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateInitialized::tryHalt( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateInitialized::tryStop( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void FunctionBlockStateWaiting::setUp( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateWaiting::start( FunctionBlockStateManager &states ) const
	{
	}

	bool FunctionBlockStateWaiting::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateWaiting::tryHalt( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateWaiting::tryStop( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void FunctionBlockStateUpdating::setUp( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: updating -> setup" );
	}

	void FunctionBlockStateUpdating::start( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: updating -> start" );
	}

	bool FunctionBlockStateUpdating::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateUpdating::tryHalt( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateUpdating::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void FunctionBlockStateStopped::setUp( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> set up" );
	}

	void FunctionBlockStateStopped::start( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> start" );
	}

	bool FunctionBlockStateStopped::tryHalt( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> stop" );
	}

	bool FunctionBlockStateStopped::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateStopped::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void FunctionBlockStateShutDown::setUp( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> set up" );
	}

	void FunctionBlockStateShutDown::start( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> start" );
	}

	bool FunctionBlockStateShutDown::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateShutDown::tryHalt( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> stop" );
	}

	bool FunctionBlockStateShutDown::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void FunctionBlockStateError::setUp( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateError::start( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: error -> start" );
	}

	bool FunctionBlockStateError::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateError::tryHalt( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: error -> stop" );
	}

	bool FunctionBlockStateError::tryStop( FunctionBlockStateManager &states ) const
	{
		return true;
	}

}