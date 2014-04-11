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

#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealBundleCollection.h"
#include "engine/_2RealTimerCollection.h"
#include "engine/_2RealThreadpoolCollection.h"
#include "engine/_2RealTypeCollection.h"
#include "helpers/_2RealConstants.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealSharedTypeMetainfo.h"

namespace _2Real
{
	EngineImpl::EngineImpl() :
		std::enable_shared_from_this< EngineImpl >(),
		mTimerCollection( new TimerCollection ),
		mThreadpoolCollection( new ThreadpoolCollection ),
		mStdThreads( mThreadpoolCollection->createThreadpool( ThreadpoolPolicy::FIFO ) ),
		mCtxtThreads( mThreadpoolCollection->createThreadpool( ThreadpoolPolicy::DEDICATED ) ),
		mTypeCollection( new TypeCollection ),
		mBundleCollection( new BundleCollection( mTypeCollection, mStdThreads.lock(), mCtxtThreads.lock() ) )
	{
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, ( uint8_t )0 ) );
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, ( int8_t )0 ) );
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, ( uint32_t )0 ) );
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, ( int32_t )0 ) );
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, ( uint64_t )0 ) );
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, ( int64_t )0 ) );
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, 0.0 ) );
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, 0.f ) );
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, false ) );
			mTypeCollection->addType( BasicTypeMetainfo::make( mTypeCollection, std::string() ) );
	}

	EngineImpl::~EngineImpl()
	{
	}

	void EngineImpl::clear()
	{
		mStdThreads.reset();
		mCtxtThreads.reset();

		//mBundleCollection.clear();
		//mTypeCollection.clear();
		//mTimerCollection.clear();
		//mThreadpoolCollection.clear();

		mBundleCollection.reset();
		mTypeCollection.reset();
		mTimerCollection.reset();
		mThreadpoolCollection.reset();
	}

	std::pair< std::shared_ptr< Bundle >, std::shared_ptr< const SharedLibraryMetainfo > > EngineImpl::loadLibrary( std::string const& filename )
	{
		std::string file = filename;
		if ( file.find( Constants::SharedLibrarySuffix ) == std::string::npos )
			file.append( Constants::SharedLibrarySuffix );

		return mBundleCollection->loadBundle( file, mTypeCollection );
	}

	Path const& EngineImpl::getBundleDirectory() const
	{
		return mBundleCollection->getBundleDirectory();
	}

	std::shared_ptr< Timer > EngineImpl::createTimer( const double fps )
	{
		return mTimerCollection->createTimer( fps );
	}

	std::shared_ptr< Threadpool > EngineImpl::createThreadpool( const ThreadpoolPolicy policy )
	{
		return mThreadpoolCollection->createThreadpool( policy );
	}
}