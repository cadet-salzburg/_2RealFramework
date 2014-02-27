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
#include "engine/_2RealThreadpool.h"
#include "helpers/_2RealConstants.h"

namespace _2Real
{
	std::shared_ptr< EngineImpl > EngineImpl::create()
	{
		std::shared_ptr< EngineImpl > result( new EngineImpl );
		result->privateInit();
		return result;
	}

	EngineImpl::EngineImpl() :
		enable_shared_from_this< EngineImpl >()
	{
		std::cout << "engine created" << std::endl;
	}

	EngineImpl::~EngineImpl()
	{
		try
		{
		//	mLogger->addLine( "ENGINE SHUTDOWN" );
		//	clearFully();
		//	mLogger->addLine( "ENGINE CLEARED" );

		//	delete mTimerManager;
		//	delete mLinkManager;
		//	delete mSystem;
		//	delete mBundleManager;
		//	delete mThreadPool;
		//	delete mTypeRegistry;
		//	delete mTimer;
		//	delete mLogger;
		}
		catch ( std::exception const& e )
		{
			std::cout << e.what() << std::endl;
		}
	}

	void EngineImpl::privateInit()
	{
		// order matters -> see order of member vars

		mThreadsDedicated.reset( new Threadpool( 0, std::numeric_limits< unsigned int >::max() ) );
		mThreads.reset( new Threadpool( 4, 8 ) );

		mBundleCollection.reset( new BundleCollection( mTypeRegistry, mThreads, mThreadsDedicated ) );
		std::cout << "engine initialized" << std::endl;
	}

// ---------------------------------- bundle loading

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
}