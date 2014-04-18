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
	class UpdateTrigger_I
	{

	public:

		UpdateTrigger_I() = default;
		virtual ~UpdateTrigger_I() = default;

		UpdateTrigger_I( UpdateTrigger_I const& other ) = delete;
		UpdateTrigger_I( UpdateTrigger_I && other ) = delete;
		UpdateTrigger_I& operator=( UpdateTrigger_I const& other ) = delete;
		UpdateTrigger_I& operator=( UpdateTrigger_I && other ) = delete;

		virtual boost::signals2::connection registerToUpdate( boost::signals2::signal< void() >::slot_type ) const = 0;

	};
}
