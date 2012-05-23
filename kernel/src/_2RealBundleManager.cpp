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
#include "_2RealSystemImpl.h"
#include "_2RealUpdatePolicy.h"
#include "_2RealServiceBlock.h"
#include "_2RealBundleData.h"
#include "_2RealBlockData.h"

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
		m_BundleContexts( NULL ),
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

	const std::string BundleManager::getInfoString( Identifier const& bundleId ) const
	{
		BundleInternal const& bundle = getBundle( bundleId );
		return bundle.getBundleInfoString();
	}
	
	const Identifier BundleManager::loadLibrary( Path const& path )
	{
		if ( m_BundleContexts == NULL )
		{
			m_BundleContexts = new SystemImpl( "context manager" );
		}

		string abs = makeAbsolutePath( path ).toString();

		if ( m_BundleLoader.isLibraryLoaded( abs ) )
		{
			return getIdentifier( abs );
		}

		BundleData *data = m_BundleLoader.loadLibrary( abs );
		BundleInternal *bundle = new BundleInternal( abs, *data );

		m_BundleInstances.insert( make_pair( bundle->getIdentifier(), bundle ) );
		m_BundleNames.insert( make_pair( abs, bundle->getIdentifier() ) );

		if ( m_BundleLoader.hasContext( abs ) )
		{
			UpdatePolicy policy;
			policy.triggerByUpdateRate( 1.0f );

			m_BundleContexts->createServiceBlock( bundle->getIdentifier(), "bundle context", policy );

			ServiceBlock &bundleContext = bundle->getBundleContext();
			m_BundleContexts->setUp( bundleContext.getIdentifier() );
		}

		return bundle->getIdentifier();
	}

	ServiceBlock & BundleManager::createServiceBlock( Identifier const& pluginId, std::string const& blockName, SystemImpl &sys, UpdatePolicyImpl const& triggers )
	{
		BundleInternal &bundle = getBundle( pluginId );
		BundleData const& bundleData = bundle.getBundleData();
		BlockData const& blockData = bundle.getBlockData( blockName );

		std::cout << bundleData << std::endl;

		Block & block = m_BundleLoader.createBlock( bundleData.getInstallDirectory(), blockName );
		ServiceBlock *serviceBlock = new ServiceBlock( blockData, block, sys, triggers );

		if ( blockName != "bundle context" )
		{
			bundle.addBlockInstance( block, blockName );

			if ( m_BundleLoader.hasContext( bundleData.getInstallDirectory() ) )
			{
				ServiceBlock &bundleContext = bundle.getBundleContext();
				BlockData const& bundleData = bundle.getBlockData( "bundle context" );
				std::map< std::string, ParameterData const* > const& out = bundleData.getOutlets();

				for ( std::map< std::string, ParameterData const* >::const_iterator it = out.begin(); it != out.end(); ++it )
				{
					serviceBlock->linkWith( it->first, bundleContext, it->first );
				}
			}
		}
		else
		{
			bundle.setBundleContext( *serviceBlock );
		}

		return *serviceBlock;
	}

	BundleInternal & BundleManager::getBundle( Identifier const& id )
	{
		BundleMap::const_iterator it = m_BundleInstances.find( id );
		
		if ( it == m_BundleInstances.end() )
		{
			ostringstream msg;
			msg << "bundle " << id.name() << " not found";
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
			msg << "bundle " << id.name() << " not found";
			throw NotFoundException( msg.str() );
		}

		return *( it->second );
	}

	const Identifier BundleManager::getIdentifier( string const& path ) const
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