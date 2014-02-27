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

#include "engine/_2RealAbstractBlockState.h"
#include "engine/_2RealPreSetupState.h"
#include "engine/_2RealPostSetupState.h"
#include "engine/_2RealPostSetupState_Running.h"
#include "engine/_2RealPostShutdownState.h"

namespace _2Real
{
	std::shared_ptr< AbstractBlockState > AbstractBlockState::create( const State::Id id )
	{
		switch ( id )
		{
		case State::PRE_SETUP:
			return std::shared_ptr< AbstractBlockState >( new PreSetupState( id ) );
		case State::POST_SETUP:
			return std::shared_ptr< AbstractBlockState >( new PostSetupState( id ) );
		case State::POST_SETUP_RUNNING:
			return std::shared_ptr< AbstractBlockState >( new PostSetupState_Running( id ) );
		case State::POST_SHUTDOWN:
			return std::shared_ptr< AbstractBlockState >( new PostShutdownState( id ) );
		default:
			return std::shared_ptr< AbstractBlockState >( new PostShutdownState( id ) );
		}
	}

	AbstractBlockState::~AbstractBlockState()
	{
	}

	State::Id AbstractBlockState::getId() const
	{
		return mId;
	}

	AbstractBlockState::AbstractBlockState( const State::Id id ) :
		std::enable_shared_from_this< AbstractBlockState >(),
		mId( id )
	{
	}
}
