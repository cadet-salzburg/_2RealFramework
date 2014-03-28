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

#include "helpers/_2RealBoost.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealConstants.h"

#include "engine/_2RealBundle.h"
#include "engine/_2RealBundleCollection.h"

namespace _2Real
{
	BundleCollection::BundleCollection( std::shared_ptr< TypeCollection > registry, std::shared_ptr< Threadpool > stdthreads, std::shared_ptr< Threadpool > ctxtthreads ) :
		std::enable_shared_from_this< BundleCollection >(),
		mBundleImporter( registry ),
		mStdThreads( stdthreads ),
		mCtxtThreads( ctxtthreads )
	{
		updateBundleDirectory();
	}

	BundleCollection::~BundleCollection()
	{
		clear( 1000 );
	}

	void BundleCollection::clear( const unsigned long timeout )
	{
		for ( auto it = mBundles.begin(); it != mBundles.end(); )
		{
			it->second->unregisterFromUnload( this, &BundleCollection::bundleUnloaded );

			/*
			*	kills blocks
			*/
			it->second->unload( timeout );

			/*
			*	calls dtor, removes exported service & types, deletes bundle metainfo
			*/
			_2Real::Path id = it->first;
			it = mBundles.erase( it );

			/*
			*	unloads dll
			*/
			mBundleImporter.unimportLibrary( id );
		}
	}

	Path const& BundleCollection::getBundleDirectory() const
	{
		return mBundleDirectory;
	}

	void BundleCollection::updateBundleDirectory()
	{
		char * dir = std::getenv( _2Real::Constants::BundleEnvName.c_str() );
		if ( nullptr != dir )
			mBundleDirectory = Path( dir );

		std::ostringstream msg;
		msg << "---- bundle directory: " << mBundleDirectory.string() << " ----";
		std::cout << msg.str() << std::endl;
	}

	std::shared_ptr< Bundle > BundleCollection::loadBundle( Path const& path )
	{
		Path absPath = mBundleDirectory / path;

		if ( mBundleImporter.isLibraryLoaded( absPath ) )
		{
			std::ostringstream msg;
			msg << "shared library " << absPath.string() << " is already loaded";
			throw AlreadyExistsException( msg.str() );
		}

		std::shared_ptr< const SharedLibraryMetainfo > info = mBundleImporter.importLibrary( absPath );
		std::shared_ptr< Bundle > bundle( new Bundle( shared_from_this(), info, mStdThreads.lock(), mCtxtThreads.lock() ) );
		bundle->registerToUnload( this, &BundleCollection::bundleUnloaded );
		bundle->init();
		mBundles[ absPath ] = bundle;
		return bundle;
	}

	void BundleCollection::bundleUnloaded( std::shared_ptr< const Bundle > bundle )
	{
		auto it = mBundles.find( bundle->getFilePath() );
		if ( it != mBundles.end() )
			mBundles.erase( it );
		mBundleImporter.unimportLibrary( bundle->getFilePath() );
	}

}
