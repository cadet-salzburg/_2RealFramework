
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

#include "_2RealBundleInternal.h"
#include "app/_2RealBundleHandle.h"
#include "_2RealBundleData.h"
#include "_2RealBlockData.h"
#include "_2RealBlock.h"
#include "app/_2RealBlockHandle.h"
#include "_2RealBundleManager.h"

#include <sstream>

using std::string;
using std::ostringstream;
using std::make_pair;

namespace _2Real
{

	BundleInternal::BundleInternal( BundleIdentifier const& id, BundleData const& data, BundleManager &bundleManager ) :
		m_BundleManager( bundleManager ),
		m_Identifier( id ),
		m_BundleData( data ),
		m_BundleContext()
	{
	}

	BundleInternal::~BundleInternal()
	{
		for ( BlockMap::iterator it = m_BlockInstances.begin(); it != m_BlockInstances.end(); ++it )
		{
			bundle::Block *b = it->second;
			delete b;
		}
	}

	BundleIdentifier const& BundleInternal::getIdentifier() const
	{
		return m_Identifier;
	}
	
	std::string const& BundleInternal::getName() const
	{
		return m_Identifier.getName();
	}

	app::BundleHandle BundleInternal::createHandle()
	{
		return app::BundleHandle( *this );
	}

	BundleData const& BundleInternal::getMetadata() const
	{
		// TODO: copy this? there might be an issue with the engine data
		return m_BundleData;
	}

	app::BlockHandle BundleInternal::createBlockInstance( std::string const& blockName )
	{
		// this is a bit strange, bundle mgr will call 'addBlockInstance'
		// = result of interface changes
		return m_BundleManager.createFunctionBlock( *this, blockName );
	}

	void BundleInternal::setBundleContextHandle( app::ContextBlockHandle const& handle )
	{
		// will be called by bundle manager on loading a bundle ( if there is one )
		m_BundleContext = handle;
	}

	app::ContextBlockHandle BundleInternal::getBundleContextHandle() const
	{
		return m_BundleContext;
	}

	unsigned int BundleInternal::getBlockInstanceCount( string const& blockName ) const
	{
		unsigned int counter = 0;
		std::pair< BlockMap::const_iterator, BlockMap::const_iterator > range = m_BlockInstances.equal_range( blockName );
		for ( BlockMap::const_iterator it = range.first; it != range.second; ++it )
		{
			++counter;
		}

		return counter;
	}

	void BundleInternal::addBlockInstance( bundle::Block &block, string const& blockName )
	{
		m_BlockInstances.insert( make_pair( blockName, &block ) );
	}

}