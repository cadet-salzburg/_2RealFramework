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

	EngineImpl::EngineImpl() :
		mLogger( new Logger( "EngineLog.txt" ) ),
		mTimer( new Timer( *mLogger ) ),
		mTypeRegistry( new TypeRegistry ),
		mThreadPool( new ThreadPool( *this, 12, 0, "2Real threadpool" ) ),
		mBundleManager( new BundleManager( this ) ),
		mSystem( new System( *mLogger ) ),
		mTimestamp( new Poco::Timestamp )
	{
		mTimestamp->update();
		mTypeRegistry->registerType( TypeRegistry::sFrameworkTypes, Image::TYPENAME, Image::getTypeMetadata(), new Deleter< TypeMetadata > );
	}

	EngineImpl::~EngineImpl()
	{
		try
		{
			mLogger->addLine( "ENGINE SHUTDOWN" );
			clearFully();
			delete mSystem;
			delete mBundleManager;
			delete mThreadPool;
			delete mTypeRegistry;
			mLogger->stop();
			delete mLogger;
			delete mTimer;
			delete mTimestamp;
		}
		catch ( std::exception &e )
		{
			std::cout << e.what() << std::endl;
		}
	}

	void EngineImpl::clearFully()
	{
		for ( EngineImpl::LinkIterator it = mLinks.begin(); it != mLinks.end(); ++it )
		{
			( *it )->deactivate();
			delete *it;
		}
		mLinks.clear();
		mSystem->clearAll();
		mBundleManager->clear();
	}

	void EngineImpl::clearBlockInstances()
	{
		for ( EngineImpl::LinkIterator it = mLinks.begin(); it != mLinks.end(); ++it )
		{
			( *it )->deactivate();
			delete *it;
		}
		mLinks.clear();
		mSystem->clearBlockInstances();
	}

	void EngineImpl::addBlock( FunctionBlock< app::BlockHandle > &block )
	{
		mSystem->addBlock( block );
	}

	void EngineImpl::removeBlock( FunctionBlock< app::BlockHandle > &block, const long timeout )
	{
		Bundle *b = block.getOwningBundle();
		b->removeBlockInstance( block );
		for ( LinkIterator it = mLinks.begin(); it != mLinks.end(); )
		{
			if ( ( *it )->isBlockInvolved( block ) )
			{
				( *it )->deactivate();
				delete *it;
				it = mLinks.erase( it );
			}
			else ++it;
		}
		mSystem->removeBlock( block, timeout );

		if ( b->hasContext() && b->getBlockInstances( *mBundleManager ).empty() )
		{
			FunctionBlock< app::ContextBlockHandle > & context = b->getContextBlock( *mBundleManager );
			mSystem->removeBlock( context, timeout );
			b->contextBlockRemoved();
		}
	}

	void EngineImpl::addBlock( FunctionBlock< app::ContextBlockHandle > &block )
	{
		mSystem->addBlock( block );
	}

	void EngineImpl::removeBlock( FunctionBlock< app::ContextBlockHandle > &block, const long timeout )
	{
		for ( LinkIterator it = mLinks.begin(); it != mLinks.end(); )
		{
			if ( ( *it )->isBlockInvolved( block ) )
			{
				( *it )->deactivate();
				delete *it;
				it = mLinks.erase( it );
			}
			else ++it;
		}
		mSystem->removeBlock( block, timeout );
	}

	const long EngineImpl::getElapsedTime() const
	{
		return static_cast< long >( mTimestamp->elapsed() );
	}

// ---------------------------------- bundle loading

	Bundle & EngineImpl::loadLibrary( string const& libraryPath )
	{
		string path = libraryPath;

		if ( path.find( shared_library_suffix ) == string::npos )
		{
			path.append( shared_library_suffix );
		}
		return mBundleManager->loadLibrary( path );
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

	//EngineImpl::Links const& EngineImpl::getCurrentLinks() const
	//{
	//	return m_Links;
	//}

	//EngineImpl::BlockInstances EngineImpl::getCurrentBlockInstances() const
	//{
	//	System::Blocks const& blocks = m_System->getBlockInstances();
	//	EngineImpl::BlockInstances result;

	//	for ( System::BlockConstIterator it = blocks.begin(); it != blocks.end(); ++it )
	//	{
	//		FunctionBlock< app::BlockHandle > *instance = static_cast< FunctionBlock< app::BlockHandle > * >( *it );
	//		result.push_back( instance );
	//	}
	//	return result;
	//}

	//EngineImpl::Links& EngineImpl::getCurrentLinks()
	//{
	//	return m_Links;
	//}

	//EngineImpl::Bundles const& EngineImpl::getCurrentBundles() const
	//{
	//	return m_BundleManager->getBundles();
	//}

	void EngineImpl::clearLinksFor( BasicInletIO &inlet )
	{
		for ( LinkIterator it = mLinks.begin(); it != mLinks.end(); )
		{
			if ( ( *it )->isInletInvolved( inlet ) )
			{
				( *it )->deactivate();
				delete *it;
				it = mLinks.erase( it );
			}
			else ++it;
		}
	}

	IOLink EngineImpl::createLink( BasicInletIO &inlet, OutletIO &outlet )
	{
		/* links are stored in a set, so I have to create a dummy link... sigh */
		IOLink *link = IOLink::link( inlet, outlet );

		if ( link != nullptr )
		{
			LinkIterator it = mLinks.find( link );
			if ( it == mLinks.end() )
			{
				link->activate();
				mLinks.insert( link );
				return *link;
			}
			else
			{
				delete link;
				return ( **it );
			}
		}
		else
		{
			return IOLink();
		}
	}

	//TypeMetadata const& EngineImpl::getType( std::string const& bundle, std::string const& name ) const
	//{
	//	return m_TypeRegistry->getType( bundle, name );
	//}

	//std::pair< IOLink, IOLink > EngineImpl::createLinkWithConversion( BasicInletIO &inlet, OutletIO &outlet )
	//{
	//	if ( IOLink::canAutoConvert( inlet, outlet ) )
	//	{
	//		IOLink *link = IOLink::linkWithAutoConversion( inlet, outlet );
	//		LinkIterator it = m_Links.find( link );
	//		if ( it == m_Links.end() )
	//		{
	//			link->activate();
	//			m_Links.insert( link );
	//			return std::make_pair( *link, IOLink() );
	//		}
	//		else
	//		{
	//			delete link;
	//			return std::make_pair( ( **it ), IOLink() );
	//		}
	//	}

	//	//const string conversionName = IOLink::findConversion( inlet, outlet );
	//	//Bundle &bundle = m_BundleManager->findBundleByName( strTypeConversions );

	//	//if ( bundle.canCreate( conversionName ) )
	//	//{
	//	//	app::BlockHandle &block = bundle.createBlockInstance( conversionName );
	//	//	block.setUpdateRate( 0. );
	//	//	app::InletHandle &in = block.getInletHandle( "src" );
	//	//	in.setUpdatePolicy( Policy::AND_NEWER_DATA );
	//	//	app::OutletHandle &out = block.getOutletHandle( "dst" );

	//	//	in.link( outlet.getHandle() );
	//	//	out.link( inlet.getHandle() );

	//	//	block.setup();
	//	//	block.start();
	//	//	return true;
	//	//}

	//	return std::make_pair( IOLink(), IOLink() );
	//}

	void EngineImpl::destroyLink( BasicInletIO &inlet, OutletIO &outlet )
	{
		//IOLink *link = new IOLink( inlet, outlet );
		//LinkIterator it = m_Links.find( link );
		//if ( it != m_Links.end() )
		//{
		//	link->deactivate();
		//	delete *it;
		//	m_Links.erase( it );
		//}
		//delete link;
	}

}