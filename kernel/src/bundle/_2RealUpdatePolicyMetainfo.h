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
#include "common/_2RealInletPolicy.h"
#include "common/_2RealDefaultUpdatePolicy.h"

namespace _2Real
{
	class UpdatePolicyMetainfoImpl;

	namespace bundle
	{
		class UpdatePolicyMetainfo
		{

		public:

			UpdatePolicyMetainfo( std::shared_ptr< UpdatePolicyMetainfoImpl > );

			/*
			*	sets the default update policy for block instances:
			*	either all inlets must have new data ( DefaultUpdatePolicy::ALL ),
			*	any inlet must have new data ( DefaultUpdatePolicy::ANY ),
			*	or no update based on inlets is going to happen ( DefaultUpdatePolicy::DISABLED )
			*/
			void set( const DefaultUpdatePolicy );

		private:

			std::shared_ptr< UpdatePolicyMetainfoImpl >		mImpl;

		};
	}
}