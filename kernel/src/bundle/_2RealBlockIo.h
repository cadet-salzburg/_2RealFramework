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

#include <vector>
#include "bundle/_2RealAbstractInletHandle.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealMultiInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "bundle/_2RealParameterHandle.h"

namespace _2Real
{
	namespace bundle
	{
		/*
		*	holds all io slot handles, in the same order they were added to the block
		*/

		class BlockIo
		{

		public:

			std::vector< ParameterHandle * >		mParameters;
			std::vector< AbstractInletHandle * >	mInlets;
			std::vector< OutletHandle *>			mOutlets;

		};
	}
}