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
#include "app/_2RealCallback.h"

namespace _2Real
{
	class Block;

	namespace app
	{
		class InletHandle;
		class OutletHandle;
		class TimerHandle;
		class BlockMetainfo;

		class FunctionBlockHandle
		{

		public:

			typedef Callback_T< void >  StateFinalizedCallback;

			FunctionBlockHandle();
			explicit FunctionBlockHandle( std::shared_ptr< Block > );

			bool				isValid() const;
			BlockMetainfo		getMetainfo() const;

			// no further call state change calls will have any effect until the function has been carried out
			void				setup( StateFinalizedCallback const& );
			void				singlestep( StateFinalizedCallback const& );
			void				shutdown( StateFinalizedCallback const& );

			void				startUpdating( TimerHandle );
			//void				startUpdating( UpdatePolicyHandle );

			// again, no further calls will have any effect until the cb has been called!
			void				stopUpdating( StateFinalizedCallback const& cb );

		private:

			std::weak_ptr< Block >		mImpl;

		};
	}
}
