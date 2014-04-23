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

#pragma once

#include "common/_2RealStdIncludes.h"
#include "common/_2RealBlockState.h"
#include "common/_2RealBlockAction.h"
#include "common/_2RealBlockResult.h"
#include "common/_2RealBlockRequest.h"

namespace _2Real
{
	class UpdateTrigger_I;	

	struct SignalResponse
	{
		SignalResponse() :
			request( BlockRequest::UNKNOWN ), action( BlockAction::DO_NOTHING ), followupState( BlockState::UNKNOWN ), shouldBeQueued( false ), result(), updateTrigger( nullptr )
		{}

		BlockRequest						request;
		BlockAction							action;
		BlockState							followupState;
		bool								shouldBeQueued;
		std::promise< BlockResult >			result;
		std::shared_ptr< UpdateTrigger_I >	updateTrigger;

		static std::shared_ptr< SignalResponse > makeResponse( const BlockRequest req, const BlockAction action, const BlockState state, const bool queue )
		{
			std::shared_ptr< SignalResponse > response( new SignalResponse );
			response->request = req;
			response->action = action;
			response->followupState = state;
			response->shouldBeQueued = queue;
			response->updateTrigger = nullptr;
			return response;
		}
	};
}