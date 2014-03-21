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
#include "engine/_2RealAbstractSharedServiceLifetimeMgr.h"
#include "engine/_2RealAbstractSharedServiceFactory.h"

namespace _2Real
{
	//template< typename TBlock >
	//class SingletonLifetimeMgr_T : public AbstractSharedServiceLifetimeMgr
	//{
	//public:
	//	SingletonLifetimeMgr_T() : mObject( nullptr ), mCreationCount( 0 ) {}

	//	std::weak_ptr< AbstractSharedService > create()
	//	{
	//		if ( 0 == mCreationCount )
	//		{
	//			mObject.reset( new SharedService_T< TBlock > );
	//			++mCreationCount;
	//		}
	//		
	//		return mObject;
	//	}

	//	void destroy( std::weak_ptr< AbstractSharedService > obj )
	//	{
	//		if ( mObject == obj.lock() )
	//			mObject.reset();
	//	}

	//	unsigned int getCreationCount() const
	//	{
	//		return mCreationCount;
	//	}

	//private:

	//	std::shared_ptr< SharedService_T< TBlock > >		mObject;
	//	unsigned int										mCreationCount;

	//};

	//template< typename TBlock >
	//class ServiceLifetimeMgr_T : public AbstractSharedServiceLifetimeMgr
	//{
	//public:
	//	ServiceLifetimeMgr_T() : mCreationCount( 0 ) {}

	//	std::weak_ptr< AbstractSharedService > create()
	//	{
	//		std::shared_ptr< SharedService_T< TBlock > > result( new SharedService_T< TBlock >() );
	//		++mCreationCount;
	//		return result;
	//	}

	//	void destroy( std::weak_ptr< AbstractSharedService > obj )
	//	{
	//		obj.reset();
	//	}

	//	unsigned int getCreationCount() const
	//	{
	//		return mCreationCount;
	//	}

	//private:

	//	unsigned int										mCreationCount;

	//};

	class SingletonLifetimeManager : public AbstractSharedServiceLifetimeManager
	{

	public:

		SingletonLifetimeManager( std::shared_ptr< const AbstractSharedServiceFactory > factory ) : mFactory( factory ), mCreationCount( 0 ) {}

		std::shared_ptr< AbstractSharedService > create( _2Real::bundle::BlockIo const& io, std::vector< std::shared_ptr<_2Real::bundle::AbstractBlock > > const& dependencies )
		{
			if ( 0 == mCreationCount )
			{
				mObject = mFactory->create( io, dependencies );
				++mCreationCount;
			}
			
			return mObject;
		}

		void destroy( std::weak_ptr< AbstractSharedService > obj )
		{
			if ( mObject == obj.lock() )
				mObject.reset();
		}

		unsigned int getCreationCount() const
		{
			return mCreationCount;
		}

	private:

		std::shared_ptr< const AbstractSharedServiceFactory >		mFactory;
		std::shared_ptr< AbstractSharedService >					mObject;
		unsigned int												mCreationCount;

	};

	class ServiceLifetimeManager : public AbstractSharedServiceLifetimeManager
	{

	public:

		ServiceLifetimeManager( std::shared_ptr< const AbstractSharedServiceFactory > factory ) : mFactory( factory ), mCreationCount( 0 ) {}

		std::shared_ptr< AbstractSharedService > create( _2Real::bundle::BlockIo const& io, std::vector< std::shared_ptr<_2Real::bundle::AbstractBlock > > const& dependencies )
		{
			std::shared_ptr< AbstractSharedService > result = mFactory->create( io, dependencies );
			++mCreationCount;
			return result;
		}

		void destroy( std::weak_ptr< AbstractSharedService > obj )
		{
			auto it = mObjects.find( obj.lock() );
			if ( it != mObjects.end() )
				mObjects.erase( it );
		}

		unsigned int getCreationCount() const
		{
			return mCreationCount;
		}

	private:

		std::shared_ptr< const AbstractSharedServiceFactory >		mFactory;
		std::set< std::shared_ptr< AbstractSharedService > >		mObjects;
		unsigned int												mCreationCount;

	};
}