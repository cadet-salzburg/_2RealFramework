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

	BundleManager::BundleManager( EngineImpl *engine ) :
		m_EngineImpl( engine ),
		m_Registry( engine->getTypeRegistry() ),
		m_BundleLoader( engine->getTypeRegistry() )
	{
#ifdef _DEBUG
		assert( m_EngineImpl );
		assert( m_Registry );
#endif

		//char * dir = std::getenv( "_2REAL_BUNDLE_DIR" );
		//if ( nullptr != dir )
		//	m_BundleDirectory = Poco::Path( dir );

		m_BundleDirectory.makeAbsolute();

		std::ostringstream msg;
		msg << "bundle directory: " << m_BundleDirectory.toString();
		m_EngineImpl->getLogger()->addLine( msg.str() );
	}

	BundleManager::~BundleManager()
	{
		clear();
	}

	void BundleManager::clear()
	{
		for ( BundleIterator it = m_Bundles.begin(); it != m_Bundles.end(); /**/ )
		{
			delete *it;
			it = m_Bundles.erase( it );
		}
	}

	void BundleManager::destroyBundle( Bundle &bundle, const long timeout )
	{
		BundleIterator it = m_Bundles.find( &bundle );
#ifdef _DEBUG
		if ( it == m_Bundles.end() )
		{
			assert( NULL );
		}
#endif

		Bundle::BlockInstances & blocks = bundle.getBlockInstances( *this );
		//for ( Bundle::BlockInstanceIterator it = blocks.begin(); it != blocks.end(); ++it )
		//{
		//	m_Engine.removeBlock( *it->second, timeout );
		//}

		while ( !blocks.empty() )
		{
			Bundle::BlockInstanceIterator it = blocks.begin();
			m_EngineImpl->removeBlock( *it->second, timeout );
		}

		if ( bundle.hasContext() )
		{
			FunctionBlock< app::ContextBlockHandle > &context = bundle.getContextBlock( *this );
			m_EngineImpl->removeBlock( context, timeout );
		}

		m_BundleLoader.unloadLibrary( bundle.getAbsPath() );
		m_Bundles.erase( it );
	}

	BundleManager::Bundles const& BundleManager::getBundles() const
	{
		return m_Bundles;
	}

	std::string BundleManager::getBundleDirectory() const
	{
		return m_BundleDirectory.toString();
	}

	//void BundleManager::setBaseDirectory( string const& directory )
	//{
	//	m_BundleDirectory = Poco::Path( directory );
	//}

	//void BundleManager::createBundleEx( std::string const& path, void ( *MetainfoFunc )( bundle::BundleMetainfo & ) )
	//{
	//	BundleMetadata const& bundleData = m_BundleLoader.createBundleEx( path, MetainfoFunc );

	//	app::BundleInfo bundleInfo;
	//	bundleInfo.name = bundleData.getName();
	//	bundleInfo.directory = bundleData.getInstallDirectory();
	//	bundleInfo.description = bundleData.getDescription();
	//	bundleInfo.contact = bundleData.getContact();
	//	bundleInfo.author = bundleData.getAuthor();
	//	bundleInfo.category = bundleData.getCategory();

	//	BundleMetadata::BlockMetadatas const& blockMetadata = bundleData.getExportedBlocks();

	//	for ( BundleMetadata::BlockMetadataConstIterator it = blockMetadata.begin(); it != blockMetadata.end(); ++it )
	//	{
	//		app::BlockInfo blockInfo;
	//		blockInfo.name = it->second->getName();
	//		blockInfo.description = it->second->getDescription();
	//		blockInfo.category = it->second->getCategory();

	//		//BlockMetadata::InletMetadatas const& input = it->second->getInlets();
	//		//BlockMetadata::OutletMetadatas const& output = it->second->getOutlets();

	//	//	for ( BlockMetadata::InletMetadataConstIterator it = input.begin(); it != input.end(); ++it )
	//	//	{
	//	//		app::InletInfo info;
	//	//		info.name = ( *it )->name;
	//	//		info.typeName = ( *it )->type->m_TypeName;
	//	//		info.longTypename = ( *it )->type->m_LongTypename;
	//	//		info.isMultiInlet = ( *it )->isMulti;
	//	//		info.hasOptionCheck = !( *it )->options.isEmpty();
	//	//		info.defaultPolicy = ( *it )->defaultPolicy;
	//	//		blockInfo.inlets.push_back( info );
	//	//	}

	//	//	for ( BlockMetadata::OutletMetadataConstIterator it = output.begin(); it != output.end(); ++it )
	//	//	{
	//	//		app::OutletInfo info;
	//	//		info.name = ( *it )->name;
	//	//		info.typeName = ( *it )->type->m_TypeName;
	//	//		info.longTypename = ( *it )->type->m_LongTypename;
	//	//		blockInfo.outlets.push_back( info );
	//	//	}

	//		bundleInfo.exportedBlocks.push_back( blockInfo );
	//	}
	//	Bundle *bundle = new Bundle( bundleInfo, *this );
	//	m_Bundles.insert( bundle );
	//}

	Bundle & BundleManager::loadLibrary( string const& libraryPath )
	{
		// libraryPath: relative to bundle dir

		// TODO: this could could be a lot shorter :)

		string absPath = makeAbsolutePath( Poco::Path( libraryPath ) ).toString();
		if ( m_BundleLoader.isLibraryLoaded( absPath ) )
		{
			ostringstream msg;
			msg << "shared library " << absPath << " is already loaded";
			throw AlreadyExistsException( msg.str() );
		}

		BundleMetadata const& bundleData = m_BundleLoader.loadLibrary( absPath );

		// TODO: argh, are all those different 'infos' really needed?
		// here, i build the app::BundleInfo from the bundle metadata.
		app::BundleInfo bundleInfo;
		bundleInfo.name = bundleData.getName();
		bundleInfo.directory = bundleData.getInstallDirectory();
		bundleInfo.description = bundleData.getDescription();
		bundleInfo.contact = bundleData.getContact();
		bundleInfo.author = bundleData.getAuthor();
		bundleInfo.category = bundleData.getCategory();

		//comment this in to force unique bundle names
		//for ( BundleConstIterator it = m_Bundles.begin(); it != m_Bundles.end(); ++it )
		//{
		//	if ( ( *it )->getName() == bundleInfo.name )
		//	{
		//		ostringstream msg;
		//		msg << "a bundle named" << bundleInfo.name << " is already loaded";
		//		throw AlreadyExistsException( msg.str() );
		//	}
		//}

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
				app::OutletInfo info;
				info.name = ( *oit )->name;
				info.customName = ( *oit )->type;
				blockInfo.outlets.push_back( info );
			}

			if ( it->second->getName() == "contextblock" )
			{
				continue;
			}

			for ( BlockMetadata::InletMetadataConstIterator iit = input.begin(); iit != input.end(); ++iit )
			{
				app::InletInfo info;
				info.name = ( *iit )->name;
				info.customName = ( *iit )->type;
				info.isMultiInlet = ( *iit )->isMulti;
				info.defaultPolicy = ( *iit )->defaultPolicy;
				info.initValue = ( *iit )->initValue;
				blockInfo.inlets.push_back( info );
			}

			for ( BlockMetadata::ParameterMetadataConstIterator pit = params.begin(); pit != params.end(); ++pit )
			{
				app::ParameterInfo info;
				info.name = ( *pit )->name;
				info.customName = ( *pit )->type;
				info.initValue = ( *pit )->initValue;
				blockInfo.parameters.push_back( info );
			}

			bundleInfo.exportedBlocks.push_back( blockInfo );
		}

		Bundle *bundle = new Bundle( bundleInfo, *this );
		m_Bundles.insert( bundle );

		return *bundle;
	}

	bool BundleManager::isLibraryLoaded( Poco::Path const& path ) const
	{
		Poco::Path abs = makeAbsolutePath( path );
		return m_BundleLoader.isLibraryLoaded( abs.toString() );
	}

	void BundleManager::removeContextBlock( Bundle const& bundle )
	{
		m_BundleLoader.removeContextBlock( bundle.getAbsPath() );
	}

	FunctionBlock< app::BlockHandle > & BundleManager::createBlockInstance( Bundle &bundle, std::string const &blockName, std::string const& name )
	{
		std::string absPath = bundle.getAbsPath();
		BundleMetadata const& bundleMetadata = m_BundleLoader.getBundleMetadata( bundle.getAbsPath() );

		if ( !bundle.hasContext() && m_BundleLoader.hasContext( absPath ) )
		{
			BlockMetadata const& contextMetadata = bundleMetadata.getBlockData( "contextblock" );

			app::BlockInfo contextInfo;
			contextInfo.name = contextMetadata.getName();
			contextInfo.description = contextMetadata.getDescription();
			contextInfo.category = contextMetadata.getCategory();

			bundle::Block & block = m_BundleLoader.createContext( absPath );
			FunctionBlock< app::ContextBlockHandle > *contextBlock = new FunctionBlock< app::ContextBlockHandle >( m_EngineImpl, &bundle, &block, contextInfo );
			bundle.setContextBlock( *contextBlock );
			m_EngineImpl->addBlock( *contextBlock );

			contextBlock->updateWithFixedRate( 30.0 );
			contextBlock->setUp();
			contextBlock->start();
		}

		BlockMetadata const& blockMetadata = bundleMetadata.getBlockData( blockName );

		app::BlockInfo blockInfo;
		blockInfo.name = blockMetadata.getName();
		blockInfo.description = blockMetadata.getDescription();
		blockInfo.category = blockMetadata.getCategory();

		BlockMetadata::InletMetadatas const& inletMetadata = blockMetadata.getInlets();
		BlockMetadata::OutletMetadatas const& outletMetadata = blockMetadata.getOutlets();
		BlockMetadata::ParameterMetadatas const& parameterMetadata = blockMetadata.getParameters();

		app::BlockInfo info( blockInfo );
		info.name = name;

		bundle::Block & block = m_BundleLoader.createBlockInstance( bundle.getAbsPath(), blockName, name );
		FunctionBlock< app::BlockHandle > *functionBlock = new FunctionBlock< app::BlockHandle >( m_EngineImpl, &bundle, &block, info );
		m_EngineImpl->addBlock( *functionBlock );

		for ( BlockMetadata::InletMetadataConstIterator it = inletMetadata.begin(); it != inletMetadata.end(); ++it )
		{
			std::shared_ptr< const CustomType > initializer;

			TypeMetadata const* meta = ( **it ).metadata;
			if ( nullptr == meta )
				meta = m_Registry->get( bundleMetadata.getName(), ( **it ).type );

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
				meta = m_Registry->get( bundleMetadata.getName(), ( **it ).type );

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
				meta = m_Registry->get( bundleMetadata.getName(), ( **it ).type );

			initializer.reset( new CustomType( *meta ) );

			functionBlock->addOutlet( **it, initializer, *meta );
		}

		functionBlock->updateWithFixedRate( 30.0 );

		return *functionBlock;
	}

	//Bundle & BundleManager::findBundleByName( string const& name ) const
	//{
	//	for ( BundleConstIterator it = m_Bundles.begin(); it != m_Bundles.end(); ++it )
	//	{
	//		if ( ( *it )->getName() == name )
	//		{
	//			return **it;
	//		}
	//	}

	//	ostringstream msg;
	//	msg << "bundle " << name << " is not loaded";
	//	throw NotFoundException( msg.str() );
	//}

	Bundle * BundleManager::findBundleByPath( string const& libraryPath ) const
	{
		Poco::Path abs = makeAbsolutePath( libraryPath );

		for ( BundleConstIterator it = m_Bundles.begin(); it != m_Bundles.end(); ++it )
		{
			if ( ( *it )->getAbsPath() == abs.toString() )
			{
				return *it;
			}
		}

		ostringstream msg;
		msg << "bundle at " << libraryPath << " is not loaded";
		throw NotFoundException( msg.str() );
	}

	const Poco::Path BundleManager::makeAbsolutePath( Poco::Path const& path ) const
	{
		if ( path.isAbsolute() )
		{
			return path;
		}
		else
		{
			Poco::Path abs = m_BundleDirectory.absolute().append( path );
			return abs;
		}
	}
}
