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
#include "engine/_2RealSharedServiceIoSlotMetainfo.h"
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

	std::shared_ptr< SharedServiceMetainfo > SharedServiceMetainfo::make( std::shared_ptr< const MetainfoId > id, std::shared_ptr< const TypeCollection > types, std::string const& name, const bool isSingleton, std::vector< std::string > inlets, std::vector< std::string > outlets, std::vector< std::string > parameters )
	{
		try
		{
			std::shared_ptr< const MetainfoId > blockId = MetainfoId::create( id, MetainfoType::BLOCK, name );
			std::shared_ptr< SharedServiceMetainfo > blockInfo( new SharedServiceMetainfo( blockId, types, isSingleton ) );

			std::vector< std::pair< std::string, bool > > inletRep;

			for ( auto it : inlets )
			{
				std::shared_ptr< SharedServiceIoSlotMetainfo > inletinfo = SharedServiceIoSlotMetainfo::make( blockId, types, it );
				blockInfo->mInlets[ it ] = inletinfo;
				inletRep.push_back( std::make_pair( it, false ) );
			}

			for ( auto it : outlets )
			{
				std::shared_ptr< SharedServiceIoSlotMetainfo > outletinfo = SharedServiceIoSlotMetainfo::make( blockId, types, it );
				blockInfo->mOutlets[ it ] = outletinfo;
			}

			for ( auto it : parameters )
			{
				std::shared_ptr< SharedServiceIoSlotMetainfo > parameterinfo = SharedServiceIoSlotMetainfo::make( blockId, types, it );
				blockInfo->mParameters[ it ] = parameterinfo;
			}

			blockInfo->mDefaultUpdatePolicy = UpdatePolicyMetainfo::make( blockId, inletRep );

			// this makes it impossible to detect if a user has not set the block class :/
			// the only other option would be to actually announce the block class, the make a clone!?
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

	std::vector< std::shared_ptr< const SharedServiceIoSlotMetainfo > > SharedServiceMetainfo::getInletMetainfos() const
	{
		std::vector< std::shared_ptr< const SharedServiceIoSlotMetainfo > > result;
		for ( auto it : mInlets )
			result.push_back( it.second );
		return result;
	}

	std::vector< std::shared_ptr< const SharedServiceIoSlotMetainfo > > SharedServiceMetainfo::getOutletMetainfos() const
	{
		std::vector< std::shared_ptr< const SharedServiceIoSlotMetainfo > > result;
		for ( auto it : mOutlets )
			result.push_back( it.second );
		return result;
	}

	std::vector< std::shared_ptr< const SharedServiceIoSlotMetainfo > > SharedServiceMetainfo::getParameterMetainfos() const
	{
		std::vector< std::shared_ptr< const SharedServiceIoSlotMetainfo > > result;
		for ( auto it : mParameters )
			result.push_back( it.second );
		return result;
	}

	std::shared_ptr< const SharedServiceIoSlotMetainfo > SharedServiceMetainfo::getInletMetainfo( std::string const& name ) const
	{
		auto it = mInlets.find( name );
		if ( it == mInlets.end() )
			throw NotFound( name );

		return it->second;
	}

	std::shared_ptr< const SharedServiceIoSlotMetainfo > SharedServiceMetainfo::getOutletMetainfo( std::string const& name ) const
	{
		auto it = mOutlets.find( name );
		if ( it == mOutlets.end() )
			throw NotFound( name );

		return it->second;
	}

	std::shared_ptr< const SharedServiceIoSlotMetainfo > SharedServiceMetainfo::getParameterMetainfo( std::string const& name ) const
	{
		auto it = mParameters.find( name );
		if ( it == mParameters.end() )
			throw NotFound( name );

		return it->second;
	}

	std::shared_ptr< SharedServiceIoSlotMetainfo > SharedServiceMetainfo::getInletMetainfo( std::string const& name )
	{
		auto it = mInlets.find( name );
		if ( it == mInlets.end() )
			throw NotFound( name );

		return it->second;
	}

	std::shared_ptr< SharedServiceIoSlotMetainfo > SharedServiceMetainfo::getOutletMetainfo( std::string const& name )
	{
		auto it = mOutlets.find( name );
		if ( it == mOutlets.end() )
			throw NotFound( name );

		return it->second;
	}

	std::shared_ptr< SharedServiceIoSlotMetainfo > SharedServiceMetainfo::getParameterMetainfo( std::string const& name )
	{
		auto it = mParameters.find( name );
		if ( it == mParameters.end() )
			throw NotFound( name );

		return it->second;
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