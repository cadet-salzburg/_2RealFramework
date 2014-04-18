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
#include "engine/_2RealSignalResponse.h"

namespace _2Real
{
	std::shared_ptr< SignalResponse > PostShutdownState::nextAction( std::deque< std::shared_ptr< SignalResponse > > &responses )
	{
		if ( responses.empty() ) return nullptr;

		std::shared_ptr< SignalResponse > response;
		for ( auto it : responses )
			it->result.set_value( BlockResult::IGNORED );
		responses.clear();
		return response;
	}

	PostShutdownState::PostShutdownState( const BlockState state ) :
		mId( state )
	{
	}

	bool PostShutdownState::operator==( const BlockState state ) const
	{
		return mId == state;
	}

// ----- loop

	std::shared_ptr< SignalResponse > PostShutdownState::onStartRunning()
	{
		return SignalResponse::makeResponse( BlockRequest::START, BlockAction::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

	std::shared_ptr< SignalResponse > PostShutdownState::onStopRunning()
	{	
		return SignalResponse::makeResponse( BlockRequest::STOP, BlockAction::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

	std::shared_ptr< SignalResponse > PostShutdownState::onUpdateSignalReceived()
	{
		assert( NULL );
		return SignalResponse::makeResponse( BlockRequest::UPDATE, BlockAction::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

// ----- user input

	std::shared_ptr< SignalResponse > PostShutdownState::onSetupSignalReceived()
	{	
		return SignalResponse::makeResponse( BlockRequest::SETUP, BlockAction::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

	std::shared_ptr< SignalResponse > PostShutdownState::onSingleUpdateSignalReceived()
	{
		return SignalResponse::makeResponse( BlockRequest::SINGLESTEP, BlockAction::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

	std::shared_ptr< SignalResponse > PostShutdownState::onShutdownSignalReceived()
	{
		return SignalResponse::makeResponse( BlockRequest::SHUTDOWN, BlockAction::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}

// ----- shutdown

	std::shared_ptr< SignalResponse > PostShutdownState::onEngineShutdownReceived()
	{
		return SignalResponse::makeResponse( BlockRequest::SHUTDOWN, BlockAction::DO_NOTHING, BlockState::POST_SHUTDOWN, false );
	}
}