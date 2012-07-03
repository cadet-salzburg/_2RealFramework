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
#include "engine/_2RealBundleInternal.h"
#include "engine/_2RealSystem.h"
#include "engine/_2RealFunctionBlock.h"
#include "engine/_2RealBundleData.h"
#include "engine/_2RealBlockData.h"
#include "engine/_2RealIdentifier.h"
#include "engine/_2RealEngineImpl.h"
#include "app/_2RealBundleHandle.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealContextBlockHandle.h"

#include <sstream>

using std::ostringstream;
using std::string;
using std::make_pair;

namespace _2Real
{

	BundleManager::BundleManager( EngineImpl &engine ) :
		m_Engine( engine ),
		m_BundleLoader(),
		m_BundleInstances(),
		m_BaseDirectory( Poco::Path() ),
		m_BundleLookupTable()
	{
	}

	BundleManager::~BundleManager()
	{
		clear();
	}

	void BundleManager::clear()
	{
		for ( BundleMap::iterator it = m_BundleInstances.begin(); it != m_BundleInstances.end(); /**/ )
		{
			BundleInternal *b = it->second;
			delete b;
			it = m_BundleInstances.erase( it );
		}

		m_BundleLookupTable.clear();
	}

	void BundleManager::setBaseDirectory( string const& directory )
	{
		m_BaseDirectory = Poco::Path( directory );
	}

	app::BundleHandle & BundleManager::loadLibrary( string const& libraryPath )
	{
		string absPath = makeAbsolutePath( Poco::Path( libraryPath ) ).toString();

		if ( m_BundleLoader.isLibraryLoaded( absPath ) )
		{
			ostringstream msg;
			msg << "shared library " << absPath << " is already loaded";
			throw AlreadyExistsException( msg.str() );
		}

		BundleData const& data = m_BundleLoader.loadLibrary( absPath );
		BundleInternal *bundle = new BundleInternal( m_Engine.createIdentifier( absPath ), data, *this );

		m_BundleInstances.insert( make_pair( bundle->getIdentifier(), bundle ) );
		m_BundleLookupTable.insert( make_pair( absPath, bundle->getIdentifier() ) );

		if ( m_BundleLoader.hasContext( absPath ) )
		{
			app::ContextBlockHandle handle = createContextBlock( *bundle );
			handle.setUpdateRate( 1.0 );
			handle.setup();
			handle.start();
		}

		return bundle->getHandle();
	}
	
	bool BundleManager::isLibraryLoaded( Poco::Path const& path ) const
	{
		Poco::Path abs = makeAbsolutePath( path );
		return m_BundleLoader.isLibraryLoaded( abs.toString() );
	}

	app::BlockHandle & BundleManager::createFunctionBlock( BundleInternal &bundle, std::string const& blockName )
	{
		System &sys = m_Engine.getSystemBlock();

		BundleData const& bundleData = bundle.getMetadata();
		BlockData const& blockData = bundleData.getBlockData( blockName );
		unsigned int count = bundle.getBlockInstanceCount( blockName );

		ostringstream name;
		name << blockName << " # " << count;

		Identifier blockId = m_Engine.createIdentifier( name.str() );

		FunctionBlock *functionBlock;

		bundle::Block & block = m_BundleLoader.createBlock( bundleData.getInstallDirectory(), blockName );
		functionBlock = new FunctionBlock( blockData, block, sys, blockId );
		sys.addUberBlock( *functionBlock, false );

		bundle.addBlockInstance( block, blockName );

		return functionBlock->HandleAble< app::BlockHandle >::getHandle();
	}

	app::ContextBlockHandle BundleManager::createContextBlock( BundleInternal &bundle )
	{
		System &sys = m_Engine.getSystemBlock();

		BundleData const& bundleData = bundle.getMetadata();
		BlockData const& blockData = bundleData.getBlockData( "bundle context" );

		ostringstream name;
		name << "bundle context";

		Identifier blockId( m_Engine.createIdentifier( name.str() ) );

		FunctionBlock *functionBlock;

		bundle::Block & block = m_BundleLoader.createContext( bundleData.getInstallDirectory() );
		functionBlock = new FunctionBlock( blockData, block, sys, blockId );
		sys.addUberBlock( *functionBlock, true );

		app::ContextBlockHandle handle = app::ContextBlockHandle( *functionBlock );
		bundle.setBundleContextHandle( handle );

		return functionBlock->HandleAble< app::ContextBlockHandle >::getHandle();
	}

	BundleInternal & BundleManager::getBundle( Identifier const& id )
	{
		BundleMap::const_iterator it = m_BundleInstances.find( id );
		
		if ( it == m_BundleInstances.end() )
		{
			ostringstream msg;
			msg << "bundle " << id << " not found";
			throw NotFoundException( msg.str() );
		}

		return *( it->second );
	}

	BundleInternal const& BundleManager::getBundle( Identifier const& id ) const
	{
		BundleMap::const_iterator it = m_BundleInstances.find( id );

		if ( it == m_BundleInstances.end() )
		{
			ostringstream msg;
			msg << "bundle " << id << " not found";
			throw NotFoundException( msg.str() );
		}

		return *( it->second );
	}

	const Identifier BundleManager::getIdentifier( string const& path ) const
	{
		LookupTable::const_iterator it = m_BundleLookupTable.find( path );

		if ( it == m_BundleLookupTable.end() )
		{
			ostringstream msg;
			msg << "bundle at " << path << " not found";
			throw NotFoundException( msg.str() );
		}

		return it->second;
	}

	const Poco::Path BundleManager::makeAbsolutePath( Poco::Path const& path ) const
	{
		if ( path.isAbsolute() )
		{
			return path;
		}
		else
		{
			Poco::Path abs = m_BaseDirectory;
			abs.append( path );
			return abs;
		}
	}

}