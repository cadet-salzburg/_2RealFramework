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
		//mLogger( new Logger( "EngineLog.txt" ) ),
		//mTimer( new Timer( this, 1, false ) ),
		//mTypeRegistry( new TypeRegistry ),
		//mThreadPool( new ThreadPool( this, 10, 0, "" ) ),
		//mBundleCollection( new BundleCollection( /*shared_from_this()*/ ) )
		//mSystem( new System( this ) ),
		//mLinkManager( new LinkCollection( this ) ),
		//mTimerManager( new TimerCollection( this ) )
	{
		//mTimestamp.update();
		//mTypeRegistry->registerType( Image::getTypeMetadata() );
		//mTimer->start();

		std::cout << "engine created" << std::endl;
	}

	EngineImpl::~EngineImpl()
	{
		//try
		//{
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
		//}
		//catch ( std::exception const& e )
		//{
		//	std::cout << e.what() << std::endl;
		//}
	}

	void EngineImpl::privateInit()
	{
		// order matters -> see order of member vars
		mBundleCollection.reset( new BundleCollection( mTypeRegistry ) );
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