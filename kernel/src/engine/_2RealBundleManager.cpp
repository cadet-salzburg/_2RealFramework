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

#include <sstream>
#include <assert.h>

using std::ostringstream;
using std::string;
using std::make_pair;

namespace _2Real
{

	BundleManager::BundleManager( EngineImpl &engine ) :
		m_Engine( engine ),
		m_BaseDirectory( Poco::Path() ),
		m_BundleLoader()
	{
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
			m_Engine.removeBlock( *it->second, timeout );
		}

		if ( bundle.hasContext() )
		{
			FunctionBlock< app::ContextBlockHandle > &context = bundle.getContextBlock( *this );
			m_Engine.removeBlock( context, timeout );
		}

		m_BundleLoader.unloadLibrary( bundle.getAbsPath() );
		m_Bundles.erase( it );
	}

	BundleManager::Bundles const& BundleManager::getBundles() const
	{
		return m_Bundles;
	}

	void BundleManager::setBaseDirectory( string const& directory )
	{
		m_BaseDirectory = Poco::Path( directory );
	}

	void BundleManager::createBundleEx( std::string const& path, void ( *MetainfoFunc )( bundle::BundleMetainfo & ) )
	{
		BundleMetadata const& bundleData = m_BundleLoader.createBundleEx( path, MetainfoFunc );

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

			//BlockMetadata::InletMetadatas const& input = it->second->getInlets();
			//BlockMetadata::OutletMetadatas const& output = it->second->getOutlets();

		//	for ( BlockMetadata::InletMetadataConstIterator it = input.begin(); it != input.end(); ++it )
		//	{
		//		app::InletInfo info;
		//		info.name = ( *it )->name;
		//		info.typeName = ( *it )->type->m_TypeName;
		//		info.longTypename = ( *it )->type->m_LongTypename;
		//		info.isMultiInlet = ( *it )->isMulti;
		//		info.hasOptionCheck = !( *it )->options.isEmpty();
		//		info.defaultPolicy = ( *it )->defaultPolicy;
		//		blockInfo.inlets.push_back( info );
		//	}

		//	for ( BlockMetadata::OutletMetadataConstIterator it = output.begin(); it != output.end(); ++it )
		//	{
		//		app::OutletInfo info;
		//		info.name = ( *it )->name;
		//		info.typeName = ( *it )->type->m_TypeName;
		//		info.longTypename = ( *it )->type->m_LongTypename;
		//		blockInfo.outlets.push_back( info );
		//	}

			bundleInfo.exportedBlocks.push_back( blockInfo );
		}
		Bundle *bundle = new Bundle( bundleInfo, *this );
		m_Bundles.insert( bundle );
	}

	Bundle & BundleManager::loadLibrary( string const& libraryPath )
	{
		// TODO: this could could be a lot shorter :)

		string absPath = makeAbsolutePath( Poco::Path( libraryPath ) ).toString();

		if ( m_BundleLoader.isLibraryLoaded( absPath ) )
		{
			ostringstream msg;
			msg << "shared library " << absPath << " is already loaded";
			throw AlreadyExistsException( msg.str() );
		}

		BundleMetadata const& bundleData = m_BundleLoader.loadLibrary( absPath );

		app::BundleInfo bundleInfo;
		bundleInfo.name = bundleData.getName();
		bundleInfo.directory = bundleData.getInstallDirectory();
		bundleInfo.description = bundleData.getDescription();
		bundleInfo.contact = bundleData.getContact();
		bundleInfo.author = bundleData.getAuthor();
		bundleInfo.category = bundleData.getCategory();

		for ( BundleConstIterator it = m_Bundles.begin(); it != m_Bundles.end(); ++it )
		{
			if ( ( *it )->getName() == bundleInfo.name )
			{
				ostringstream msg;
				msg << "a bundle named" << bundleInfo.name << " is already loaded";
				throw AlreadyExistsException( msg.str() );
			}
		}

		BundleMetadata::BlockMetadatas const& blockMetadata = bundleData.getExportedBlocks();

		for ( BundleMetadata::BlockMetadataConstIterator it = blockMetadata.begin(); it != blockMetadata.end(); ++it )
		{
			app::BlockInfo blockInfo;
			blockInfo.name = it->second->getName();
			blockInfo.description = it->second->getDescription();
			blockInfo.category = it->second->getCategory();

			if ( it->second->getName() == "contextblock" )
			{
				continue;
			}

			//BlockMetadata::InletMetadatas const& input = it->second->getInlets();
			//BlockMetadata::OutletMetadatas const& output = it->second->getOutlets();

			//for ( BlockMetadata::InletMetadataConstIterator it = input.begin(); it != input.end(); ++it )
			//{
			//	app::InletInfo info;
			//	info.name = ( *it )->name;
			//	info.typeName = ( *it )->type->m_TypeName;
			//	info.longTypename = ( *it )->type->m_LongTypename;
			//	info.isMultiInlet = ( *it )->isMulti;
			//	info.hasOptionCheck = !( *it )->options.isEmpty();
			//	info.defaultPolicy = ( *it )->defaultPolicy;
			//	blockInfo.inlets.push_back( info );
			//}

			//for ( BlockMetadata::OutletMetadataConstIterator it = output.begin(); it != output.end(); ++it )
			//{
			//	app::OutletInfo info;
			//	info.name = ( *it )->name;
			//	info.typeName = ( *it )->type->m_TypeName;
			//	info.longTypename = ( *it )->type->m_LongTypename;
			//	blockInfo.outlets.push_back( info );
			//}

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

	FunctionBlock< app::BlockHandle > & BundleManager::createBlockInstance( Bundle &bundle, std::string const &blockName )
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

			//BlockMetadata::InletMetadatas const& inletMetadata = contextMetadata.getInlets();
			//BlockMetadata::OutletMetadatas const& outletMetadata = contextMetadata.getOutlets();

			//for ( BlockMetadata::InletMetadataConstIterator it = inletMetadata.begin(); it != inletMetadata.end(); ++it )
			//{
			//	app::InletInfo info;
			//	info.name = ( *it )->name;
			//	info.typeName = ( *it )->type->m_TypeName;
			//	info.longTypename = ( *it )->type->m_LongTypename;
			//	info.isMultiInlet = ( *it )->isMulti;
			//	info.hasOptionCheck = !( *it )->options.isEmpty();
			//	info.defaultPolicy = ( *it )->defaultPolicy;
			//	contextInfo.inlets.push_back( info );
			//}

			//for ( BlockMetadata::OutletMetadataConstIterator it = outletMetadata.begin(); it != outletMetadata.end(); ++it )
			//{
			//	app::OutletInfo info;
			//	info.name = ( *it )->name;
			//	info.typeName = ( *it )->type->m_TypeName;
			//	info.longTypename = ( *it )->type->m_LongTypename;
			//	contextInfo.outlets.push_back( info );
			//}

			bundle::Block & block = m_BundleLoader.createContext( absPath );
			FunctionBlock< app::ContextBlockHandle > *contextBlock = new FunctionBlock< app::ContextBlockHandle >( bundle, block, contextInfo );
			bundle.setContextBlock( *contextBlock );
			m_Engine.addBlock( *contextBlock );

			/* NO INLETS */
			//for ( BlockMetadata::ParameterMetadataConstIterator it = inletMetadata.begin(); it != inletMetadata.end(); ++it )
			//{
			//	contextBlock->addInlet( ( *it )->name, *( ( *it )->type ), ( *it )->initValue, ( *it )->options, false );
			//}

			//for ( BlockMetadata::OutletMetadataConstIterator it = outletMetadata.begin(); it != outletMetadata.end(); ++it )
			//{
			//	contextBlock->addOutlet( ( *it )->name, *( ( *it )->type ), ( *it )->initValue );
			//}

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

		//for ( BlockMetadata::InletMetadataConstIterator it = inletMetadata.begin(); it != inletMetadata.end(); ++it )
		//{
		//	app::InletInfo info;
		//	info.name = ( *it )->name;
		//	info.typeName = ( *it )->type->m_TypeName;
		//	info.longTypename = ( *it )->type->m_LongTypename;
		//	info.isMultiInlet = ( *it )->isMulti;
		//	info.hasOptionCheck = !( *it )->options.isEmpty();
		//	info.defaultPolicy = ( *it )->defaultPolicy;
		//	blockInfo.inlets.push_back( info );
		//}

		//for ( BlockMetadata::OutletMetadataConstIterator it = outletMetadata.begin(); it != outletMetadata.end(); ++it )
		//{
		//	app::OutletInfo info;
		//	info.name = ( *it )->name;
		//	info.typeName = ( *it )->type->m_TypeName;
		//	info.longTypename = ( *it )->type->m_LongTypename;
		//	blockInfo.outlets.push_back( info );
		//}

		app::BlockInfo info( blockInfo );

		bundle::Block & block = m_BundleLoader.createBlockInstance( bundle.getAbsPath(), blockName );
		FunctionBlock< app::BlockHandle > *functionBlock = new FunctionBlock< app::BlockHandle >( bundle, block, info );
		m_Engine.addBlock( *functionBlock );

		// adding inlets & outlets

		for ( BlockMetadata::InletMetadataConstIterator it = inletMetadata.begin(); it != inletMetadata.end(); ++it )
			functionBlock->addInlet( **it );

		//for ( BlockMetadata::OutletMetadataConstIterator it = outletMetadata.begin(); it != outletMetadata.end(); ++it )
		//	functionBlock->addOutlet( ( *it )->name, *( ( *it )->type ), ( *it )->initValue );

		return *functionBlock;
	}

	Bundle & BundleManager::findBundleByName( string const& name ) const
	{
		for ( BundleConstIterator it = m_Bundles.begin(); it != m_Bundles.end(); ++it )
		{
			if ( ( *it )->getName() == name )
			{
				return **it;
			}
		}

		ostringstream msg;
		msg << "bundle " << name << " is not loaded";
		throw NotFoundException( msg.str() );
	}

	Bundle & BundleManager::findBundleByPath( string const& libraryPath ) const
	{
		Poco::Path abs = makeAbsolutePath( libraryPath );

		for ( BundleConstIterator it = m_Bundles.begin(); it != m_Bundles.end(); ++it )
		{
			if ( ( *it )->getAbsPath() == abs.toString() )
			{
				return **it;
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
			Poco::Path abs = m_BaseDirectory.absolute().append( path );
			return abs;
		}
	}
}
