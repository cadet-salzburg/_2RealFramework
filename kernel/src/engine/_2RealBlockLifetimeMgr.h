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
#include "engine/_2RealBlockLifetimeMgr_I.h"
#include "engine/_2RealBlockFactory_I.h"

namespace _2Real
{
	class SingletonLifetimeManager : public BlockLifetimeMgr_I
	{

	public:

		SingletonLifetimeManager( std::shared_ptr< const BlockFactory_I > factory ) : mFactory( factory ), mCreationCount( 0 ) {}

		std::shared_ptr< bundle::Block > create( bundle::BlockIo const& io, std::vector< std::shared_ptr< bundle::Block > > const& dependencies )
		{
			if ( 0 == mCreationCount )
			{
				mObject = mFactory->create( io, dependencies );
				++mCreationCount;
			}
			
			return mObject;
		}

		void destroy( std::weak_ptr< bundle::Block > obj )
		{
			if ( mObject == obj.lock() )
				mObject.reset();
		}

		unsigned int getCreationCount() const
		{
			return mCreationCount;
		}

	private:

		std::shared_ptr< const BlockFactory_I >			mFactory;
		std::shared_ptr< bundle::Block >				mObject;
		unsigned int									mCreationCount;

	};

	class BlockLifetimeManager : public BlockLifetimeMgr_I
	{

	public:

		BlockLifetimeManager( std::shared_ptr< const BlockFactory_I > factory ) : mFactory( factory ), mCreationCount( 0 ) {}

		std::shared_ptr< bundle::Block > create( bundle::BlockIo const& io, std::vector< std::shared_ptr< bundle::Block > > const& dependencies )
		{
			std::shared_ptr< bundle::Block > result = mFactory->create( io, dependencies );
			++mCreationCount;
			return result;
		}

		void destroy( std::weak_ptr< bundle::Block > obj )
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

		std::shared_ptr< const BlockFactory_I >				mFactory;
		std::set< std::shared_ptr< bundle::Block > >		mObjects;
		unsigned int										mCreationCount;

	};
}