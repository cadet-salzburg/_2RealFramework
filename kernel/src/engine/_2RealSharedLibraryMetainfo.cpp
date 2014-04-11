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

#include "engine/_2RealSharedLibraryMetainfo.h"
#include "engine/_2RealSharedServiceMetainfo.h"
#include "engine/_2RealSharedTypeMetainfo.h"
#include "engine/_2RealTypeCollection.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealId.h"
#include "engine/_2RealSharedLibrary.h"

#include "bundle/_2RealBundleMetainfo.h"
#include "bundle/_2RealFunctionBlockMetainfo.h"
#include "bundle/_2RealContextBlockMetainfo.h"
#include "bundle/_2RealTypeMetainfo.h"
#include "bundle/_2RealIoSlotMetainfo.h"

namespace _2Real
{
	std::shared_ptr< SharedLibraryMetainfo > SharedLibraryMetainfo::make( std::shared_ptr< SharedLibrary > lib, Path const& path, std::shared_ptr< TypeCollection > types )
	{
		typedef void ( *BundleMetainfoFunc )( bundle::BundleMetainfo & );
		typedef void ( *BlockMetainfoFunc )( bundle::BlockMetainfo &, std::map< std::string, const bundle::TypeMetainfo > const& );
		typedef void ( *TypeMetainfoFunc )( bundle::TypeMetainfo &, std::map< std::string, const bundle::TypeMetainfo > const& );

		if ( lib->hasSymbol( "getBundleMetainfo" ) )
		{
			BundleMetainfoFunc bundlefunc = ( BundleMetainfoFunc ) lib->getSymbol( "getBundleMetainfo" );

			std::shared_ptr< const MetainfoId > id = MetainfoId::create( nullptr, MetainfoType::BUNDLE, path.string() );
			std::shared_ptr< SharedLibraryMetainfo > bundleinfo( new SharedLibraryMetainfo( id, types ) );

			bundle::BundleMetainfo bundleMetainfo( bundleinfo );
			bundlefunc( bundleMetainfo );

			// TODO: check basic attribs

			std::map< std::string, const bundle::TypeMetainfo > previousTypes;

			if ( !bundleinfo->mTypePreinfos.empty() && lib->hasSymbol( "getTypeMetainfo" ) )
			{
				TypeMetainfoFunc typefunc = ( TypeMetainfoFunc ) lib->getSymbol( "getTypeMetainfo" );

				for ( auto const& it : bundleinfo->mTypePreinfos )
				{
					std::shared_ptr< SharedTypeMetainfo > typeinfo = SharedTypeMetainfo::make( id, types, it.mName );
					bundleinfo->mTypes[ it.mName ] = typeinfo;
					bundle::TypeMetainfo typeMetainfo( typeinfo );
					typefunc( typeMetainfo, previousTypes );
					previousTypes.insert( std::make_pair( it.mName, typeMetainfo ) );

					types->addType( typeinfo );
				}

				bundleinfo->mTypePreinfos.clear();
			}
			else if ( !bundleinfo->mTypePreinfos.empty() && !lib->hasSymbol( "getTypeMetainfo" ) )
			{
				throw NotFound( "function getTypeMetainfo not found in bundle" );
			}

			if ( bundleinfo->mBlockPreinfos.empty() )
			{	
				// EXCEPTION
			}
			else if ( !lib->hasSymbol( "getBlockMetainfo" ) )
			{
				// EXCEPTION
			}
			else
			{
				BlockMetainfoFunc blockfunc = ( BlockMetainfoFunc ) lib->getSymbol( "getBlockMetainfo" );

				for ( auto const& it : bundleinfo->mBlockPreinfos )
				{
					std::shared_ptr< SharedServiceMetainfo > blockinfo = SharedServiceMetainfo::make( id, types, it );

					bundleinfo->mBlocks[ it.mName] = blockinfo;
					if ( it.mIsSingleton )
					{
						bundle::ContextBlockMetainfo blockMetainfo( blockinfo );
						blockfunc( blockMetainfo, previousTypes );
					}
					else
					{
						bundle::FunctionBlockMetainfo blockMetainfo( blockinfo );
						blockfunc( blockMetainfo, previousTypes );
					}
				}

				bundleinfo->mBlockPreinfos.clear();
			}

			return bundleinfo;
		}
		else
		{
			return nullptr;
		}
	}

	SharedLibraryMetainfo::SharedLibraryMetainfo( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types ) :
		mId( id ),
		mCategory( "no category available" ),
		mDescription( "no description available" ),
		mAuthor( "no author available" ),
		mContact( "no contact available" ),
		mVersion( 0, 0, 0 ),
		mTypeCollection( types ),
		mTypes(),
		mBlocks()
	{
	}

	std::string SharedLibraryMetainfo::getDescription() const
	{
		return mDescription;
	}

	std::string SharedLibraryMetainfo::getAuthor() const
	{
		return mAuthor;
	}

	std::string SharedLibraryMetainfo::getContact() const
	{
		return mContact;
	}

	std::string SharedLibraryMetainfo::getCategory() const
	{
		return mCategory;
	}

	Version SharedLibraryMetainfo::getVersion() const
	{
		return mVersion;
	}

	std::shared_ptr< const MetainfoId > SharedLibraryMetainfo::getId() const
	{
		return mId;
	}

	void SharedLibraryMetainfo::setDescription( std::string const& description )
	{
		mDescription = description;
	}

	void SharedLibraryMetainfo::setAuthor( std::string const& author )
	{
		mAuthor = author;
	}

	void SharedLibraryMetainfo::setContact( std::string const& contact )
	{
		mContact = contact;
	}

	void SharedLibraryMetainfo::setCategory( std::string const& category )
	{
		mCategory = category;
	}

	void SharedLibraryMetainfo::setVersion( Version const& version )
	{
		mVersion = version;
	}

	void SharedLibraryMetainfo::exportType( const std::string name, const std::vector< FieldDeclaration > fields )
	{
		for ( auto const& it : mTypePreinfos )
		{
			if ( it.mName == name )
			{
				std::ostringstream msg;
				msg << "a type named " << name << " was already exported by this bundle" << std::endl;
				throw AlreadyExists( msg.str() );
			}
		}

		TypeDeclaration typeDecl;
		typeDecl.mName = name;
		typeDecl.mFields = fields;
		mTypePreinfos.push_back( typeDecl );
	}

	void SharedLibraryMetainfo::exportBlock( const std::string name, const bool isSingleton, const std::vector< InletDeclaration > inlets, const std::vector< OutletDeclaration > outlets, const std::vector< ParameterDeclaration > parameters )
	{
		for ( auto const& it : mBlockPreinfos )
		{
			if ( it.mName == name )
			{
				std::ostringstream msg;
				msg << "a block named " << name << " was already exported by this bundle" << std::endl;
				throw AlreadyExists( msg.str() );
			}
		}

		BlockDeclaration blockDecl;
		blockDecl.mName = name;
		blockDecl.mIsSingleton = isSingleton;
		blockDecl.mInlets = inlets;
		blockDecl.mOutlets = outlets;
		blockDecl.mParameters = parameters;
		mBlockPreinfos.push_back( blockDecl );
	}

	std::vector< std::shared_ptr< const SharedServiceMetainfo > > SharedLibraryMetainfo::getServiceMetainfos() const
	{
		std::vector< std::shared_ptr< const SharedServiceMetainfo > > tmp;
		tmp.reserve( mBlocks.size() );
		for ( auto it : mBlocks )
			tmp.push_back( it.second );
		return tmp;
	}

	std::vector< std::shared_ptr< const SharedTypeMetainfo > > SharedLibraryMetainfo::getTypeMetainfos() const
	{
		std::vector< std::shared_ptr< const SharedTypeMetainfo > > tmp;
		tmp.reserve( mTypes.size() );
		for ( auto it : mTypes )
			tmp.push_back( it.second );
		return tmp;
	}

	std::shared_ptr< const SharedServiceMetainfo > SharedLibraryMetainfo::getServiceMetainfo( std::string const& name ) const
	{
		auto it = mBlocks.find( name );
		if ( it == mBlocks.end() )
		{
			std::ostringstream msg;
			msg << "block " << name << " is not exported by " << mId->getName() << std::endl;
			throw NotFound( msg.str() );
		}

		return it->second;
	}

	std::shared_ptr< const SharedTypeMetainfo > SharedLibraryMetainfo::getTypeMetainfo( std::string const& name ) const
	{
		auto it = mTypes.find( name );
		if ( it == mTypes.end() )
		{
			std::ostringstream msg;
			msg << "type " << name << " is not exported by " << mId->getName() << std::endl;
			throw NotFound( msg.str() );
		}

		return it->second;
	}

}