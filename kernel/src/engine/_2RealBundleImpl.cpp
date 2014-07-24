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

#include "engine/_2RealBundleImpl.h"
#include "engine/_2RealBundleCollection.h"
#include "engine/_2RealBlockImpl.h"
#include "engine/_2RealBundleMetainfoImpl.h"
#include "engine/_2RealBlockMetainfoImpl.h"
#include "engine/_2RealBlockFactory_I.h"
#include "engine/_2RealBlockLifetimeMgr.h"
#include "common/_2RealException.h"

#include "engine/_2RealId.h"

namespace _2Real
{
	class BlockFactoryByName
	{
	public:
		explicit BlockFactoryByName( const std::string name ) : mBaseline( name ) {}
		bool operator()( std::pair< std::string, std::shared_ptr< BlockLifetimeMgr_I > > const& val ) const
		{
			assert( val.second.get() );
			return mBaseline == val.first;
		}
	private:
		std::string mBaseline;
	};

	class BlockByAddress
	{
	public:
		explicit BlockByAddress( std::shared_ptr< const BlockImpl > block ) : mBaseline( block ) { assert( block.get() ); }
		bool operator()( std::pair< boost::signals2::connection, std::shared_ptr< const BlockImpl > > const& val ) const
		{
			assert( val.second.get() );
			return mBaseline.get() == val.second.get();
		}
	private:
		std::shared_ptr< const BlockImpl > mBaseline;
	};

	std::shared_ptr< BundleImpl > BundleImpl::createFromMetainfo( std::shared_ptr< const BundleMetainfoImpl > meta, std::shared_ptr< SharedLibrary > lib, Path const& path )
	{
		std::shared_ptr< const InstanceId > bundleId = InstanceId::create( meta->getId(), nullptr, InstanceType::BUNDLE, path.string() );
		std::shared_ptr< BundleImpl > bundle( new BundleImpl( lib, meta, bundleId, path.string() ) );

		for ( auto serviceMeta : meta->getServiceMetainfos() )
		{
			std::shared_ptr< BlockLifetimeMgr_I > manager;
			//if ( serviceMeta->isSingleton() )
			//	manager.reset( new SingletonLifetimeManager( serviceMeta->getFactory() ) );
			//else
			manager.reset( new BlockLifetimeManager( serviceMeta->getFactory() ) );

			bundle->mLifetimeMgrs.push_back( std::make_pair( serviceMeta->getName(), manager ) );
		}

		return bundle;
	}

	BundleImpl::BundleImpl( std::shared_ptr< SharedLibrary > lib, std::shared_ptr< const BundleMetainfoImpl > meta, std::shared_ptr< const InstanceId > id, Path const& path ) :
		enable_shared_from_this< BundleImpl >(),
		mLibrary( lib ),
		mMetainfo( meta ),
		mId( id ),
		mPath( path )
	{
	}

	std::shared_ptr< const BundleMetainfoImpl > BundleImpl::getMetainfo() const
	{
		return mMetainfo;
	}

	std::shared_ptr< const InstanceId > BundleImpl::getId() const
	{
		return mId;
	}

	void BundleImpl::unload( const uint64_t timeout )
	{
		// hold the ptr to the bundle until everything is cleared
		std::shared_ptr< BundleImpl > tmp = shared_from_this();

		std::vector< std::pair< std::shared_ptr< BlockImpl >, std::future< BlockResult > > > blocks;
		// stores pointres to blocks, and the block's response to 'parent unloaded'
		for ( auto it : mBlocks )
			blocks.push_back( std::make_pair( it.second, it.second->parentUnloaded() ) );

		// give the blocks enough time to perform the shutdown
		// ( default timeout = 5000 milliseconds, which is pretty long )
		if ( !blocks.empty() )
			std::this_thread::sleep_for( std::chrono::milliseconds( timeout ) );

		std::ostringstream msg;
		bool isOk = true;
		for ( auto &it : blocks )
		{
			try
			{
				// see if 'get' function returns anything, but we don't actually care about the result
				BlockResult result = it.second.get();
				( void )( result );
				// if get did NOT throw, remove the block
				auto blockIter = std::find_if( mBlocks.begin(), mBlocks.end(), BlockByAddress( it.first ) );
				mBlocks.erase( blockIter );
				it.first.reset();
			}
			catch ( std::future_error &e )
			{
				msg << it.first->getId()->getName() << " : " << e.what() << " " << e.code() << std::endl;
				isOk = false;
			}
		}

		if ( isOk )
		{
			// causes the bundle collection to release the bundle
			// unless the call to unload was issued by the collection itsself
			mDestroyed( shared_from_this() );
			mDestroyed.disconnect_all_slots();

			tmp.reset();
		}
		else
			throw Timeout( msg.str() );
		
	}

	Path BundleImpl::getFilePath() const
	{
		return mPath;
	}

	std::shared_ptr< BlockImpl > BundleImpl::createBlock( std::string const& name, std::shared_ptr< ThreadpoolImpl_I > threads, std::vector< std::shared_ptr< BlockImpl > > const& dependencies )
	{
		auto it = std::find_if( mLifetimeMgrs.begin(), mLifetimeMgrs.end(), BlockFactoryByName( name ) );
		if ( it == mLifetimeMgrs.end() )
		{
			std::ostringstream msg;
			msg << mId << " does not export block " << name;
			throw NotFound( msg.str() );
		}

		auto ctor = it->second;
		auto metainfo = mMetainfo->getServiceMetainfo( name );

		std::shared_ptr< BlockImpl > block = BlockImpl::createFromMetainfo( shared_from_this(), metainfo, threads, dependencies, 0 /* TODO instance */ );
		auto connection = block->registerToDestroyed( std::bind( &BundleImpl::blockDestroyed, this, std::placeholders::_1 ) );
		mBlocks.push_back( std::make_pair( connection, block ) );
		return block;
	}

	boost::signals2::connection BundleImpl::registerToDestroyed( boost::signals2::signal< void( std::shared_ptr< const BundleImpl > ) >::slot_type slot ) const
	{
		return mDestroyed.connect( slot );
	}

	void BundleImpl::blockDestroyed( std::shared_ptr< const BlockImpl > block )
	{
		auto blockIter = std::find_if( mBlocks.begin(), mBlocks.end(), BlockByAddress( block ) );
		if ( blockIter != mBlocks.end() )
		{
			blockIter->first.disconnect();
			mBlocks.erase( blockIter );
		}
	}
}
