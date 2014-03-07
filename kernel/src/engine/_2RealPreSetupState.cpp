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

#include "engine/_2RealPreSetupState.h"

namespace _2Real
{
	PreSetupState::PreSetupState( const BlockState state ) :
		AbstractBlockState( state )
	{
	}

// ----- loop

	std::shared_ptr< SignalResponse > PreSetupState::onStartRunning()
	{
		return makeResponse( Action::DO_NOTHING, BlockState::PRE_SETUP, false );
	}

	std::shared_ptr< SignalResponse > PreSetupState::onStopRunning()
	{	
		return makeResponse( Action::DO_NOTHING, BlockState::PRE_SETUP, false );
	}

	std::shared_ptr< SignalResponse > PreSetupState::onUpdateSignalReceived()
	{
		assert( NULL );
		return makeResponse( Action::DO_NOTHING, BlockState::PRE_SETUP, false );
	}

// ----- user input

	std::shared_ptr< SignalResponse > PreSetupState::onSetupSignalReceived()
	{	
		// valid -> post setup state
		return makeResponse( Action::DO_SETUP, BlockState::POST_SETUP, false );
	}

	std::shared_ptr< SignalResponse > PreSetupState::onSingleUpdateSignalReceived()
	{
		return makeResponse( Action::DO_NOTHING, BlockState::PRE_SETUP, false );
	}

	std::shared_ptr< SignalResponse > PreSetupState::onShutdownSignalReceived()
	{
		// no setup -> no shutdown needed
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SHUTDOWN, true );
	}

// ----- shutdown

	std::shared_ptr< SignalResponse > PreSetupState::onEngineShutdownReceived()
	{
		// no setup -> no shutdown needed
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SHUTDOWN, true );
	}
}
