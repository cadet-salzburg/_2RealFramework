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

#include "helpers/_2RealStdIncludes.h"
#include "engine/_2RealStateData.h"

#include "enums/_2RealBlockState.h"

namespace _2Real
{
	struct SignalResponse;

	class AbstractBlockState : public std::enable_shared_from_this< AbstractBlockState >
	{

	public:

		static std::shared_ptr< AbstractBlockState > create( const BlockState );

		virtual ~AbstractBlockState();

		BlockState getId() const;

		virtual std::shared_ptr< SignalResponse > onStartRunning() = 0;
		virtual std::shared_ptr< SignalResponse > onStopRunning() = 0;
		virtual std::shared_ptr< SignalResponse > onUpdateSignalReceived() = 0;
		virtual std::shared_ptr< SignalResponse > onSetupSignalReceived() = 0;
		virtual std::shared_ptr< SignalResponse > onSingleUpdateSignalReceived() = 0;
		virtual std::shared_ptr< SignalResponse > onShutdownSignalReceived() = 0;
		virtual std::shared_ptr< SignalResponse > onEngineShutdownReceived() = 0;

	protected:

		explicit AbstractBlockState( const BlockState );

		static std::shared_ptr< SignalResponse > makeResponse( const Action, const BlockState, const bool );
	
		BlockState mId;

	private:

		AbstractBlockState( AbstractBlockState const& other ) = delete;
		AbstractBlockState& operator=( AbstractBlockState const& other ) = delete;

	};
}