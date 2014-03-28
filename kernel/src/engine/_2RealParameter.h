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
#include "engine/_2RealInSlot.h"

namespace _2Real
{
	class SharedServiceInletMetainfo;

	class Parameter : private InSlot, public std::enable_shared_from_this< Parameter >
	{

	public:

		explicit Parameter( std::shared_ptr< const SharedServiceInletMetainfo > );

		std::string const& getName() const;
		//std::shared_ptr< const SharedServiceInletMetainfo > getMetainfo() const;

		using InSlot::update;
		using InSlot::getValue;

	private:

		std::shared_ptr< const SharedServiceInletMetainfo >	mMetainfo;

	};
}