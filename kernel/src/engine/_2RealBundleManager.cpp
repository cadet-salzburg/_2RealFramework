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

#include "engine/_2RealBundleManager.h"
#include "engine/_2RealBundle.h"
#include "engine/_2RealFunctionBlock.h"
#include "engine/_2RealBundleMetadata.h"
#include "engine/_2RealBlockMetadata.h"
#include "engine/_2RealIOMetadata.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealLogger.h"
#include "datatypes/_2RealTypeRegistry.h"

using std::ostringstream;
using std::string;
using std::make_pair;

namespace _2Real
{
	const std::string BundleManager::sContextBlock = "contextblock";

	BundleManager::BundleManager( EngineImpl *engine ) :
		mEngineImpl( engine ),
		mBundleLoader( engine->getTypeRegistry() )
	{
		//char * dir = std::getenv( "_2REAL_BUNDLE_DIR" );
		//if ( nullptr != dir )
		//	m_BundleDirectory = Poco::Path( dir );

		mBundleDirectory.makeAbsolute();

		std::ostringstream msg;
		msg << "---- bundle directory: " << mBundleDirectory.toString() << " ----";
		mEngineImpl->getLogger()->addLine( msg.str() );
	}

	BundleManager::~BundleManager()
	{
		clear();
	}

	void BundleManager::clear()
	{
		// only ever called after links & blocks have been destroied
		for ( BundleIterator it = mBundles.begin(); it != mBundles.end(); /**/ )
		{
			mBundleLoader.unloadLibrary( it->first );
			it = mBundles.erase( it );
		}
	}

	void BundleManager::unloadBundle( Bundle *bundle, const long timeout )
	{
		std::string const& absPath = bundle->getAbsPath();

		BundleIterator it = mBundles.find( absPath );
#ifdef _DEBUG
		if ( it == mBundles.end() )
			assert( NULL );
#endif
		mBundleLoader.unloadLibrary( absPath );
		mBundles.erase( it );
	}

	std::string BundleManager::getBundleDirectory() const
	{
		return mBundleDirectory.toString();
	}

	std::shared_ptr< Bundle > BundleManager::loadBundle( string const& libraryPath )
	{
		// TODO: this could could be a lot shorter :)

		string absPath = makeAbsolutePath( Poco::Path( libraryPath ) ).toString();
		if ( mBundleLoader.isLibraryLoaded( absPath ) )
		{
			ostringstream msg;
			msg << "shared library " << absPath << " is already loaded";
			throw AlreadyExistsException( msg.str() );
		}

		std::shared_ptr< const BundleMetadata > bundleMetadata = mBundleLoader.loadLibrary( absPath );
		std::shared_ptr< Bundle > bundle( new Bundle( mEngineImpl, bundleMetadata ) );
		mBundles.insert( std::make_pair( absPath, bundle ) );
		return bundle;
	}

	std::shared_ptr< FunctionBlock > BundleManager::createContextBlockInstance( Bundle *b )
	{
		std::string absPath = b->getAbsPath();
		if ( mBundleLoader.exportsContext( absPath ) )
		{
			// need the shared ptr
			std::shared_ptr< Bundle > bundle = findBundleByPath( absPath );

			std::shared_ptr< const BundleMetadata > bundleMetadata = mBundleLoader.getBundleMetadata( absPath );
			std::shared_ptr< const BlockMetadata > blockMetadata = bundleMetadata->getContextBlockMetadata();
			std::shared_ptr< bundle::Block > block = mBundleLoader.createContext( absPath );
			std::shared_ptr< FunctionBlock > contextBlock( new FunctionBlock( mEngineImpl, bundle, block, blockMetadata ) );

			BlockMetadata::IOMetadatas const& inletMetadata = blockMetadata->getInlets();
			BlockMetadata::IOMetadatas const& outletMetadata = blockMetadata->getOutlets();
			BlockMetadata::IOMetadatas const& parameterMetadata = blockMetadata->getParameters();

			for ( BlockMetadata::IOMetadataConstIterator it = outletMetadata.begin(); it != outletMetadata.end(); ++it )
				contextBlock->addOutlet( *it );

			contextBlock->updateWithFixedRate( 30.0 );
			contextBlock->setUp();
			contextBlock->start();

			return contextBlock;
		}

		return std::shared_ptr< FunctionBlock >();
	}

	std::shared_ptr< FunctionBlock > BundleManager::createBlockInstance( Bundle *b, std::string const &blockName, std::string const& name )
	{
		// need the shared ptr
		std::string absPath = b->getAbsPath();

		std::shared_ptr< Bundle > bundle = findBundleByPath( absPath );

		std::shared_ptr< const BundleMetadata > bundleMetadata = mBundleLoader.getBundleMetadata( absPath );
		std::shared_ptr< const BlockMetadata > blockMetadata = bundleMetadata->getFunctionBlockMetadata( blockName );

		std::shared_ptr< bundle::Block > block = mBundleLoader.createBlockInstance( absPath, blockName );
		std::shared_ptr< FunctionBlock > functionBlock( new FunctionBlock( mEngineImpl, bundle, block, blockMetadata ) );

		BlockMetadata::IOMetadatas const& inletMetadata = blockMetadata->getInlets();
		BlockMetadata::IOMetadatas const& outletMetadata = blockMetadata->getOutlets();
		BlockMetadata::IOMetadatas const& parameterMetadata = blockMetadata->getParameters();

		for ( BlockMetadata::IOMetadataConstIterator it = inletMetadata.begin(); it != inletMetadata.end(); ++it )
		{
			std::shared_ptr< const TypeMetadata > meta = ( *it )->typeMetadata;
			if ( nullptr == meta.get() )
				assert( NULL );

			functionBlock->addInlet( *it );
		}

		for ( BlockMetadata::IOMetadataConstIterator it = parameterMetadata.begin(); it != parameterMetadata.end(); ++it )
		{
			std::shared_ptr< const TypeMetadata > meta = ( *it )->typeMetadata;
			if ( nullptr == meta.get() )
				assert( NULL );

			functionBlock->addParameter( *it );
		}

		for ( BlockMetadata::IOMetadataConstIterator it = outletMetadata.begin(); it != outletMetadata.end(); ++it )
		{
			std::shared_ptr< const TypeMetadata > meta = ( *it )->typeMetadata;
			if ( nullptr == meta.get() )
				assert( NULL );

			functionBlock->addOutlet( *it );
		}

		functionBlock->updateWithFixedRate( 30.0 );

		return functionBlock;
	}

	std::shared_ptr< Bundle > BundleManager::findBundleByPath( std::string const& libraryPath ) const
	{
		Poco::Path abs = makeAbsolutePath( libraryPath );

		BundleConstIterator it  = mBundles.find( abs.toString() );
		if ( mBundles.end() == it )
		{
			ostringstream msg;
			msg << "bundle at " << libraryPath << " is not loaded";
			throw NotFoundException( msg.str() );
		}

		return it->second;
	}

	const Poco::Path BundleManager::makeAbsolutePath( Poco::Path const& path ) const
	{
		if ( path.isAbsolute() )
			return path;
		else
		{
			Poco::Path abs = mBundleDirectory.absolute().append( path );
			return abs;
		}
	}
}
