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
#include "app/_2RealBlockHandle.h"
#include "enums/_2RealBlockState.h"

#include <future>

namespace _2Real
{
	namespace app
	{
		class TimerHandle;
		class UpdatePolicyHandle;
		class BlockIo;

		class FunctionBlockHandle : public BlockHandle
		{

		public:

			explicit FunctionBlockHandle( std::shared_ptr< Block > );

			using BlockHandle::isValid;
			using BlockHandle::isSingleton;
			using BlockHandle::getBundle;

			std::future< BlockState >		setup();
			std::future< BlockState >		singlestep();
			std::future< BlockState >		shutdown();
			// go to run state & start listening to a timer
			std::future< BlockState >		startUpdating( TimerHandle );
			// go to run state & start listening to the block's update policy
			std::future< BlockState >		startUpdating();
			std::future< BlockState >		stopUpdating();

			UpdatePolicyHandle				getUpdatePolicy();

			// --------- io slots

			BlockIo getBlockIo();

		};
	}
}
