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

#include "engine/_2RealSharedServiceMetainfo.h"
#include "engine/_2RealIoSlotMetainfo.h"
#include "engine/_2RealAbstractSharedServiceFactory.h"
#include "engine/_2RealUpdatePolicy.h"
#include "engine/_2RealId.h"
#include "helpers/_2RealException.h"

//#include "bundle/_2RealAbstractBlock.h"

namespace _2Real
{
	//class NullBlock : public _2Real::bundle::AbstractBlock
	//{

	//public:

	//	NullBlock( _2Real::bundle::BlockIo const& io, std::vector< std::shared_ptr< _2Real::bundle::AbstractBlock > > const& dependencies ) :
	//		AbstractBlock( io, dependencies )
	//	{
	//	}

	//	void update() {}
	//	void setup() {}
	//	void shutdown() {}

	//};

	struct NameCmp : public std::unary_function< std::string, bool >
	{
		explicit NameCmp( const std::string name ) : mName( name ) {}

		bool operator()( std::shared_ptr< IoSlotMetainfo > arg )
		{
			return mName == arg->getName();
		}

		std::string mName;
	};

	std::shared_ptr< SharedServiceMetainfo > SharedServiceMetainfo::make( std::shared_ptr< const MetainfoId > id, std::shared_ptr< const TypeCollection > types, BlockDeclaration const& decl )
	{
		try
		{
			std::shared_ptr< const MetainfoId > blockId = MetainfoId::create( id, MetainfoType::BLOCK, decl.mName );
			std::shared_ptr< SharedServiceMetainfo > blockInfo( new SharedServiceMetainfo( blockId, types, decl.mIsSingleton ) );

			for ( auto const& it : decl.mInlets )
			{
				std::shared_ptr< IoSlotMetainfo > inletinfo = IoSlotMetainfo::make( blockId, types, it.mName );
				inletinfo->setMulti( it.mIsMulti );
				blockInfo->mInlets.push_back( inletinfo );
			}

			for ( auto const& it : decl.mOutlets )
			{
				std::shared_ptr< IoSlotMetainfo > outletinfo = IoSlotMetainfo::make( blockId, types, it.mName );
				blockInfo->mOutlets.push_back( outletinfo );
			}

			for ( auto const& it : decl.mParameters )
			{
				std::shared_ptr< IoSlotMetainfo > parameterinfo = IoSlotMetainfo::make( blockId, types, it.mName );
				blockInfo->mParameters.push_back( parameterinfo );
			}

			blockInfo->mDefaultUpdatePolicy = UpdatePolicyMetainfo::make( blockId, decl.mInlets );

			// this makes it impossible to detect if a user has not set the block class :/
			// the only other option would be to actually announce the block class, the make a clone
			// blockInfo->mFactory = std::shared_ptr< AbstractSharedServiceFactory >( new NullBlock( _2Real::bundle::BlockIo(), std::vector< std::shared_ptr< _2Real::bundle::AbstractBlock > >() ) );

			return blockInfo;
		}
		catch ( ... )
		{
			throw _2Real::CreationFailure( "failed to create block metainfo object" );
		}
	}

	SharedServiceMetainfo::SharedServiceMetainfo( std::shared_ptr< const MetainfoId > id, std::shared_ptr< const TypeCollection > types, const bool isSingleton ) :
		mId( id ),
		mTypes( types ),
		mIsSingleton( isSingleton ),
		mDescription( "no block description available" ),
		mDependencies(),
		mDefaultUpdatePolicy( nullptr ),
		mFactory( nullptr ),
		mInlets(),
		mOutlets(),
		mParameters()
	{
	}

	std::shared_ptr< const MetainfoId > SharedServiceMetainfo::getId() const
	{
		return mId;
	}

	void SharedServiceMetainfo::setDescription( std::string const& description )
	{
		mDescription = description;
	}

	void SharedServiceMetainfo::setDependencies( std::vector< std::string > const& dependencies )
	{
		mDependencies = dependencies;
	}

	bool SharedServiceMetainfo::isSingleton() const
	{
		return mIsSingleton;
	}

	std::string SharedServiceMetainfo::getName() const
	{
		return mId->getName();
	}

	std::string SharedServiceMetainfo::getDescription() const
	{
		return mDescription;
	}

	std::vector< std::string > SharedServiceMetainfo::getDependencies() const
	{
		return mDependencies;
	}

	std::vector< std::shared_ptr< const IoSlotMetainfo > > SharedServiceMetainfo::getInletMetainfos() const
	{
		std::vector< std::shared_ptr< const IoSlotMetainfo > > result;
		for ( auto it : mInlets )
			result.push_back( it );
		return result;
	}

	std::vector< std::shared_ptr< const IoSlotMetainfo > > SharedServiceMetainfo::getOutletMetainfos() const
	{
		std::vector< std::shared_ptr< const IoSlotMetainfo > > result;
		for ( auto it : mOutlets )
			result.push_back( it );
		return result;
	}

	std::vector< std::shared_ptr< const IoSlotMetainfo > > SharedServiceMetainfo::getParameterMetainfos() const
	{
		std::vector< std::shared_ptr< const IoSlotMetainfo > > result;
		for ( auto it : mParameters )
			result.push_back( it );
		return result;
	}

	std::shared_ptr< const IoSlotMetainfo > SharedServiceMetainfo::getInletMetainfo( std::string const& name ) const
	{
		auto it = std::find_if( mInlets.begin(), mInlets.end(), NameCmp( name ) );
		if ( it == mInlets.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< const IoSlotMetainfo > SharedServiceMetainfo::getOutletMetainfo( std::string const& name ) const
	{
		auto it = std::find_if( mOutlets.begin(), mOutlets.end(), NameCmp( name ) );
		if ( it == mOutlets.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< const IoSlotMetainfo > SharedServiceMetainfo::getParameterMetainfo( std::string const& name ) const
	{
		auto it = std::find_if( mParameters.begin(), mParameters.end(), NameCmp( name ) );
		if ( it == mParameters.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< IoSlotMetainfo > SharedServiceMetainfo::getInletMetainfo( std::string const& name )
	{
		auto it = std::find_if( mInlets.begin(), mInlets.end(), NameCmp( name ) );
		if ( it == mInlets.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< IoSlotMetainfo > SharedServiceMetainfo::getOutletMetainfo( std::string const& name )
	{
		auto it = std::find_if( mOutlets.begin(), mOutlets.end(), NameCmp( name ) );
		if ( it == mOutlets.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< IoSlotMetainfo > SharedServiceMetainfo::getParameterMetainfo( std::string const& name )
	{
		auto it = std::find_if( mParameters.begin(), mParameters.end(), NameCmp( name ) );
		if ( it == mParameters.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< const UpdatePolicyMetainfo > SharedServiceMetainfo::getDefaultUpdatePolicy() const
	{
		return mDefaultUpdatePolicy;
	}

	std::shared_ptr< UpdatePolicyMetainfo > SharedServiceMetainfo::getDefaultUpdatePolicy()
	{
		return mDefaultUpdatePolicy;
	}

	void SharedServiceMetainfo::setFactory( std::shared_ptr< AbstractSharedServiceFactory > factory )
	{
		mFactory = factory;
	}

	std::shared_ptr< const AbstractSharedServiceFactory > SharedServiceMetainfo::getFactory() const
	{
		return mFactory;
	}
}