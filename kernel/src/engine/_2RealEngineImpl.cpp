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
//#include "engine/_2RealBundle.h"
//#include "engine/_2RealTimer.h"
//#include "engine/_2RealThreadPool.h"
//#include "datatypes/_2RealTypeRegistry.h"
//#include "engine/_2RealLogger.h"
//#include "engine/_2RealSystem.h"
//#include "engine/_2RealFunctionBlock.h"
//#include "helpers/_2RealCallback.h"
//#include "helpers/_2RealEvent.h"

//#include "internal_bundles/_2RealConversionBundle.h"
//#include "datatypes/_2RealImage.h"

#include "helpers/_2RealConstants.h"

using std::string;
using std::ostringstream;

namespace _2Real
{
	//LinkCollection::LinkCollection( EngineImpl *engine ) :
	//	mEngineImpl( engine )
	//{
	//}

	//LinkCollection::~LinkCollection()
	//{
	//	clear();
	//}

	//void LinkCollection::clear()
	//{
	//	for ( LinkIterator it = mLinks.begin(); it != mLinks.end(); ++it )
	//		( *it )->deactivate();
	//	mLinks.clear();
	//}
	//
	//std::shared_ptr< IOLink > LinkCollection::createLink( std::shared_ptr< BasicInletIO > inlet, std::shared_ptr< OutletIO > outlet )
	//{
	//	// find out if those 2 are already linked, if yes, return already existing link
	//	std::shared_ptr< IOLink > dummy( new IOLink( inlet, outlet ) );
	//	LinkIterator it = mLinks.find( dummy );
	//	if ( it != mLinks.end() )
	//		return *it;
	//	dummy.reset();

	//	// try linking, w. conversion if neccessary
	//	std::shared_ptr< IOLink > link = IOLink::link( inlet, outlet );
	//	if ( link.get() != nullptr )
	//	{
	//		mLinks.insert( link );
	//		link->activate();
	//	}

	//	return link;
	//}

	//void LinkCollection::destroyLink( std::shared_ptr< BasicInletIO > inlet, std::shared_ptr< OutletIO > outlet )
	//{
	//	std::shared_ptr< IOLink > dummy( new IOLink( inlet, outlet ) );
	//	LinkIterator it = mLinks.find( dummy );
	//	dummy.reset();

	//	if ( it != mLinks.end() )
	//	{
	//		( *it )->deactivate();
	//		mLinks.erase( it );
	//	}
	//}

	//TimerCollection::TimerCollection( EngineImpl *engine ) :
	//	mEngineImpl( engine ),
	//	mDefaultTimer( new Timer( mEngineImpl, 33, false ) ),
	//	mNullTimer( new Timer( mEngineImpl, ( std::numeric_limits< long >::max )(), false ) )
	//{
	//	mDefaultTimer->start();
	//}

	//TimerCollection::~TimerCollection()
	//{
	//	clear();
	//}

	//void TimerCollection::clear()
	//{
	//	mTimers.clear();
	//}
	//
	//std::shared_ptr< Timer > TimerCollection::createTimer( const unsigned long resolution )
	//{
	//	std::shared_ptr< Timer > timer( new Timer( mEngineImpl, resolution, false ) );
	//	mTimers.insert( timer );
	//	timer->start();
	//	return timer;
	//}

	//std::shared_ptr< Timer > TimerCollection::getDefaultTimer()
	//{
	//	return mDefaultTimer;
	//}

	//std::shared_ptr< Timer > TimerCollection::getNullTimer()
	//{
	//	return mNullTimer;
	//}

	//void TimerCollection::destroyTimer( std::shared_ptr< Timer > timer )
	//{
	//	TimerIterator it = mTimers.find( timer );
	//	if ( it != mTimers.end() )
	//		mTimers.erase( it );
	//}

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

	//void EngineImpl::clearFully()
	//{
	//	mLinkManager->clear();
	//	mSystem->clear();
	//	mBundleManager->clear();
	//}

	//void EngineImpl::clearBlockInstances()
	//{
	//	mLinkManager->clear();
	//	mSystem->clear();
	//}

	//const long EngineImpl::getElapsedTime() const
	//{
	//	return static_cast< long >( mTimestamp.elapsed() );
	//}

// ---------------------------------- bundle loading

	std::shared_ptr< Bundle > EngineImpl::loadLibrary( Path const& path )
	{
		string pathAsString = path.string();

		if ( pathAsString.find( Constants::SharedLibrarySuffix ) == string::npos )
		{
			pathAsString.append( Constants::SharedLibrarySuffix );
		}

		return mBundleCollection->loadBundle( path );
	}

	Path const& EngineImpl::getBundleDirectory() const
	{
		return mBundleCollection->getBundleDirectory();
	}

//// ---------------------------------- bundle loading
//
//	std::shared_ptr< Timer > EngineImpl::addTimer( const double updatesPerSecond )
//	{
//		if ( updatesPerSecond <= 0. )
//			return mTimerManager->getNullTimer();
//
//		double millis = 1000/updatesPerSecond;
//		return mTimerManager->createTimer( static_cast< long >( millis ) );
//	}
//
//	void EngineImpl::removeTimer( std::shared_ptr< Timer > timer )
//	{
//		mTimerManager->destroyTimer( timer );
//	}
//
//// ---------------------------------- exception handling
//
//	void EngineImpl::registerToException( app::BlockExcCallback &callback )
//	{
//		mBlockExceptionEvent.addListener( callback );
//	}
//
//	void EngineImpl::unregisterFromException( app::ContextBlockExcCallback &callback )
//	{
//		mContextBlockExceptionEvent.removeListener( callback );
//	}
//
//	void EngineImpl::registerToException( app::ContextBlockExcCallback &callback )
//	{
//		mContextBlockExceptionEvent.addListener( callback );
//	}
//
//	void EngineImpl::unregisterFromException( app::BlockExcCallback &callback )
//	{
//		mBlockExceptionEvent.removeListener( callback );
//	}
//
//	void EngineImpl::handleException( std::shared_ptr< FunctionBlock > block, Exception const& exception ) const
//	{
//		if ( block->isContext() )
//			mContextBlockExceptionEvent.notify( std::make_pair( exception, app::ContextBlockHandle( block ) ) );
//		else
//			mBlockExceptionEvent.notify( std::make_pair( exception, app::BlockHandle( block ) ) );
//	}

// ---------------------------------- exception handling

}