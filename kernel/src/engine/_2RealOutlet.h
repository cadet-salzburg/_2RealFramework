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
#include "engine/_2RealDataSource.h"
#include "engine/_2RealData.h"

namespace _2Real
{
	class SharedServiceIoSlotMetainfo;

	class Outlet : public DataSource, public std::enable_shared_from_this< Outlet >
	{

	public:

		explicit Outlet( std::shared_ptr< const SharedServiceIoSlotMetainfo > );

		std::string getName() const;

		DataItem & getValue();

		void update();

	private:

		std::shared_ptr< const SharedServiceIoSlotMetainfo >  mMetainfo;

		mutable std::mutex					mMutex;
		std::shared_ptr< DataItem >			mTmpValue;
		std::shared_ptr< DataItem >			mValue;

	};
}