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

#include "engine/_2RealPostSetupState.h"

namespace _2Real
{
	PostSetupState::PostSetupState( const BlockState state ) :
		AbstractBlockState( state )
	{
	}

// ----- loop

	std::shared_ptr< SignalResponse > PostSetupState::onStartRunning()
	{
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SETUP_RUNNING, false );
	}

	std::shared_ptr< SignalResponse > PostSetupState::onStopRunning()
	{	
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SETUP, false );
	}

	std::shared_ptr< SignalResponse > PostSetupState::onUpdateSignalReceived()
	{
		assert( NULL );
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SETUP, false );
	}

// ----- user input

	std::shared_ptr< SignalResponse > PostSetupState::onSetupSignalReceived()
	{	
		return makeResponse( Action::DO_SETUP, BlockState::POST_SETUP, false );
	}

	std::shared_ptr< SignalResponse > PostSetupState::onSingleUpdateSignalReceived()
	{
		return makeResponse( Action::DO_UPDATE, BlockState::POST_SETUP, false );
	}

	std::shared_ptr< SignalResponse > PostSetupState::onShutdownSignalReceived()
	{
		return makeResponse( Action::DO_SHUTDOWN, BlockState::POST_SHUTDOWN, true );
	}

// ----- shutdown

	std::shared_ptr< SignalResponse > PostSetupState::onEngineShutdownReceived()
	{
		return makeResponse( Action::DO_SHUTDOWN, BlockState::POST_SHUTDOWN, true );
	}
}