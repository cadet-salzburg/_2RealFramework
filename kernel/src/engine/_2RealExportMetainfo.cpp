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

namespace _2Real
{
	const std::string Metainfo::sContextBlockName = "ContextBlock";

	Metainfo::Metainfo( std::string const& id, TypeRegistry *init ) :
		mBundleId( id ),
		mBundleMetadata( new BundleMetadata( this ) ),
		mTypes( new TypeRegistry() ),
		mFrameworkTypes( init )
	{
		// get all framework types into the registry
		// they will be known as framework types
		// bundle types will instead be added with the bundle id....
		mTypes->merge( *init, TypeRegistry::sFrameworkTypes, TypeRegistry::sFrameworkTypes );
	}

	Metainfo::~Metainfo()
	{
		for ( BlockInfoIterator it = mBlockMetadata.begin(); it != mBlockMetadata.end(); ++it )
			delete it->second.ctor;
		delete mTypes;
	}

	std::shared_ptr< BundleMetadata > Metainfo::getBundleMetadata()
	{
		return mBundleMetadata;
	}

	bool Metainfo::exportsContext() const
	{
		return ( mBlockMetadata.find( sContextBlockName ) != mBlockMetadata.end() );
	}

	bool Metainfo::hasContext() const
	{
		BlockInfos::const_iterator it = mBlockMetadata.find( sContextBlockName );
		if ( it == mBlockMetadata.end() )	return false;
		else								return ( it->second.ctor->getCreationCount() > 0 );
	}

	unsigned int Metainfo::getCreationCount( std::string const& name ) const
	{
		BlockInfos::const_iterator bIt = mBlockMetadata.end();
		for ( BlockInfoConstIterator it = mBlockMetadata.begin(); it != mBlockMetadata.end(); ++it )
		{
			if ( toLower( it->first ) == toLower( name ) )
				bIt = it;
		}

		if ( bIt == mBlockMetadata.end() )
		{
			std::ostringstream msg;
			msg << "block " << name << " is not exported by " << mBundleMetadata->getName();
			throw NotFoundException( msg.str() );
		}

		return bIt->second.ctor->getCreationCount();
	}

	std::shared_ptr< TypeMetadata > Metainfo::exportCustomType( std::string const& name )
	{
		std::shared_ptr< const TypeMetadata > test = mFrameworkTypes->get( TypeRegistry::sFrameworkTypes, name );
		if ( nullptr != test )
		{
			std::ostringstream msg;
			msg << "type name " << name << " is reserved for a framework type" << std::endl;
			throw AlreadyExistsException( msg.str() );
		}

		std::shared_ptr< TypeMetadata > m( new TypeMetadata( TypeMetadata::TypeId( mBundleId, name ), mTypes ) );
		mTypes->registerType( mBundleId, name, m );
		return m;
	}

	std::shared_ptr< BlockMetadata > Metainfo::exportContextBlock( bundle::AbstractBlockCreator *obj )
	{
		return exportFunctionBlock( obj, sContextBlockName );
	}

	std::shared_ptr< BlockMetadata > Metainfo::exportFunctionBlock( bundle::AbstractBlockCreator *obj, std::string const& name )
	{
		for ( BlockInfoConstIterator it = mBlockMetadata.begin(); it != mBlockMetadata.end(); ++it )
		{
			if ( toLower( it->first ) == toLower( name ) )
			{
				std::ostringstream msg;
				msg << "block " << name << " is already defined in bundle " << mBundleMetadata->getName();
				throw AlreadyExistsException( msg.str() );
			}
		}

		bool isContext = ( name == sContextBlockName );
		bool needsContext = obj->needsContext();

		BlockInfo b;
		b.ctor = obj;
		b.metadata.reset( new BlockMetadata( BlockMetadata::BlockId( mBundleId, name ), mTypes, isContext, needsContext ) );

		mBlockMetadata[ name ] = b;

		return b.metadata;
	}

	std::shared_ptr< bundle::Block > Metainfo::createContextBlock() const
	{
		BlockInfos::const_iterator it = mBlockMetadata.find( sContextBlockName );
#ifdef _DEBUG
		if ( it == mBlockMetadata.end() )
			assert( NULL );
#endif

		return it->second.ctor->create( nullptr );
	}

	std::shared_ptr< bundle::Block > Metainfo::createFunctionBlock( std::string const& name ) const
	{
		BlockInfos::const_iterator bIt = mBlockMetadata.end();
		for ( BlockInfoConstIterator it = mBlockMetadata.begin(); it != mBlockMetadata.end(); ++it )
		{
			if ( toLower( it->first ) == toLower( name ) )
				bIt = it;
		}

		if ( bIt == mBlockMetadata.end() )
		{
			std::ostringstream msg;
			msg << "block " << name << " is not exported by " << mBundleMetadata->getName();
			throw NotFoundException( msg.str() );
		}

		std::shared_ptr< bundle::ContextBlock > context;
		BlockInfos::const_iterator cIt = mBlockMetadata.find( sContextBlockName );
		if ( cIt != mBlockMetadata.end() )
		{
			context = std::dynamic_pointer_cast< _2Real::bundle::ContextBlock >( cIt->second.ctor->create( nullptr ) );
		}

		return bIt->second.ctor->create( context );
	}

	void Metainfo::getExportedBlocks( std::vector< std::shared_ptr< const BlockMetadata > > &meta ) const
	{
		meta.clear(); meta.reserve( mBlockMetadata.size() );
		for ( BlockInfoConstIterator it = mBlockMetadata.begin(); it != mBlockMetadata.end(); ++it )
			meta.push_back( it->second.metadata );
	}

	void Metainfo::postLoading( TypeRegistry *registry )
	{
		if ( mBundleMetadata->getName().empty() )
		{
			throw NotFoundException( "bundle has no name" );
		}

		//registry->merge( *mTypes, mBundleId, mBundleId );
	}

	std::shared_ptr< const BlockMetadata > Metainfo::getContextBlockMetadata() const
	{
		BlockInfoConstIterator it = mBlockMetadata.find( sContextBlockName );
		if ( it == mBlockMetadata.end() )
			return nullptr;
		else return it->second.metadata;
	}

	std::shared_ptr< const BlockMetadata > Metainfo::getFunctionBlockMetadata( std::string const& name ) const
	{
		BlockInfoConstIterator bIt = mBlockMetadata.end();
		for ( BlockInfoConstIterator it = mBlockMetadata.begin(); it != mBlockMetadata.end(); ++it )
		{
			if ( toLower( it->first ) == toLower( name ) )
			{
				bIt = it;
				break;
			}
		}

		if ( bIt == mBlockMetadata.end() )
		{
			std::ostringstream msg;
			msg << "block " << name << " is not exported by " << mBundleMetadata->getName();
			throw NotFoundException( msg.str() );
		}
		else return bIt->second.metadata;
	}
}