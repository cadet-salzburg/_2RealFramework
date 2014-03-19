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
#include "helpers/_2RealEvent_T.h"

namespace _2Real
{
	/*
	*	base class for timer & update policy
	*/

	class UpdateTrigger
	{

	public:

		virtual ~UpdateTrigger();

		void registerToUpdate( std::shared_ptr< AbstractCallback_T< void > > );
		void unregisterFromUpdate( std::shared_ptr< AbstractCallback_T< void > > );

	protected:

		UpdateTrigger();
		void fire();

	private:

		UpdateTrigger( UpdateTrigger const& other ) = delete;
		UpdateTrigger& operator=( UpdateTrigger const& other ) = delete;

		Event_T< void >			mEvent;

		/*Signal_T< std::mutex, void >		mTrigger;*/

	};
}
