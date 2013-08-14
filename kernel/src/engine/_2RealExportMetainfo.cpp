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

#include "engine/_2RealExportMetainfo.h"
#include "engine/_2RealIOMetadata.h"
#include "engine/_2RealTypeMetadata.h"
#include "bundle/_2RealCreationPolicy.h"
#include "helpers/_2RealException.h"
#include "datatypes/_2RealTypeRegistry.h"
#include "helpers/_2RealConstants.h"

namespace _2Real
{
	Metainfo::Metainfo( TypeRegistry *init, Poco::Path const& p, std::shared_ptr< TemplateId > id ) :
		mBundleMetadata( new BundleMetadata( this ) ),
		mExportedTypes( new TypeRegistry ),
		mFrameworkTypes( init ),
		mIdentifier( id )
	{
		// get all framework types into the registry
		// they will be known as framework types
		// bundle types will instead be added with the bundle id....
		mBundleMetadata->setInstallDirectory( p );
		//mTypes->merge( *init, Constants::FrameworkTypename, Constants::FrameworkTypename );
	}

	Metainfo::~Metainfo()
	{
		for ( BlockInfoIterator it = mBlockMetadata.begin(); it != mBlockMetadata.end(); ++it )
			delete it->second.ctor;
		delete mExportedTypes;
	}

	std::shared_ptr< const TemplateId > Metainfo::getIdentifier() const
	{
		return mIdentifier;
	}

	std::shared_ptr< BundleMetadata > Metainfo::getBundleMetadata()
	{
		return mBundleMetadata;
	}

	bool Metainfo::exportsContext() const
	{
		return ( mBlockMetadata.find( Constants::ContextBlockName ) != mBlockMetadata.end() );
	}

	bool Metainfo::hasContext() const
	{
		BlockInfos::const_iterator it = mBlockMetadata.find( Constants::ContextBlockName );
		if ( it == mBlockMetadata.end() )	return false;
		else								return ( it->second.ctor->getCreationCount() > 0 );
	}

	unsigned int Metainfo::getCreationCount( std::string const& name ) const
	{
		BlockInfos::const_iterator it = mBlockMetadata.find( name );
		if ( it == mBlockMetadata.end() )
		{
			std::ostringstream msg;
			msg << "block " << name << " is not exported by " << mBundleMetadata->getName();
			throw NotFoundException( msg.str() );
		}

		return it->second.ctor->getCreationCount();
	}

	std::shared_ptr< TypeMetadata > Metainfo::exportCustomType( std::string const& name )
	{
		std::shared_ptr< TemplateId > typeId = IdGenerator::makeExportedTypeId( mIdentifier, name );

		std::shared_ptr< const TypeMetadata > test = mFrameworkTypes->get( name );
		if ( nullptr != test.get() )
		{
			std::ostringstream msg;
			msg << "type name " << name << " is reserved for a framework type" << std::endl;
			throw AlreadyExistsException( msg.str() );
		}

		std::shared_ptr< TypeMetadata > meta( new TypeMetadata( typeId, mFrameworkTypes, mExportedTypes ) );
		mExportedTypes->registerType( meta );
		return meta;
	}

	std::shared_ptr< BlockMetadata > Metainfo::exportContextBlock( bundle::AbstractBlockCreator *obj )
	{
		return exportFunctionBlock( obj, Constants::ContextBlockName );
	}

	std::shared_ptr< BlockMetadata > Metainfo::exportFunctionBlock( bundle::AbstractBlockCreator *obj, std::string const& name )
	{
		BlockInfos::const_iterator it = mBlockMetadata.find( name );
		if ( it != mBlockMetadata.end() )
		{
			std::ostringstream msg;
			msg << "block " << name << " is already defined in bundle " << mBundleMetadata->getName();
			throw AlreadyExistsException( msg.str() );
		}

		bool isContext = ( name == Constants::ContextBlockName );
		bool needsContext = obj->needsContext();

		BlockInfo b;
		b.ctor = obj;
		std::shared_ptr< TemplateId > blockId = IdGenerator::makeExportedBlockId( mIdentifier, name );
		b.metadata.reset( new BlockMetadata( blockId, this, mFrameworkTypes, mExportedTypes, isContext, needsContext ) );

		mBlockMetadata[ blockId->getObjectName() ] = b;

		return b.metadata;
	}

	std::shared_ptr< bundle::Block > Metainfo::createContextBlock() const
	{
		BlockInfos::const_iterator it = mBlockMetadata.find( Constants::ContextBlockName );
		return it->second.ctor->create( nullptr );
	}

	std::shared_ptr< bundle::Block > Metainfo::createFunctionBlock( std::string const& name ) const
	{
		BlockInfos::const_iterator it = mBlockMetadata.find( name );
		if ( it == mBlockMetadata.end() )
		{
			std::ostringstream msg;
			msg << "block " << name << " is not exported by " << mBundleMetadata->getName();
			throw NotFoundException( msg.str() );
		}

		std::shared_ptr< bundle::ContextBlock > context;
		BlockInfos::const_iterator cIt = mBlockMetadata.find( Constants::ContextBlockName );
		if ( cIt != mBlockMetadata.end() )
		{
			context = std::dynamic_pointer_cast< _2Real::bundle::ContextBlock >( cIt->second.ctor->create( nullptr ) );
		}

		return it->second.ctor->create( context );
	}

	void Metainfo::getExportedBlocks( std::vector< std::shared_ptr< const BlockMetadata > > &meta ) const
	{
		meta.clear(); meta.reserve( mBlockMetadata.size() );
		for ( BlockInfoConstIterator it = mBlockMetadata.begin(); it != mBlockMetadata.end(); ++it )
			meta.push_back( it->second.metadata );
	}

	std::shared_ptr< const BlockMetadata > Metainfo::getContextBlockMetadata() const
	{
		BlockInfoConstIterator it = mBlockMetadata.find( Constants::ContextBlockName );
		if ( it == mBlockMetadata.end() )
			return nullptr;
		else return it->second.metadata;
	}

	std::shared_ptr< const BlockMetadata > Metainfo::getFunctionBlockMetadata( std::string const& name ) const
	{
		BlockInfos::const_iterator it = mBlockMetadata.find( name );
		if ( it == mBlockMetadata.end() )
		{
			std::ostringstream msg;
			msg << "block " << name << " is not exported by " << mBundleMetadata->getName();
			throw NotFoundException( msg.str() );
		}
		return it->second.metadata;
	}
}