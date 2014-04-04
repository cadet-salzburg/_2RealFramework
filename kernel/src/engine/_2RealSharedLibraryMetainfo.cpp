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
			std::shared_ptr< SharedLibraryMetainfo > bundleinfo( new SharedLibraryMetainfo( path, id, types ) );

			bundle::BundleMetainfo bundleMetainfo( bundleinfo );
			bundlefunc( bundleMetainfo );

			// TODO: check basic attribs

			std::map< std::string, const bundle::TypeMetainfo > previousTypes;
			std::cout << bundleinfo->mTypePreinfos.size() << std::endl;

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
					std::shared_ptr< SharedServiceMetainfo > blockinfo = SharedServiceMetainfo::make( id, types, it.mName, it.mIsSingleton, it.mInlets, it.mOutlets, it.mParameters );

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

	SharedLibraryMetainfo::SharedLibraryMetainfo( Path const& absPath, std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types ) :
		mCategory(), mDescription(), mAuthor(), mContact(), mVersion( 0, 0, 0 ), mPath( absPath ),
		mId( id ), mTypeCollection( types ), mTypes(), mBlocks()
	{
	}

	SharedLibraryMetainfo::~SharedLibraryMetainfo()
	{
		mBlocks.clear();
		mTypes.clear();
	}

	Path SharedLibraryMetainfo::getFilePath() const
	{
		return mPath;
	}

	std::string SharedLibraryMetainfo::getName() const
	{
		return mId->getName();
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

	void SharedLibraryMetainfo::exportType( std::string const& name )
	{
		for ( auto const& it : mTypePreinfos )
		{
			if ( it.mName == name )
			{
				std::ostringstream msg;
				msg << "a type named " << name << " was already exported by bundle " << getName() << std::endl;
				throw AlreadyExists( msg.str() );
			}
		}

		TypePreinfo info;
		info.mName = name;
		mTypePreinfos.push_back( info );
	}

	void SharedLibraryMetainfo::exportBlock( std::string const& name, const bool isSingleton, std::vector< std::string > const& inlets, std::vector< std::string > const& outlets, std::vector< std::string > const& parameters )
	{

		for ( auto const& it : mBlockPreinfos )
		{
			if ( it.mName == name )
			{
				std::ostringstream msg;
				msg << "a block named " << name << " was already exported by bundle " << getName() << std::endl;
				throw AlreadyExists( msg.str() );
			}
		}

		BlockPreinfo info;
		info.mName = name;
		info.mIsSingleton = isSingleton;
		info.mInlets = inlets;
		info.mOutlets = outlets;
		info.mParameters = parameters;
		mBlockPreinfos.push_back( info );
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

}