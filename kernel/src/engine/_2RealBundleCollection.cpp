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

#include "engine/_2RealBundleCollection.h"

#include <boost/log/sources/basic_logger.hpp>

namespace _2Real
{
	BundleCollection::BundleCollection( std::shared_ptr< TypeRegistry > registry ) :
		std::enable_shared_from_this< BundleCollection >(),
		mBundleImporter( registry )
	{
		updateBundleDirectory();
	}

	BundleCollection::~BundleCollection()
	{
		clear();
	}

	void BundleCollection::clear()
	{
	//	// only ever called after links & blocks have been destroied
	//	for ( BundleIterator it = mBundles.begin(); it != mBundles.end(); )
	//	{
	//		mBundleLoader.unloadLibrary( it->first );
	//		it = mBundles.erase( it );
	//	}
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
		if ( mBundleImporter.isLibraryLoaded( path ) )
		{
			std::ostringstream msg;
			msg << "shared library " << path.string() << " is already loaded";
			throw AlreadyExistsException( msg.str() );
		}

	//	std::shared_ptr< TemplateId > bundleId = IdGenerator::makeBundleId( relativePath );
		std::shared_ptr< const BundleMetadata > bundleMetadata = mBundleImporter.importLibrary( path );
	//	std::shared_ptr< Bundle > bundle( new Bundle( mEngineImpl, bundleMetadata ) );
	//	bundle->setSelfRef( bundle );
	//	mBundles.insert( std::make_pair( absPath, bundle ) );
	//	return bundle;
	}

	void BundleCollection::unloadBundle( std::shared_ptr< Bundle >, const long timeout )
	{
	//	std::string const& absPath = bundle->getBundleMetadata()->getInstallDirectory();

	//	BundleIterator it = mBundles.find( absPath );
	//	mBundleLoader.unloadLibrary( absPath );
	//	mBundles.erase( it );
	}

}
