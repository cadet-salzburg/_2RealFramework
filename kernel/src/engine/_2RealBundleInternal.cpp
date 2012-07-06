
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

#include "engine/_2RealBundleInternal.h"
#include "app/_2RealBundleHandle.h"
#include "engine/_2RealBundleData.h"
#include "engine/_2RealBlockData.h"
#include "../_2RealBlock.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealBundleData.h"
#include "app/_2RealBlockData.h"
#include "app/_2RealParameterData.h"
#include "engine/_2RealBundleManager.h"
#include "engine/_2RealFunctionBlock.h"

#include <sstream>

using std::string;
using std::ostringstream;
using std::make_pair;

namespace _2Real
{

	Bundle::Bundle( Identifier const& id, BundleData const& data, BundleManager &bundleManager ) :
		HandleAble< app::BundleHandle >( *this ),
		m_BundleManager( bundleManager ),
		m_Identifier( id ),
		m_Metadata( data )
	{
	}

	Bundle::~Bundle()
	{
		clear();
	}

	void Bundle::clear()
	{
		for ( BlockIterator it = m_BlockInstances.begin(); it != m_BlockInstances.end(); /**/ )
		{
			delete it->second;
			it = m_BlockInstances.erase( it );
		}
	}

	Identifier const& Bundle::getIdentifier() const
	{
		return m_Identifier;
	}
	
	std::string const& Bundle::getName() const
	{
		return m_Identifier.getName();
	}

	app::BundleHandle Bundle::createHandle()
	{
		return app::BundleHandle( *this );
	}

	app::BundleInfo Bundle::getBundleData() const
	{
		app::BundleInfo bundleData;

		bundleData.m_Author = m_Metadata.getAuthor();
		bundleData.m_Category = m_Metadata.getCategory();

		BundleData::BlockMetas const& blocks = m_Metadata.getExportedBlocks();

		for ( BundleData::BlockMetaConstIterator it = blocks.begin(); it != blocks.end(); ++it )
		{
			app::BlockInfo blockData;

			blockData.m_Name = it->second.getName();
			blockData.m_Description = it->second.getDescription();
			blockData.m_Category = it->second.getCategory();

			BlockData::ParamMetas const& input = it->second.getInlets();
			BlockData::ParamMetas const& output = it->second.getOutlets();

			for ( BlockData::ParamMetaConstIterator it = input.begin(); it != input.end(); ++it )
			{
				app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
				blockData.m_Inlets.push_back( paramInfo );
			}

			for ( BlockData::ParamMetaConstIterator it = output.begin(); it != output.end(); ++it )
			{
				app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
				blockData.m_Outlets.push_back( paramInfo );
			}

			bundleData.m_ExportedBlocks.push_back( blockData );
		}

		return bundleData;
	}

	BundleData const& Bundle::getMetadata() const
	{
		return m_Metadata;
	}

	app::BlockHandle & Bundle::createBlockInstance( std::string const& blockName )
	{
		// this is a bit strange, bundle mgr will call 'addBlockInstance'
		// = result of interface changes
		return m_BundleManager.createFunctionBlock( *this, blockName )->HandleAble< app::BlockHandle >::getHandle();
	}

	unsigned int Bundle::getBlockInstanceCount( string const& blockName ) const
	{
		unsigned int counter = 0;
		std::pair< BlockConstIterator, BlockConstIterator > range = m_BlockInstances.equal_range( blockName );
		for ( BlockConstIterator it = range.first; it != range.second; ++it )
		{
			++counter;
		}

		return counter;
	}

	void Bundle::addBlockInstance( bundle::Block &block, string const& blockName )
	{
		m_BlockInstances.insert( make_pair( blockName, &block ) );
	}

}