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

#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealStdIncludes.h"
#include "engine/_2RealTimestampedData.h"

namespace _2Real
{
	struct ValidData
	{
		bool operator()( TimestampedData const& newData, TimestampedData const& oldData ) const
		{
			// nullptr in timestamped data -> not valid, return false
			return ( newData.value.get() != nullptr );
		}
	};

	struct NewerTimestamp
	{
		bool operator()( TimestampedData const& newData, TimestampedData const& oldData ) const
		{
			return ( newData.timestamp > oldData.timestamp );
		}
	};

	class ADataCondition
	{
	public:
		virtual ~ADataCondition() {}
		virtual bool canFulfill( TimestampedData const& ) const = 0;
		virtual bool tryFulfill( TimestampedData const&, const bool ) = 0;
		virtual bool isFulfilled() const = 0;
		virtual void setFulfilled( const bool ) = 0;
	protected:
		mutable Poco::FastMutex		mAccess;
		bool						mIsFulfilled;
	};

	template< typename TCondition >
	class DataCondition : public ADataCondition
	{

	public:

		DataCondition() :
			mIsFulfilled( false )
		{
		}

		bool tryFulfill( TimestampedData const& data, const bool reTry )
		{
			// if retry flag -> ignore cond state ( i.e., recheck if already fulfilled )
			if ( ( reTry || !isFulfilled() ) && mCondition( data, mLastData ) )
			{
				setFulfilled( true );
				mLastData = data;
				return true;
			}
			else return false;
		}

		bool canFulfill( TimestampedData const& data ) const
		{
			return mCondition( data, mLastData );
		}

		bool isFulfilled() const
		{
			Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
			return mIsFulfilled;
		}

		void setFulfilled( const bool fulfilled )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
			mIsFulfilled = fulfilled;
		}

	private:

		mutable Poco::FastMutex		mAccess;
		bool						mIsFulfilled;
		TCondition					mCondition;
		TimestampedData				mLastData;

	};

}