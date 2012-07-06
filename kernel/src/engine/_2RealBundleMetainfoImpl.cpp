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

#include "engine/_2RealBundleMetainfoImpl.h"
#include "bundle/_2RealCreationPolicy.h"
#include "engine/_2RealBundleData.h"
#include "engine/_2RealBlockData.h"
#include "engine/_2RealTypetable.h"
#include "bundle/_2RealBlockMetainfo.h"
#include "bundle/_2RealContextBlockMetainfo.h"
#include "helpers/_2RealHelpersInternal.h"

#include <sstream>

using std::string;
using std::ostringstream;
using std::make_pair;
using std::map;

namespace _2Real
{

	Metainfo::Metainfo( Typetable const& typetable ) :
		m_HasContext( false ),
		m_Typetable( typetable )
	{
	}

	Metainfo::~Metainfo()
	{
		for ( BlockInfoIterator it = m_BlockInfos.begin(); it != m_BlockInfos.end(); ++it )
		{
			safeDelete( it->second.ctor );
			safeDelete( it->second.data );
			safeDelete( it->second.meta );
		}

		safeDelete( m_ContextInfo.ctor );
		safeDelete( m_ContextInfo.data );
		safeDelete( m_ContextInfo.meta );
	}

	bool Metainfo::hasContext() const
	{
		return m_HasContext;
	}

	BundleData const& Metainfo::getBundleData() const
	{
		return m_BundleData;
	}

	void Metainfo::setAuthor( string const& author )
	{
		m_BundleData.setAuthor( author );
	}

	void Metainfo::setDescription( string const& description )
	{
		m_BundleData.setDescription( description );
	}

	void Metainfo::setCategory( string const& category )
	{
		m_BundleData.setCategory( category );
	}

	void Metainfo::setVersion( Version const& version )
	{
		m_BundleData.setVersion( version );
	}

	void Metainfo::setContact( string const& contact )
	{
		m_BundleData.setContact( contact );
	}

	void Metainfo::setInstallDirectory( string const& path )
	{
		m_BundleData.setInstallDirectory( path );
	}

	void Metainfo::setName( string const& name )
	{
		m_BundleData.setName( name );
	}

	bundle::ContextBlockMetainfo & Metainfo::setContextBlockCreator( bundle::AbstractBlockCreator &obj )
	{
		if ( m_HasContext )
		{
			ostringstream msg;
			msg << "bundles' context block was already exported " << m_BundleData.getName();
			throw AlreadyExistsException( msg.str() );
		}

		m_ContextInfo.ctor = &obj;
		// TODO: reserve name for context
		m_ContextInfo.data = new BlockData( "bundle context" );
		m_ContextInfo.data->setDescription( "context for all other blocks exported by the bundle" );
		m_ContextInfo.meta = new bundle::ContextBlockMetainfo( *m_ContextInfo.data );

		m_HasContext = true;

		return *m_ContextInfo.meta;
	}

	bundle::BlockMetainfo & Metainfo::setBlockCreator( string const& blockName, bundle::AbstractBlockCreator &obj )
	{
		if ( m_BlockInfos.find( blockName ) != m_BlockInfos.end() )
		{
			ostringstream msg;
			msg << "block " << blockName << " was exported already " << m_BundleData.getName();
			throw AlreadyExistsException( msg.str() );
		}

		m_BlockInfos[ blockName ].ctor = &obj;
		BlockData *data = new BlockData( blockName );
		m_BlockInfos[ blockName ].data = data;
		bundle::BlockMetainfo *meta = new bundle::BlockMetainfo( *data, m_Typetable );
		m_BlockInfos[ blockName ].meta = meta;

		return *meta;
	}

	bundle::Block & Metainfo::createContextBlock() const
	{
		if ( m_HasContext )
		{
			return m_ContextInfo.ctor->create( nullptr );
		}
		else
		{
			ostringstream msg;
			msg << "no bundle context declared in " << m_BundleData.getName();
			throw NotFoundException( msg.str() );
		}
	}

	bundle::Block & Metainfo::createBlock( string const& blockName ) const
	{
		bundle::ContextBlock *context = nullptr;

		if ( m_HasContext )
		{
			context = dynamic_cast< bundle::ContextBlock * > ( &( m_ContextInfo.ctor->create( nullptr ) ) );
		}

		BlockInfoConstIterator it = m_BlockInfos.find( blockName );
		if ( it != m_BlockInfos.end() )
		{
			return it->second.ctor->create( context );
		}
		else
		{
			ostringstream msg;
			msg << "block " << blockName << " is not exported by " << m_BundleData.getName();
			throw NotFoundException( msg.str() );
		}
	}

	void Metainfo::cleanup()
	{
		for ( BlockInfoIterator it = m_BlockInfos.begin(); it != m_BlockInfos.end(); ++it )
		{
			m_BundleData.addBlockData( *it->second.data );
		}

		if ( m_HasContext )
		{
			m_BundleData.addBlockData( *m_ContextInfo.data );
		}
	}

}