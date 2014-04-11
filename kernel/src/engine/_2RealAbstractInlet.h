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

namespace _2Real
{
	class IoSlotMetainfo;
	class InstanceId;
	class Block;

	class AbstractInlet
	{

	public:

		AbstractInlet( std::shared_ptr< Block >, std::shared_ptr< const IoSlotMetainfo >, std::shared_ptr< const InstanceId > );

		AbstractInlet() = delete;
		AbstractInlet( AbstractInlet const& other ) = delete;
		AbstractInlet( AbstractInlet && other ) = delete;
		AbstractInlet& operator=( AbstractInlet const& other ) = delete;
		AbstractInlet& operator=( AbstractInlet && other ) = delete;

		virtual ~AbstractInlet() = default;

		bool								isMultiInlet() const;
		std::shared_ptr< const InstanceId >	getId() const;
		std::shared_ptr< Block >			getParent();

		virtual void update() = 0;

	protected:

		std::weak_ptr< Block >						mParent;
		std::weak_ptr< const IoSlotMetainfo >		mMetainfo;
		std::shared_ptr< const InstanceId >			mId;

	};
}