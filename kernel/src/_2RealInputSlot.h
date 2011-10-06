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

#include "_2RealIOSlot.h"

namespace _2Real
{

	/**
	*	represents an input slot of a service
	*/

	class OutputSlot;

	class InputSlot : public IOSlot
	{

	public:

		InputSlot(Id *const id, ServiceContainer *const service, std::string const& type);
		InputSlot(InputSlot const& src);
		InputSlot& operator=(InputSlot const& src);
		~InputSlot();

		/**
		*	return sender id
		*/
		OutputSlot *const		linkedOutput();

		/**
		*	link with other slot
		*/
		void					linkWith(OutputSlot *const output);

		/**
		*	get value from any
		*/
		void					extractFrom(IOSlot::SharedAny const& any);

		/**
		*	breaks IO linkage
		*/
		void					reset();

	};

}