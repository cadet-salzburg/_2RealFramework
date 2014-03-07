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
#include "helpers/_2RealConstants.h"
#include "helpers/_2RealException.h"

namespace _2Real
{
	EngineImpl::EngineImpl() :
		std::enable_shared_from_this< EngineImpl >(),
		mThreadpoolCollection( new ThreadpoolCollection ),
		mStdThreads( mThreadpoolCollection->createThreadpool( ThreadpoolPolicy::FIFO ) ),
		mCtxtThreads( mThreadpoolCollection->createThreadpool( ThreadpoolPolicy::DEDICATED ) ),
		mTypeCollection( nullptr ),
		mBundleCollection( new BundleCollection( mTypeCollection, mStdThreads.lock(), mCtxtThreads.lock() ) ),
		mTimerCollection( new TimerCollection )
	{
		std::cout << "engine constructor: done" << std::endl;
	}

	EngineImpl::~EngineImpl()
	{
		mBundleCollection.reset();
		mTypeCollection.reset();
		mThreadpoolCollection.reset();

		std::cout << "engine destructor: done" << std::endl;
	}

	std::shared_ptr< Bundle > EngineImpl::loadLibrary( std::string const& filename )
	{
		std::string file = filename;
		if ( file.find( Constants::SharedLibrarySuffix ) == std::string::npos )
			file.append( Constants::SharedLibrarySuffix );

		return mBundleCollection->loadBundle( file );
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