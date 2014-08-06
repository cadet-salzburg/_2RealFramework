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

#include <sdkddkver.h>

// removes silly warning related to assignment operator
#pragma warning(disable: 4512)
// removes silly warning related to unreachable code in boost/signals2/detail/variadic_slot_invoker.hpp
#pragma warning(disable: 4702)
// removes silly warning related to unused args in boost/signals2/detail/variadic_slot_invoker.hpp
#pragma warning(disable: 4100)
#pragma warning(disable: 4503)