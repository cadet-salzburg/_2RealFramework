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
#include "engine/_2RealBundleManager.h"
#include "engine/_2RealBundle.h"
#include "engine/_2RealTimer.h"
#include "engine/_2RealThreadPool.h"
#include "datatypes/_2RealTypeRegistry.h"
#include "engine/_2RealLogger.h"
#include "engine/_2RealSystem.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealCallback.h"
#include "helpers/_2RealEvent.h"
#include "helpers/_2RealSingletonHolder.h"
#include "helpers/_2RealHelpers.h"

#include "internal_bundles/_2RealConversionBundle.h"
#include "datatypes/_2RealImage.h"

#include <sstream>
#include <iostream>

#ifdef _2REAL_WINDOWS
	#ifndef _DEBUG
		#define shared_library_suffix "_32.dll"
	#else
		#define shared_library_suffix "_32d.dll"
	#endif
#elif _2REAL_UNIX
	#ifndef _DEBUG
		#define shared_library_suffix ".so"
	#else
		#define shared_library_suffix "_d.so"
	#endif
#elif _2REAL_MAC
	#ifndef _DEBUG
		#define shared_library_suffix ".dylib"
	#else
		#define shared_library_suffix "_d.dylib"
	#endif
#endif

using std::string;
using std::ostringstream;

namespace _2Real
{
	LinkCollection::LinkCollection( EngineImpl *engine ) :
		mEngineImpl( engine )
	{
	}

	LinkCollection::~LinkCollection()
	{
		clear();
	}

	void LinkCollection::clear()
	{
		for ( LinkIterator it = mLinks.begin(); it != mLinks.end(); ++it )
			( *it )->deactivate();
		mLinks.clear();
	}
	
	std::shared_ptr< IOLink > LinkCollection::createLink( std::shared_ptr< BasicInletIO > inlet, std::shared_ptr< OutletIO > outlet )
	{
		// find out if those 2 are already linked, if yes, return already existing link
		std::shared_ptr< IOLink > dummy( new IOLink( inlet, outlet ) );
		LinkIterator it = mLinks.find( dummy );
		if ( it != mLinks.end() )
			return *it;
		dummy.reset();

		// try linking, w. conversion if neccessary
		std::shared_ptr< IOLink > link = IOLink::link( inlet, outlet );
		if ( link.get() != nullptr )
		{
			mLinks.insert( link );
			link->activate();
		}

		return link;
	}

	EngineImpl::EngineImpl() :
		mLogger( new Logger( "EngineLog.txt" ) ),
		mTimer( new Timer( mLogger ) ),
		mTypeRegistry( new TypeRegistry ),
		mThreadPool( new ThreadPool( this, 1, 0, "2Real threadpool" ) ),
		mBundleManager( new BundleManager( this ) ),
		mSystem( new System( this ) ),
		mLinkManager( new LinkCollection( this ) )
	{
		mTimestamp.update();
		mTypeRegistry->registerType( TypeRegistry::sFrameworkTypes, Image::TYPENAME, Image::getTypeMetadata(), new Deleter< TypeMetadata > );
	}

	EngineImpl::~EngineImpl()
	{
		try
		{
			mLogger->addLine( "ENGINE SHUTDOWN" );
			clearFully();
			mLogger->addLine( "ENGINE CLEARED" );

			delete mLinkManager;
			delete mSystem;
			delete mBundleManager;
			delete mThreadPool;
			delete mTypeRegistry;
			delete mTimer;
			delete mLogger;
		}
		catch ( std::exception const& e )
		{
			std::cout << e.what() << std::endl;
		}
	}

	void EngineImpl::clearFully()
	{
		mLinkManager->clear();
		mSystem->clear();
		mBundleManager->clear();
	}

	void EngineImpl::clearBlockInstances()
	{
		mLinkManager->clear();
		mSystem->clear();
	}

	const long EngineImpl::getElapsedTime() const
	{
		return static_cast< long >( mTimestamp.elapsed() );
	}

// ---------------------------------- bundle loading

	std::shared_ptr< Bundle > EngineImpl::loadLibrary( string const& libraryPath )
	{
		string path = libraryPath;

		if ( path.find( shared_library_suffix ) == string::npos )
		{
			path.append( shared_library_suffix );
		}
		return mBundleManager->loadBundle( path );
	}

	std::string EngineImpl::getBundleDirectory() const
	{
		return mBundleManager->getBundleDirectory();
	}

// ---------------------------------- bundle loading

// ---------------------------------- exception handling

	void EngineImpl::registerToException( app::BlockExcCallback &callback )
	{
		mBlockExceptionEvent.addListener( callback );
	}

	void EngineImpl::unregisterFromException( app::ContextBlockExcCallback &callback )
	{
		mContextBlockExceptionEvent.removeListener( callback );
	}

	void EngineImpl::registerToException( app::ContextBlockExcCallback &callback )
	{
		mContextBlockExceptionEvent.addListener( callback );
	}

	void EngineImpl::unregisterFromException( app::BlockExcCallback &callback )
	{
		mBlockExceptionEvent.removeListener( callback );
	}

	void EngineImpl::handleException( app::BlockHandle &block, Exception const& exception ) const
	{
		mBlockExceptionEvent.notify( make_pair( exception, block ) );
	}

	void EngineImpl::handleException( app::ContextBlockHandle &block, Exception const& exception ) const
	{
		mContextBlockExceptionEvent.notify( make_pair( exception, block ) );
	}

// ---------------------------------- exception handling

}