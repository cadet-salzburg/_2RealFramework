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
#include "engine/_2RealBlockMetainfoImpl.h"
#include "engine/_2RealCustomTypeMetainfoImpl.h"
#include "engine/_2RealTypeCollection.h"
#include "common/_2RealException.h"
#include "engine/_2RealId.h"
#include "engine/_2RealSharedLibrary.h"

#include "bundle/_2RealBundleMetainfo.h"
#include "bundle/_2RealBlockMetainfo.h"
#include "bundle/_2RealCustomTypeMetainfo.h"
#include "bundle/_2RealIoSlotMetainfo.h"

namespace _2Real
{
	class BlockMetainfoNameCmp : public std::unary_function< bool, std::shared_ptr< const BlockMetainfoImpl > >
	{

	public:

		explicit BlockMetainfoNameCmp( const std::string name ) : mName( name )
		{
		}

		bool operator()( std::shared_ptr< const BlockMetainfoImpl > obj ) const
		{
			return mName == obj->getName();
		}

	private:

		std::string mName;

	};

	class TypeMetainfoNameCmp : public std::unary_function< bool, std::shared_ptr< const CustomTypeMetainfoImpl > >
	{

	public:

		explicit TypeMetainfoNameCmp( const std::string name ) : mName( name )
		{
		}

		bool operator()( std::shared_ptr< const CustomTypeMetainfoImpl > obj ) const
		{
			return mName == obj->getName();
		}

	private:

		std::string mName;

	};

	std::shared_ptr< BundleMetainfoImpl > BundleMetainfoImpl::make( std::shared_ptr< SharedLibrary > lib, Path const& path, std::shared_ptr< TypeCollection > types )
	{
		typedef void ( *BundleMetainfoFunc )( bundle::BundleMetainfo & );
		typedef void ( *BlockMetainfoFunc )( bundle::BlockMetainfo &, std::vector< const bundle::CustomTypeMetainfo > const& );
		typedef void ( *TypeMetainfoFunc )( bundle::CustomTypeMetainfo &, std::vector< const bundle::CustomTypeMetainfo > const& );

		if ( lib->hasSymbol( "getBundleMetainfo" ) )
		{
			BundleMetainfoFunc bundlefunc = ( BundleMetainfoFunc ) lib->getSymbol( "getBundleMetainfo" );

			std::shared_ptr< const MetainfoId > id = MetainfoId::create( nullptr, MetainfoType::BUNDLE, path.string() );
			std::shared_ptr< BundleMetainfoImpl > bundleinfo( new BundleMetainfoImpl( id, types ) );

			bundle::BundleMetainfo bundleMetainfo( bundleinfo );
			bundlefunc( bundleMetainfo );

			// TODO: check basic attribs

			std::vector< const bundle::CustomTypeMetainfo > previousTypes;

			if ( !bundleinfo->mTypePreinfos.empty() && lib->hasSymbol( "getTypeMetainfo" ) )
			{
				TypeMetainfoFunc typefunc = ( TypeMetainfoFunc ) lib->getSymbol( "getTypeMetainfo" );

				for ( auto const& it : bundleinfo->mTypePreinfos )
				{
					std::shared_ptr< CustomTypeMetainfoImpl > typeinfo = CustomTypeMetainfoImpl::make( id, types, it.mName );
					bundleinfo->mTypes.push_back( typeinfo );
					bundle::CustomTypeMetainfo typeMetainfo( typeinfo );
					typefunc( typeMetainfo, previousTypes );
					previousTypes.push_back( typeMetainfo );

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
				// TODO: EXCEPTION
			}
			else if ( !lib->hasSymbol( "getBlockMetainfo" ) )
			{
				// TODO: EXCEPTION
			}
			else
			{
				BlockMetainfoFunc blockfunc = ( BlockMetainfoFunc ) lib->getSymbol( "getBlockMetainfo" );

				for ( auto const& it : bundleinfo->mBlockPreinfos )
				{
					std::shared_ptr< BlockMetainfoImpl > blockinfo = BlockMetainfoImpl::make( id, types, it );

					bundleinfo->mBlocks.push_back( blockinfo );
					bundle::BlockMetainfo blockMetainfo( blockinfo );
					blockfunc( blockMetainfo, previousTypes );
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

	BundleMetainfoImpl::BundleMetainfoImpl( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types ) :
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

	std::string BundleMetainfoImpl::getDescription() const
	{
		return mDescription;
	}

	std::string BundleMetainfoImpl::getAuthor() const
	{
		return mAuthor;
	}

	std::string BundleMetainfoImpl::getContact() const
	{
		return mContact;
	}

	std::string BundleMetainfoImpl::getCategory() const
	{
		return mCategory;
	}

	Version BundleMetainfoImpl::getVersion() const
	{
		return mVersion;
	}

	std::shared_ptr< const MetainfoId > BundleMetainfoImpl::getId() const
	{
		return mId;
	}

	void BundleMetainfoImpl::setDescription( std::string const& description )
	{
		mDescription = description;
	}

	void BundleMetainfoImpl::setAuthor( std::string const& author )
	{
		mAuthor = author;
	}

	void BundleMetainfoImpl::setContact( std::string const& contact )
	{
		mContact = contact;
	}

	void BundleMetainfoImpl::setCategory( std::string const& category )
	{
		mCategory = category;
	}

	void BundleMetainfoImpl::setVersion( Version const& version )
	{
		mVersion = version;
	}

	void BundleMetainfoImpl::exportType( const std::string name, const std::vector< FieldDeclaration > fields )
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

	void BundleMetainfoImpl::exportBlock( const std::string name, const std::vector< InletDeclaration > inlets, const std::vector< OutletDeclaration > outlets, const std::vector< ParameterDeclaration > parameters )
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
		blockDecl.mInlets = inlets;
		blockDecl.mOutlets = outlets;
		blockDecl.mParameters = parameters;
		mBlockPreinfos.push_back( blockDecl );
	}

	std::vector< std::shared_ptr< const BlockMetainfoImpl > > BundleMetainfoImpl::getServiceMetainfos() const
	{
		std::vector< std::shared_ptr< const BlockMetainfoImpl > > tmp;
		tmp.reserve( mBlocks.size() );
		for ( auto it : mBlocks )
			tmp.push_back( it );
		return tmp;
	}

	std::vector< std::shared_ptr< const CustomTypeMetainfoImpl > > BundleMetainfoImpl::getTypeMetainfos() const
	{
		std::vector< std::shared_ptr< const CustomTypeMetainfoImpl > > tmp;
		tmp.reserve( mTypes.size() );
		for ( auto it : mTypes )
			tmp.push_back( it );
		return tmp;
	}

	std::shared_ptr< const BlockMetainfoImpl > BundleMetainfoImpl::getServiceMetainfo( std::string const& name ) const
	{
		auto it = std::find_if( mBlocks.begin(), mBlocks.end(), BlockMetainfoNameCmp( name ) );
		if ( it == mBlocks.end() )
		{
			std::ostringstream msg;
			msg << "block " << name << " is not exported by " << mId->getName() << std::endl;
			throw NotFound( msg.str() );
		}

		return *it;
	}

	std::shared_ptr< const CustomTypeMetainfoImpl > BundleMetainfoImpl::getTypeMetainfo( std::string const& name ) const
	{
		auto it = std::find_if( mTypes.begin(), mTypes.end(), TypeMetainfoNameCmp( name ) );
		if ( it == mTypes.end() )
		{
			std::ostringstream msg;
			msg << "type " << name << " is not exported by " << mId->getName() << std::endl;
			throw NotFound( msg.str() );
		}

		return *it;
	}

}