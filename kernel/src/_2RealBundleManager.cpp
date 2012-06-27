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

#include "_2RealBundleManager.h"
#include "_2RealBundleInternal.h"
#include "_2RealSystemBlock.h"
#include "_2RealFunctionBlock.h"
#include "_2RealBundleData.h"
#include "_2RealBlockData.h"
#include "_2RealBundleIdentifier.h"
#include "_2RealEngineImpl.h"

#include <iostream>
#include <sstream>

using Poco::Path;
using std::ostringstream;
using std::string;
using std::make_pair;

namespace _2Real
{

	BundleManager::BundleManager() :
		m_BundleLoader(),
		m_BundleInstances(),
		m_BaseDirectory( Path() ),
		m_BundleContexts( nullptr ),
		m_BundleNames()
	{
	}

	BundleManager::~BundleManager()
	{
		if ( m_BundleContexts )
		{
			m_BundleContexts->clear();
			delete m_BundleContexts;
		}

		m_BundleNames.clear();

		for ( BundleMap::iterator it = m_BundleInstances.begin(); it != m_BundleInstances.end(); /**/ )
		{
			BundleInternal *b = it->second;
			delete b;
			it = m_BundleInstances.erase( it );
		}
	}

	void BundleManager::setBaseDirectory( Path const& path )
	{
		m_BaseDirectory = path;
	}
	
	bool BundleManager::isLibraryLoaded( Path const& path ) const
	{
		Path abs = makeAbsolutePath( path );
		return m_BundleLoader.isLibraryLoaded( abs.toString() );
	}

	const std::string BundleManager::getInfoString( BundleIdentifier const& bundleId ) const
	{
		BundleInternal const& bundle = getBundle( bundleId );
		return bundle.getBundleInfoString();
	}

	BundleData const& BundleManager::getBundleData( BundleIdentifier const& bundleId ) const
	{
		BundleInternal const& bundle = getBundle( bundleId );
		return bundle.getBundleData();
	}

	BlockData const& BundleManager::getBlockData( BundleIdentifier const& bundleId, std::string const& blockName ) const
	{
		BundleInternal const& bundle = getBundle( bundleId );
		return bundle.getBlockData( blockName );
	}
	
	const BundleIdentifier BundleManager::loadLibrary( Path const& path )
	{
		if ( m_BundleContexts == nullptr )
		{
			m_BundleContexts = new SystemBlock( EngineImpl::instance().createBlockId( "context manager" ) );
		}

		string absPath = makeAbsolutePath( path ).toString();

		if ( m_BundleLoader.isLibraryLoaded( absPath ) )
		{
			return getIdentifier( absPath );
		}

		BundleData const& data = m_BundleLoader.loadLibrary( absPath );
		BundleInternal *bundle = new BundleInternal( EngineImpl::instance().createBundleId( absPath ), data );

		m_BundleInstances.insert( make_pair( bundle->getIdentifier(), bundle ) );
		m_BundleNames.insert( make_pair( absPath, bundle->getIdentifier() ) );

		if ( m_BundleLoader.hasContext( absPath ) )
		{
			m_BundleContexts->createFunctionBlock( BundleIdentifier( bundle->getIdentifier() ), "bundle context" );

			FunctionBlock &bundleContext = bundle->getBundleContext();
			m_BundleContexts->setUp( BlockIdentifier( bundleContext.getIdentifier() ) );
			m_BundleContexts->start( BlockIdentifier( bundleContext.getIdentifier() ) );
		}

		return bundle->getIdentifier();
	}

	FunctionBlock & BundleManager::createServiceBlock( BundleIdentifier const& bundleId, std::string const& blockName, SystemBlock &sys )
	{
		BundleInternal &bundle = getBundle( bundleId );
		BundleData const& bundleData = bundle.getBundleData();

		BlockData const& blockData = bundle.getBlockData( blockName );
		unsigned int count = bundle.getBlockInstanceCount( blockName );

		std::ostringstream name;
		name << blockName << " # " << count;

		BlockIdentifier blockId = EngineImpl::instance().createBlockId( name.str() );

		FunctionBlock *uberBlock;

		if ( blockName != "bundle context" )
		{
			Block & block = m_BundleLoader.createBlock( bundleData.getInstallDirectory(), blockName );
			uberBlock = new FunctionBlock( blockData, block, sys, blockId );

			bundle.addBlockInstance( block, blockName );
		}
		else
		{
			Block & block = m_BundleLoader.createContext( bundleData.getInstallDirectory() );
			uberBlock = new FunctionBlock( blockData, block, sys, blockId );

			bundle.setBundleContext( *uberBlock );
		}

		return *uberBlock;
	}

	BundleInternal & BundleManager::getBundle( BundleIdentifier const& id )
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

	BundleInternal const& BundleManager::getBundle( BundleIdentifier const& id ) const
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

	const BundleIdentifier BundleManager::getIdentifier( string const& path ) const
	{
		LookupTable::const_iterator it = m_BundleNames.find( path );

		if ( it == m_BundleNames.end() )
		{
			ostringstream msg;
			msg << "bundle at " << path << " not found";
			throw NotFoundException( msg.str() );
		}

		return it->second;
	}

	const Path BundleManager::makeAbsolutePath( Path const& path ) const
	{
		if ( path.isAbsolute() )
		{
			return path;
		}
		else
		{
			Path abs = m_BaseDirectory;
			abs.append( path );
			return abs;
		}
	}

}