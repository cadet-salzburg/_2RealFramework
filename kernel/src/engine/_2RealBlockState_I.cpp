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

#include "engine/_2RealBlockState_I.h"
#include "engine/_2RealPreSetupState.h"
#include "engine/_2RealPostSetupState.h"
#include "engine/_2RealPostSetupState_Running.h"
#include "engine/_2RealPostShutdownState.h"

namespace _2Real
{

	std::shared_ptr< BlockState_I > BlockState_I::create( const BlockState id )
	{
		switch ( id )
		{
		case BlockState::PRE_SETUP:
			return std::shared_ptr< BlockState_I >( new PreSetupState( id ) );
		case BlockState::POST_SETUP:
			return std::shared_ptr< BlockState_I >( new PostSetupState( id ) );
		case BlockState::POST_SETUP_RUNNING:
			return std::shared_ptr< BlockState_I >( new PostSetupState_Running( id ) );
		case BlockState::POST_SHUTDOWN:
			return std::shared_ptr< BlockState_I >( new PostShutdownState( id ) );
		default:
			return std::shared_ptr< BlockState_I >();
		}
	}

}
