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
#include "engine/_2RealBlockState_I.h"

namespace _2Real
{
	class PreSetupState : public BlockState_I
	{

	public:

		PreSetupState( const BlockState );

		std::shared_ptr< SignalResponse > onStartRunning() override;
		std::shared_ptr< SignalResponse > onStopRunning() override;
		std::shared_ptr< SignalResponse > onUpdateSignalReceived() override;
		std::shared_ptr< SignalResponse > onSetupSignalReceived() override;
		std::shared_ptr< SignalResponse > onSingleUpdateSignalReceived() override;
		std::shared_ptr< SignalResponse > onShutdownSignalReceived() override;
		std::shared_ptr< SignalResponse > onEngineShutdownReceived() override;

		BlockState getId() const { return mId; }

	private:

		const BlockState	mId;

	};
}