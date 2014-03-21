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
#include "app/_2RealAbstractInletHandle.h"
#include "engine/_2RealData.h"

namespace _2Real
{
	class Inlet;

	namespace app
	{
		class InletHandle : public AbstractInletHandle
		{

		public:

			InletHandle();
			explicit InletHandle( std::shared_ptr< Inlet > );

			// use to check if underlying bundle is still valid ( =loaded ) if you're unsure
			bool					isValid() const;

			std::string	const&		getDescription() const;

			// hm. set value function:
			// this should definitely throw an exception in case of a datatype mismatch
			void setValue( DataItem const& value );



		private:

			std::weak_ptr< Inlet >		mImpl;

		};
	}
}
