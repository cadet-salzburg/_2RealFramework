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

#include "_2RealMetadata.h"
#include "_2RealBundleData.h"
#include "_2RealParameterData.h"
#include "_2RealEngineImpl.h"
#include "_2RealTypetable.h"

#include <sstream>
#include <iostream>

namespace _2Real
{

	BundleMetainfo::BundleMetainfo( Metainfo &info, Typetable const& typetable ) :
		m_Impl( info ),
		m_Typetable( typetable )
	{
	}

	BundleMetainfo::~BundleMetainfo()
	{
	}

	BundleMetainfo::BundleMetainfo( BundleMetainfo const& src ) :
		m_Impl( src.m_Impl ),
		m_Typetable( src.m_Typetable )
	{
	}

	void BundleMetainfo::setDescription( std::string const& description )
	{
		m_Impl.setDescription( description );
	}

	void BundleMetainfo::setVersion( unsigned int major, unsigned int minor, unsigned int revision )
	{
		m_Impl.setVersion( Version( major, minor, revision ) );
	}

	void BundleMetainfo::setAuthor( std::string const& author )
	{
		m_Impl.setAuthor( author );
	}

	void BundleMetainfo::setContact( std::string const& contact )
	{
		m_Impl.setContact( contact );
	}

	void BundleMetainfo::setCategory( std::string const& category )
	{
		m_Impl.setCategory( category );
	}

	void BundleMetainfo::setBundleContextInternal( AbstractBlockCreator &obj )
	{
		m_Impl.setContextCreator( obj );
	}

	BlockMetainfo BundleMetainfo::addExportedBlockInternal( AbstractBlockCreator &obj, std::string const& blockName )
	{
		BlockData &data = m_Impl.addExportedBlock( obj, blockName );
		return BlockMetainfo( data, m_Typetable );
	}

	void BundleMetainfo::addContextParameterInternal( std::string const& paramName, EngineData const& defaultValue )
	{
		const std::string type = defaultValue.typeinfo().name();
		const std::string keyword = m_Typetable.lookupKey( type );
		ParameterData *data = new ParameterData( paramName, type, keyword, "context", defaultValue );
		m_Impl.addContextParameter( *data );
	}

	BlockMetainfo::BlockMetainfo( BlockData &data, Typetable const& typetable ) :
		m_Impl( data ),
		m_Typetable( typetable )
	{
	}

	BlockMetainfo::~BlockMetainfo()
	{
	}

	BlockMetainfo::BlockMetainfo( BlockMetainfo const& src ) :
		m_Impl( src.m_Impl ),
		m_Typetable( src.m_Typetable )
	{
	}

	void BlockMetainfo::setDescription( std::string const& description )
	{
		m_Impl.setDescription( description );
	}

	void BlockMetainfo::addSetupParameterInternal( std::string const& paramName, EngineData const& defaultValue )
	{
		const std::string type = defaultValue.typeinfo().name();
		const std::string keyword = m_Typetable.lookupKey( type );
		ParameterData *data = new ParameterData( paramName, type, keyword, "param", defaultValue );
		m_Impl.addParameter( *data );
	}

	void BlockMetainfo::addInletInternal( std::string const& inletName, EngineData const& defaultValue )
	{
		const std::string type = defaultValue.typeinfo().name();
		const std::string keyword = m_Typetable.lookupKey( type );
		ParameterData *data = new ParameterData( inletName, type, keyword, "inlet", defaultValue );
		m_Impl.addInlet( *data );
	}

	void BlockMetainfo::addOutletInternal( std::string const& outletName, EngineData const& defaultValue )
	{
		const std::string type = defaultValue.typeinfo().name();
		const std::string keyword = m_Typetable.lookupKey( type );
		ParameterData *data = new ParameterData( outletName, type, keyword, "outlet", defaultValue );
		m_Impl.addOutlet( *data );
	}

	Metainfo::Metainfo( BundleData &data ) :
		m_BundleData( &data ),
		m_ContextInfo(),
		m_HasContext( false )
	{
	}

	Metainfo::~Metainfo()
	{
		for ( BlockInfoMap::iterator it = m_ExportedBlocks.begin(); it != m_ExportedBlocks.end(); /**/ )
		{
			std::string name = it->second.data->getName();
			delete it->second.data;
			delete it->second.ctor;
			it = m_ExportedBlocks.erase( it );
		}

		delete m_BundleData;
	}

	void Metainfo::setAuthor( std::string const& author )
	{
		m_BundleData->setAuthor( author );
	}

	void Metainfo::setDescription( std::string const& description )
	{
		m_BundleData->setDescription( description );
	}

	void Metainfo::setCategory( std::string const& category )
	{
		m_BundleData->setCategory( category );
	}

	void Metainfo::setVersion( Version const& version )
	{
		m_BundleData->setVersion( version );
	}

	void Metainfo::setContact( std::string const& contact )
	{
		m_BundleData->setContact( contact );
	}

	void Metainfo::setInstallDirectory( std::string const& path )
	{
		m_BundleData->setInstallDirectory( path );
	}

	void Metainfo::setName( std::string const& name )
	{
		m_BundleData->setName( name );
	}

	void Metainfo::setContextCreator( AbstractBlockCreator &obj )
	{
		m_HasContext = true;
		if ( m_ContextInfo.data != NULL )
		{
			delete m_ContextInfo.ctor;
			delete m_ContextInfo.data;
		}

		m_ContextInfo.ctor = &obj;
		m_ContextInfo.data = new BlockData( "bundle context" );
		m_ContextInfo.data->setDescription( "context manager for exported blocks" );
	}

	BlockData & Metainfo::addExportedBlock( AbstractBlockCreator &obj, std::string const& blockName )
	{
		if ( m_ExportedBlocks.find( blockName ) != m_ExportedBlocks.end() )
		{
			BlockInfo &info = m_ExportedBlocks[ blockName ];
			delete info.ctor;
			delete info.data;
		}

		BlockInfo info;
		info.ctor = &obj;
		info.data = new BlockData( blockName );
		m_ExportedBlocks[ blockName ] = info;

		return *( info.data );
	}

	void Metainfo::addContextParameter( ParameterData &data )
	{
		m_ContextInfo.data->addOutlet( data );
	}

	bool Metainfo::hasContext() const
	{
		return m_HasContext;
	}

	//Block & Metainfo::createBundleContext() const
	//{
	//	if ( m_ContextInfo.ctor != NULL )
	//	{
	//		return m_ContextInfo.ctor->create();
	//	}
	//	else
	//	{
	//		std::ostringstream msg;
	//		msg << "no bundle context declared in " << m_BundleData.getName();
	//		throw NotFoundException( msg.str() );
	//	}
	//}

	Block & Metainfo::createBlock( std::string const& blockName ) const
	{
		BlockInfoMap::const_iterator it = m_ExportedBlocks.find( blockName );
		if ( it != m_ExportedBlocks.end() )
		{
			return it->second.ctor->create();
		}
		else
		{
			std::ostringstream msg;
			msg << "block " << blockName << " no exported by " << m_BundleData->getName();
			throw NotFoundException( msg.str() );
		}
	}

	void Metainfo::postProcess()
	{
		std::map< std::string, ParameterData const* > params;
		if ( m_HasContext )
		{
			m_BundleData->addBlockData( *m_ContextInfo.data );
			params = m_ContextInfo.data->getOutlets();
		}

		for ( std::map< std::string, BlockInfo >::iterator bIt = m_ExportedBlocks.begin(); bIt != m_ExportedBlocks.end(); ++bIt )
		{
			if ( m_HasContext )
			{
				for ( std::map< std::string, ParameterData const* >::iterator it = params.begin(); it != params.end(); ++it )
				{
					ParameterData const* out = it->second;
					ParameterData const *in= new ParameterData( out->getName(), out->getType(), out->getKeyword(), out->getCategory(), out->getDefaultValue() );
					bIt->second.data->addInlet( *in );
				}
			}

			m_BundleData->addBlockData( *bIt->second.data );
		}

		if ( m_HasContext )
		{
			m_ExportedBlocks.insert( std::make_pair( "bundle context", m_ContextInfo ) );
			m_ContextInfo.ctor = NULL;
			m_ContextInfo.data = NULL;
		}

		std::cout << *m_BundleData << std::endl;

	}

}