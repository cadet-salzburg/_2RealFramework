
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

#include "engine/_2RealBundle.h"
#include "engine/_2RealBundleManager.h"
#include "engine/_2RealFunctionBlock.h"

#include "app/_2RealBundleHandle.h"

#include <sstream>

using std::string;
using std::ostringstream;
using std::make_pair;

namespace _2Real
{

	Bundle::Bundle( app::BundleInfo const& info, BundleManager &bundleManager ) :
		NonCopyable< Bundle >(),
		Identifiable< Bundle >( Ids(), info.name ),
		Handleable< Bundle, app::BundleHandle >( *this ),
		m_BundleManager( bundleManager ),
		m_ContextBlock( nullptr ),
		m_BundleInfo( info )
	{
	}

	Bundle::~Bundle()
	{
		clear();
	}

	void Bundle::clear()
	{
		m_BlockInstances.clear();
	}

	bool Bundle::hasContext() const
	{
		return ( m_ContextBlock != nullptr );
	}

	void Bundle::contextBlockRemoved()
	{
		m_ContextBlock = nullptr;
		m_BundleManager.removeContextBlock( *this );
	}

	void Bundle::unload( const long timeout )
	{
		m_BundleManager.destroyBundle( *this, timeout );
		delete this;
	}

	Bundle::BlockInstances & Bundle::getBlockInstances( BundleManager const& m )
	{
		return m_BlockInstances;
	}

	FunctionBlock< app::ContextBlockHandle > & Bundle::getContextBlock( BundleManager const& m )
	{
		return *m_ContextBlock;
	}

	string const& Bundle::getAbsPath() const
	{
		return m_BundleInfo.directory;
	}

	app::BundleInfo const& Bundle::getBundleInfo() const
	{
		return m_BundleInfo;
	}

	bool Bundle::canCreate( std::string const& blockName ) const
	{
		for ( app::BundleInfo::BlockInfoConstIterator it = m_BundleInfo.exportedBlocks.begin(); it != m_BundleInfo.exportedBlocks.end(); ++it )
		{
			if ( toLower( it->name ) == toLower( blockName ) ) return true;
		}

		return false;
	}

	app::BlockHandle & Bundle::createBlockInstance( std::string const& blockName )
	{
		FunctionBlock< app::BlockHandle > *block = &m_BundleManager.createBlockInstance( *this, blockName );

		ostringstream name;
		name << blockName << "_" << getBlockInstanceCount( blockName );
		block->setName( name.str() );

		m_BlockInstances.insert( make_pair( blockName, block ) );
		return block->getHandle();
	}

	void Bundle::removeBlockInstance( FunctionBlock< app::BlockHandle > &block )
	{
		for ( BlockInstanceIterator it = m_BlockInstances.begin(); it != m_BlockInstances.end(); ++it )
		{
			if ( it->second->getName() == block.getName() )
			{
				m_BlockInstances.erase( it );
				break;
			}
		}
	}

	app::ContextBlockHandle & Bundle::getContextBlockHandle() const
	{
		return m_ContextBlock->getHandle();
	}

	unsigned int Bundle::getBlockInstanceCount( string const& blockName ) const
	{
		unsigned int counter = 0;
		std::pair< BlockInstanceConstIterator, BlockInstanceConstIterator > range = m_BlockInstances.equal_range( blockName );
		for ( BlockInstanceConstIterator it = range.first; it != range.second; ++it )
		{
			++counter;
		}

		return counter;
	}

	void Bundle::setContextBlock( FunctionBlock< app::ContextBlockHandle > &context )
	{
		m_ContextBlock = &context;
	}
}
