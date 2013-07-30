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
#include "engine/_2RealParameterMetadata.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealLogger.h"
#include "datatypes/_2RealTypeRegistry.h"

#include <sstream>
#include <assert.h>

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

		BundleMetadata const& bundleData = mBundleLoader.loadLibrary( absPath );

		// TODO: argh, are all those different 'infos' really needed?
		// here, i build the app::BundleInfo from the bundle metadata.
		app::BundleInfo bundleInfo;
		bundleInfo.name = bundleData.getName();
		bundleInfo.directory = bundleData.getInstallDirectory();
		bundleInfo.description = bundleData.getDescription();
		bundleInfo.contact = bundleData.getContact();
		bundleInfo.author = bundleData.getAuthor();
		bundleInfo.category = bundleData.getCategory();

		BundleMetadata::BlockMetadatas const& blockMetadata = bundleData.getExportedBlocks();

		for ( BundleMetadata::BlockMetadataConstIterator it = blockMetadata.begin(); it != blockMetadata.end(); ++it )
		{
			app::BlockInfo blockInfo;
			blockInfo.name = it->second->getName();
			blockInfo.description = it->second->getDescription();
			blockInfo.category = it->second->getCategory();

			BlockMetadata::InletMetadatas const& input = it->second->getInlets();
			BlockMetadata::OutletMetadatas const& output = it->second->getOutlets();
			BlockMetadata::ParameterMetadatas const& params = it->second->getParameters();

			for ( BlockMetadata::OutletMetadataConstIterator oit = output.begin(); oit != output.end(); ++oit )
			{
				app::IOInfo info;
				info.name = ( *oit )->name;
				info.typeName = ( *oit )->type;
				blockInfo.outlets.push_back( info );
			}

			if ( it->second->getName() == sContextBlock )
			{
				continue;
			}

			for ( BlockMetadata::InletMetadataConstIterator iit = input.begin(); iit != input.end(); ++iit )
			{
				app::IOInfo info;
				info.name = ( *iit )->name;
				info.typeName = ( *iit )->type;
				info.isMulti = ( *iit )->isMulti;
				info.defaultPolicy = ( *iit )->defaultPolicy;
				blockInfo.inlets.push_back( info );
			}

			for ( BlockMetadata::ParameterMetadataConstIterator pit = params.begin(); pit != params.end(); ++pit )
			{
				app::IOInfo info;
				info.name = ( *pit )->name;
				info.typeName = ( *pit )->type;
				blockInfo.parameters.push_back( info );
			}

			bundleInfo.exportedBlocks.push_back( blockInfo );
		}

		std::shared_ptr< Bundle > bundle( new Bundle( mEngineImpl, bundleInfo ) );
		mBundles.insert( std::make_pair( absPath, bundle ) );
		return bundle;
	}

	std::shared_ptr< FunctionBlock > BundleManager::createContextBlockConditionally( Bundle *b, std::string const& blockName )
	{
		std::string absPath = b->getAbsPath();
		if ( ( b->getContextBlock() == nullptr ) && mBundleLoader.hasContext( absPath ) )
		{
			// need the shared ptr
			std::shared_ptr< Bundle > bundle = findBundleByPath( absPath );

			BundleMetadata const& bundleMetadata = mBundleLoader.getBundleMetadata( absPath );
			BlockMetadata const& contextMetadata = bundleMetadata.getBlockData( sContextBlock );

			app::BlockInfo contextInfo;
			contextInfo.name = contextMetadata.getName();
			contextInfo.description = contextMetadata.getDescription();
			contextInfo.category = contextMetadata.getCategory();

			std::shared_ptr< bundle::Block > block = mBundleLoader.createContext( absPath );
			std::shared_ptr< FunctionBlock > contextBlock( new FunctionBlock( mEngineImpl, bundle, block, contextInfo ) );

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

		BundleMetadata const& bundleMetadata = mBundleLoader.getBundleMetadata( absPath );
		BlockMetadata const& blockMetadata = bundleMetadata.getBlockData( blockName );

		app::BlockInfo blockInfo;
		blockInfo.name = blockMetadata.getName();
		blockInfo.description = blockMetadata.getDescription();
		blockInfo.category = blockMetadata.getCategory();

		app::BlockInfo info( blockInfo );
		info.name = name;

		std::shared_ptr< bundle::Block > block = mBundleLoader.createBlockInstance( absPath, blockName );
		std::shared_ptr< FunctionBlock > functionBlock( new FunctionBlock( mEngineImpl, bundle, block, info ) );

		BlockMetadata::InletMetadatas const& inletMetadata = blockMetadata.getInlets();
		BlockMetadata::OutletMetadatas const& outletMetadata = blockMetadata.getOutlets();
		BlockMetadata::ParameterMetadatas const& parameterMetadata = blockMetadata.getParameters();

		for ( BlockMetadata::InletMetadataConstIterator it = inletMetadata.begin(); it != inletMetadata.end(); ++it )
		{
			std::shared_ptr< const CustomType > initializer;

			TypeMetadata const* meta = ( **it ).metadata;
			if ( nullptr == meta )
				meta = mEngineImpl->getTypeRegistry()->get( bundleMetadata.getName(), ( **it ).type );

			if ( ( **it ).initValue.get() == nullptr )
				initializer.reset( new CustomType( *meta ) );
			else initializer = ( **it ).initValue;

			functionBlock->addInlet( **it, initializer, *meta );
		}

		for ( BlockMetadata::ParameterMetadataConstIterator it = parameterMetadata.begin(); it != parameterMetadata.end(); ++it )
		{
			std::shared_ptr< const CustomType > initializer;

			TypeMetadata const* meta = ( **it ).metadata;
			if ( nullptr == meta )
				meta = mEngineImpl->getTypeRegistry()->get( bundleMetadata.getName(), ( **it ).type );

			if ( ( **it ).initValue.get() == nullptr )
				initializer.reset( new CustomType( *meta ) );
			else initializer = ( **it ).initValue;

			functionBlock->addParameter( **it, initializer, *meta );
		}

		for ( BlockMetadata::OutletMetadataConstIterator it = outletMetadata.begin(); it != outletMetadata.end(); ++it )
		{
			std::shared_ptr< const CustomType > initializer;

			TypeMetadata const* meta = ( **it ).metadata;
			if ( nullptr == meta )
				meta = mEngineImpl->getTypeRegistry()->get( bundleMetadata.getName(), ( **it ).type );

			initializer.reset( new CustomType( *meta ) );

			functionBlock->addOutlet( **it, initializer, *meta );
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
