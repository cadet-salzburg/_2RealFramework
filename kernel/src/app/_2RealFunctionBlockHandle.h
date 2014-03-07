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
#include "enums/_2RealBlockState.h"

#include <future>

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

			FunctionBlockHandle();
			explicit FunctionBlockHandle( std::shared_ptr< Block > );

			bool				isValid() const;
			BlockMetainfo		getMetainfo() const;

			/*
			*	none of the following functions should be called while
			*	there's still an unfulfilled std::future object around
			*	( from the same block, of course: different blocks are fine )
			*/

			/*
			*	the functions here are not finalized yet
			*	personally, i really like the std::future based approach
			*	as it allows programmers to block whenever it makes sense for their applications
			*	- yet without further testing it's impossible to tell how useful it is in practice
			*/

			/*
			*	on the subject of futures, maybe a boolean success / failure result would be more useful
			*	than the block state: from the enum, you can't tell if anything was done to the block...
			*/

			std::future< BlockState >		setup();
			std::future< BlockState >		singlestep();
			std::future< BlockState >		shutdown();
			std::future< BlockState >		startUpdating( TimerHandle );
			//std::future< BlockState >		startUpdating( UpdatePolicyHandle );
			std::future< BlockState >		stopUpdating();

			/*
				old functions: these were based on async callbacks
			*/
			/*
			void				setup( std::function< void() > const& );
			void				singlestep( std::function< void() > const& );
			void				shutdown( std::function< void() > const& );
			void				startUpdating( TimerHandle );
			void				startUpdating( UpdatePolicyHandle );
			void				stopUpdating( std::function< void() > const& );
			*/

		private:

			std::weak_ptr< Block >		mImpl;

		};
	}
}
