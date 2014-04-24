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
#include "engine/_2RealDataSource_I.h"
#include "common/_2RealData.h"

namespace _2Real
{
	class IoSlotMetainfoImpl;
	class InstanceId;
	class BlockImpl;

	class OutletImpl : public DataSource_I, public std::enable_shared_from_this< OutletImpl >
	{

	public:

		static std::shared_ptr< OutletImpl > createFromMetainfo( std::shared_ptr< BlockImpl >, std::shared_ptr< const IoSlotMetainfoImpl > );

		OutletImpl() = delete;
		OutletImpl( OutletImpl const& other ) = delete;
		OutletImpl( OutletImpl && other ) = delete;
		OutletImpl& operator=( OutletImpl const& other ) = delete;
		OutletImpl& operator=( OutletImpl && other ) = delete;

		~OutletImpl() = default;

		DataItem & getValue();
		std::shared_ptr< const DataItem > getData() const;
		void update();

		std::shared_ptr< const IoSlotMetainfoImpl > getMetainfo() const;
		std::shared_ptr< const InstanceId > getId() const;
		std::shared_ptr< BlockImpl >		getParent();

		void unlink();

		boost::signals2::connection registerToUpdate( boost::signals2::signal< void( std::shared_ptr< const DataItem > )>::slot_type ) const;
		boost::signals2::connection registerToRemoved( boost::signals2::signal< void( std::shared_ptr< const DataSource_I > )>::slot_type ) const;

	private:

		OutletImpl( std::shared_ptr< BlockImpl >, std::shared_ptr< const IoSlotMetainfoImpl >, std::shared_ptr< const InstanceId > );

		std::weak_ptr< BlockImpl >						mParent;
		std::weak_ptr< const IoSlotMetainfoImpl >		mMetainfo;
		std::shared_ptr< const InstanceId >			mId;

		mutable std::mutex							mMutex;
		std::shared_ptr< DataItem >					mTmpValue;
		std::shared_ptr< DataItem >					mValue;

		mutable boost::signals2::signal< void( std::shared_ptr< const DataItem > ) >		mUpdated;
		mutable boost::signals2::signal< void( std::shared_ptr< const DataSource_I > ) >	mRemoved;

	};
}