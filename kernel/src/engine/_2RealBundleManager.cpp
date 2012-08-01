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
		m_Bundles.erase( it );
		Bundle::BlockInstances & blocks = bundle.getBlockInstances( *this );

		for ( Bundle::BlockInstanceIterator it = blocks.begin(); it != blocks.end(); ++it )
		{
			m_Engine.removeBlock( *it->second, timeout );
		}

		if ( bundle.hasContext() )
		{
			FunctionBlock< app::ContextBlockHandle > &context = bundle.getContextBlock( *this );
			m_Engine.removeBlock( context, timeout );
		}
	}

	BundleManager::Bundles const& BundleManager::getBundles() const
	{
		return m_Bundles;
	}

	void BundleManager::setBaseDirectory( string const& directory )
	{
		m_BaseDirectory = Poco::Path( directory );
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

		app::BundleInfo::BundleData data;
		app::BundleInfo::BlockInfos blocks;

		data.name = bundleData.getName();
		data.directory = bundleData.getInstallDirectory();
		data.description = bundleData.getDescription();
		data.contact = bundleData.getContact();
		data.author = bundleData.getAuthor();
		data.category = bundleData.getCategory();

		for ( BundleConstIterator it = m_Bundles.begin(); it != m_Bundles.end(); ++it )
		{
			if ( ( *it )->getName() == data.name )
			{
				ostringstream msg;
				msg << "a bundle named" << data.name << " is already loaded";
				throw AlreadyExistsException( msg.str() );
			}
		}

		BundleMetadata::BlockMetas const& blockMetadata = bundleData.getExportedBlocks();

		for ( BundleMetadata::BlockMetaConstIterator it = blockMetadata.begin(); it != blockMetadata.end(); ++it )
		{
			app::BlockInfo::BlockData blockData;
			app::BlockInfo::ParameterInfos inlets;
			app::BlockInfo::ParameterInfos outlets;

			blockData.name = it->second.getName();
			blockData.description = it->second.getDescription();
			blockData.category = it->second.getCategory();

			BlockMetadata::ParamMetas const& input = it->second.getInlets();
			BlockMetadata::ParamMetas const& output = it->second.getOutlets();

			for ( BlockMetadata::ParamMetaConstIterator it = input.begin(); it != input.end(); ++it )
			{
				app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
				inlets.push_back( paramInfo );
			}

			for ( BlockMetadata::ParamMetaConstIterator it = output.begin(); it != output.end(); ++it )
			{
				app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
				outlets.push_back( paramInfo );
			}

			blocks.push_back( app::BlockInfo( blockData, inlets, outlets ) );
		}
		Bundle *bundle = new Bundle( app::BundleInfo( data, blocks ), *this );
		m_Bundles.insert( bundle );

		if ( m_BundleLoader.hasContext( absPath ) )
		{
			BlockMetadata const& contextData = bundleData.getBlockData( "contextblock" );

			app::BlockInfo::BlockData blockData;
			app::BlockInfo::ParameterInfos inlets;
			app::BlockInfo::ParameterInfos outlets;

			blockData.name = contextData.getName();
			blockData.description = contextData.getDescription();
			blockData.category = contextData.getCategory();

			BlockMetadata::ParamMetas const& input = contextData.getInlets();
			BlockMetadata::ParamMetas const& output = contextData.getOutlets();

			for ( BlockMetadata::ParamMetaConstIterator it = input.begin(); it != input.end(); ++it )
			{
				app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
				inlets.push_back( paramInfo );
			}

			for ( BlockMetadata::ParamMetaConstIterator it = output.begin(); it != output.end(); ++it )
			{
				app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
				outlets.push_back( paramInfo );
			}

			app::BlockInfo info( blockData, inlets, outlets );

			bundle::Block & block = m_BundleLoader.createContext( absPath );
			FunctionBlock< app::ContextBlockHandle > *contextBlock = new FunctionBlock< app::ContextBlockHandle >( *bundle, block, info );
			bundle->setContextBlock( *contextBlock );

			m_Engine.addBlock( *contextBlock );

			// adding inlets & outlets
			for ( BlockMetadata::ParamMetaConstIterator it = input.begin(); it != input.end(); ++it )
			{
				contextBlock->addInlet( it->getName(), it->getLongTypename(), it->getTypename(), it->getDefaultValue(), it->getOptions() );
			}

			for ( BlockMetadata::ParamMetaConstIterator it = output.begin(); it != output.end(); ++it )
			{
				contextBlock->addOutlet( it->getName(), it->getLongTypename(), it->getTypename(), it->getDefaultValue() );
			}

			contextBlock->updateWithFixedRate( 1.0 );
			contextBlock->setUp();
			contextBlock->start();
		}

		return *bundle;
	}

	bool BundleManager::isLibraryLoaded( Poco::Path const& path ) const
	{
		Poco::Path abs = makeAbsolutePath( path );
		return m_BundleLoader.isLibraryLoaded( abs.toString() );
	}

	FunctionBlock< app::BlockHandle > & BundleManager::createBlockInstance( Bundle &bundle, std::string const &blockName )
	{
		BundleMetadata const& bundleMetadata = m_BundleLoader.getBundleMetadata( bundle.getAbsPath() );
		BlockMetadata const& blockMetadata = bundleMetadata.getBlockData( blockName );

		app::BlockInfo::BlockData blockData;
		app::BlockInfo::ParameterInfos inlets;
		app::BlockInfo::ParameterInfos outlets;

		blockData.name = blockMetadata.getName();
		blockData.description = blockMetadata.getDescription();
		blockData.category = blockMetadata.getCategory();

		BlockMetadata::ParamMetas const& input = blockMetadata.getInlets();
		BlockMetadata::ParamMetas const& output = blockMetadata.getOutlets();

		for ( BlockMetadata::ParamMetaConstIterator it = input.begin(); it != input.end(); ++it )
		{
			app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
			inlets.push_back( paramInfo );
		}

		for ( BlockMetadata::ParamMetaConstIterator it = output.begin(); it != output.end(); ++it )
		{
			app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
			outlets.push_back( paramInfo );
		}

		app::BlockInfo info( blockData, inlets, outlets );

		bundle::Block & block = m_BundleLoader.createBlockInstance( bundle.getAbsPath(), blockName );
		FunctionBlock< app::BlockHandle > *functionBlock = new FunctionBlock< app::BlockHandle >( bundle, block, info );
		m_Engine.addBlock( *functionBlock );

		// adding inlets & outlets
		for ( BlockMetadata::ParamMetaConstIterator it = input.begin(); it != input.end(); ++it )
		{
			functionBlock->addInlet( it->getName(), it->getLongTypename(), it->getTypename(), it->getDefaultValue(), it->getOptions() );
		}

		for ( BlockMetadata::ParamMetaConstIterator it = output.begin(); it != output.end(); ++it )
		{
			functionBlock->addOutlet( it->getName(), it->getLongTypename(), it->getTypename(), it->getDefaultValue() );
		}

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
