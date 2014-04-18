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

namespace _2Real
{
	struct SignalResponse;

	class BlockState_I
	{

	public:

		BlockState_I() = default;
		virtual ~BlockState_I() = default;

		BlockState_I( BlockState_I const& other ) = delete;
		BlockState_I( BlockState_I && other ) = delete;
		BlockState_I& operator=( BlockState_I const& other ) = delete;
		BlockState_I& operator=( BlockState_I && other ) = delete;

		virtual bool operator==( const BlockState ) const = 0;
		virtual std::shared_ptr< SignalResponse > nextAction( std::deque< std::shared_ptr< SignalResponse > > & ) = 0;

		virtual std::shared_ptr< SignalResponse > onStartRunning() = 0;
		virtual std::shared_ptr< SignalResponse > onStopRunning() = 0;
		virtual std::shared_ptr< SignalResponse > onUpdateSignalReceived() = 0;
		virtual std::shared_ptr< SignalResponse > onSetupSignalReceived() = 0;
		virtual std::shared_ptr< SignalResponse > onSingleUpdateSignalReceived() = 0;
		virtual std::shared_ptr< SignalResponse > onShutdownSignalReceived() = 0;
		virtual std::shared_ptr< SignalResponse > onEngineShutdownReceived() = 0;

	};
}