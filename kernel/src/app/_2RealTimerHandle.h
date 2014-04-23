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
#include "common/_2RealSignals.h"

namespace _2Real
{
	class TimerImpl;

	namespace app
	{
		class TimerHandle
		{

		public:

			/*
			*	created by engine via Engine::createTimer
			*/
			explicit TimerHandle( std::shared_ptr< TimerImpl > );

			/*
			*	@return: true if the underlying object is valid ( timers stay valid until TimerHandle::destroy is called or the engine is destroyed )
			*/
			bool			isValid() const;

			/*
			*	starts the timer: should be called after creation
			*/
			void			start();

			/*
			*	stops the timer from updating
			*/
			void			stop();

			/*
			*	allows you to register to the timer signal
			*/
			Connection		registerToTimer( boost::signals2::signal< void() >::slot_type );

			/*
			*	destroys the timer
			*/
			void			destroy();

		private:

			friend class BlockHandle;

			std::weak_ptr< TimerImpl >		mImpl;

		};
	}
}