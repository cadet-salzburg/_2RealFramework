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
#include "common/_2RealData.h"

#include "app/_2RealInletHandle_I.h"

namespace _2Real
{
	class InletImpl;

	namespace app
	{
		class OutletHandle;
		class LinkHandle;

		class InletHandle : public InletHandle_I
		{

		public:

			explicit InletHandle( std::shared_ptr< InletImpl > );
			
			/*
			*	@return: true if underlying object is valid ( all io slots are destroyed when the parent block is destroyed )
			*/
			bool				isValid() const;

			/*
			*	@return: always true
			*/
			bool				isMultiInlet() const;

			/*
			*	@return: handle to parent block
			*/
			BlockHandle			getBlock();

			/*
			*	set inlet value - will overwrite any other set value
			*/
			void				setValue( std::shared_ptr< const DataItem > );

		private:

			friend class MultiInletHandle;
			friend class Engine;

			std::weak_ptr< InletImpl >		mImpl;

		};
	}
}
