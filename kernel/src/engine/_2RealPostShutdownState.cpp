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

#include "engine/_2RealPostShutdownState.h"

namespace _2Real
{
	PostShutdownState::PostShutdownState( const BlockState state ) :
		AbstractBlockState( state )
	{
	}

// ----- loop

	std::shared_ptr< SignalResponse > PostShutdownState::onStartRunning()
	{
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

	std::shared_ptr< SignalResponse > PostShutdownState::onStopRunning()
	{	
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

	std::shared_ptr< SignalResponse > PostShutdownState::onUpdateSignalReceived()
	{
		assert( NULL );
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

// ----- user input

	std::shared_ptr< SignalResponse > PostShutdownState::onSetupSignalReceived()
	{	
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

	std::shared_ptr< SignalResponse > PostShutdownState::onSingleUpdateSignalReceived()
	{
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

	std::shared_ptr< SignalResponse > PostShutdownState::onShutdownSignalReceived()
	{
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

// ----- shutdown

	std::shared_ptr< SignalResponse > PostShutdownState::onEngineShutdownReceived()
	{
		return makeResponse( Action::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}
}