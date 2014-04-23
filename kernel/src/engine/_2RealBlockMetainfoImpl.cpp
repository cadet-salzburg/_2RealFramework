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

#include "engine/_2RealBlockMetainfoImpl.h"
#include "engine/_2RealIoSlotMetainfoImpl.h"
#include "engine/_2RealBlockFactory_I.h"
#include "engine/_2RealUpdatePolicyMetainfoImpl.h"
#include "engine/_2RealId.h"
#include "common/_2RealException.h"

namespace _2Real
{
	class IoSlotByName
	{
	public:
		explicit IoSlotByName( const std::string name ) : mBaseline( name ) {}
		bool operator()( std::shared_ptr< const IoSlotMetainfoImpl > val )
		{
			assert( val.get() );
			return mBaseline == val->getName();
		}
	private:
		std::string mBaseline;
	};

	std::shared_ptr< BlockMetainfoImpl > BlockMetainfoImpl::make( std::shared_ptr< const MetainfoId > id, std::shared_ptr< const TypeCollection > types, BlockDeclaration const& decl )
	{
		try
		{
			std::shared_ptr< const MetainfoId > blockId = MetainfoId::create( id, MetainfoType::BLOCK, decl.mName );
			std::shared_ptr< BlockMetainfoImpl > blockInfo( new BlockMetainfoImpl( blockId, types ) );

			for ( auto const& it : decl.mInlets )
			{
				std::shared_ptr< IoSlotMetainfoImpl > inletinfo = IoSlotMetainfoImpl::make( blockId, types, it.mName );
				inletinfo->setMulti( it.mIsMulti );
				blockInfo->mInlets.push_back( inletinfo );
			}

			for ( auto const& it : decl.mOutlets )
			{
				std::shared_ptr< IoSlotMetainfoImpl > outletinfo = IoSlotMetainfoImpl::make( blockId, types, it.mName );
				blockInfo->mOutlets.push_back( outletinfo );
			}

			for ( auto const& it : decl.mParameters )
			{
				std::shared_ptr< IoSlotMetainfoImpl > parameterinfo = IoSlotMetainfoImpl::make( blockId, types, it.mName );
				blockInfo->mParameters.push_back( parameterinfo );
			}

			blockInfo->mDefaultUpdatePolicy = UpdatePolicyMetainfoImpl::make( blockId, decl.mInlets );

			return blockInfo;
		}
		catch ( ... )
		{
			throw CreationFailure( "failed to create block metainfo object" );
		}
	}

	BlockMetainfoImpl::BlockMetainfoImpl( std::shared_ptr< const MetainfoId > id, std::shared_ptr< const TypeCollection > types ) :
		mId( id ),
		mTypes( types ),
		mDescription( "no block description available" ),
		mDependencies(),
		mDefaultUpdatePolicy( nullptr ),
		mFactory( nullptr ),
		mInlets(),
		mOutlets(),
		mParameters()
	{
	}

	std::shared_ptr< const MetainfoId > BlockMetainfoImpl::getId() const
	{
		return mId;
	}

	void BlockMetainfoImpl::setDescription( std::string const& description )
	{
		mDescription = description;
	}

	void BlockMetainfoImpl::setDependencies( std::vector< std::string > const& dependencies )
	{
		mDependencies = dependencies;
	}

	std::string BlockMetainfoImpl::getName() const
	{
		return mId->getName();
	}

	std::string BlockMetainfoImpl::getDescription() const
	{
		return mDescription;
	}

	std::vector< std::string > BlockMetainfoImpl::getDependencies() const
	{
		return mDependencies;
	}

	std::vector< std::shared_ptr< const IoSlotMetainfoImpl > > BlockMetainfoImpl::getInletMetainfos() const
	{
		std::vector< std::shared_ptr< const IoSlotMetainfoImpl > > result;
		for ( auto it : mInlets )
			result.push_back( it );
		return result;
	}

	std::vector< std::shared_ptr< const IoSlotMetainfoImpl > > BlockMetainfoImpl::getOutletMetainfos() const
	{
		std::vector< std::shared_ptr< const IoSlotMetainfoImpl > > result;
		for ( auto it : mOutlets )
			result.push_back( it );
		return result;
	}

	std::vector< std::shared_ptr< const IoSlotMetainfoImpl > > BlockMetainfoImpl::getParameterMetainfos() const
	{
		std::vector< std::shared_ptr< const IoSlotMetainfoImpl > > result;
		for ( auto it : mParameters )
			result.push_back( it );
		return result;
	}

	std::shared_ptr< const IoSlotMetainfoImpl > BlockMetainfoImpl::getInletMetainfo( std::string const& name ) const
	{
		auto it = std::find_if( mInlets.begin(), mInlets.end(), IoSlotByName( name ) );
		if ( it == mInlets.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< const IoSlotMetainfoImpl > BlockMetainfoImpl::getOutletMetainfo( std::string const& name ) const
	{
		auto it = std::find_if( mOutlets.begin(), mOutlets.end(), IoSlotByName( name ) );
		if ( it == mOutlets.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< const IoSlotMetainfoImpl > BlockMetainfoImpl::getParameterMetainfo( std::string const& name ) const
	{
		auto it = std::find_if( mParameters.begin(), mParameters.end(), IoSlotByName( name ) );
		if ( it == mParameters.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< IoSlotMetainfoImpl > BlockMetainfoImpl::getInletMetainfo( std::string const& name )
	{
		auto it = std::find_if( mInlets.begin(), mInlets.end(), IoSlotByName( name ) );
		if ( it == mInlets.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< IoSlotMetainfoImpl > BlockMetainfoImpl::getOutletMetainfo( std::string const& name )
	{
		auto it = std::find_if( mOutlets.begin(), mOutlets.end(), IoSlotByName( name ) );
		if ( it == mOutlets.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< IoSlotMetainfoImpl > BlockMetainfoImpl::getParameterMetainfo( std::string const& name )
	{
		auto it = std::find_if( mParameters.begin(), mParameters.end(), IoSlotByName( name ) );
		if ( it == mParameters.end() )
			throw NotFound( name );

		return *it;
	}

	std::shared_ptr< const UpdatePolicyMetainfoImpl > BlockMetainfoImpl::getDefaultUpdatePolicy() const
	{
		return mDefaultUpdatePolicy;
	}

	std::shared_ptr< UpdatePolicyMetainfoImpl > BlockMetainfoImpl::getDefaultUpdatePolicy()
	{
		return mDefaultUpdatePolicy;
	}

	void BlockMetainfoImpl::setFactory( std::shared_ptr< BlockFactory_I > factory )
	{
		mFactory = factory;
	}

	std::shared_ptr< const BlockFactory_I > BlockMetainfoImpl::getFactory() const
	{
		return mFactory;
	}
}