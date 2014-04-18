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

#pragma warning( disable : 4996 )
#pragma warning( disable : 4702 )

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
	class BlockNameCmp : public std::unary_function< bool, std::pair< std::string, std::shared_ptr< BlockLifetimeMgr_I > > >
	{

	public:

		explicit BlockNameCmp( const std::string name ) : mName( name )
		{
		}

		bool operator()( std::pair< std::string, std::shared_ptr< BlockLifetimeMgr_I > > const& val ) const
		{
			return mName == val.first;
		}

	private:

		std::string mName;

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

	std::shared_ptr< const InstanceId > BundleImpl::getId() const
	{
		return mId;
	}

	void BundleImpl::unload( const long timeout )
	{
		// hold the ptr to the bundle until everything is cleared
		std::shared_ptr< BundleImpl > tmp = shared_from_this();

		// causes the bundle collection to release the bundle
		// unless the call to unload was issued by the collection itsself
		mUnloaded( shared_from_this() );
		mUnloaded.disconnect_all_slots();

		// TODO: kill child blocks etc.
		( void ) ( timeout );

		// this should kill the bundle, finally
		tmp.reset();
	}

	Path BundleImpl::getFilePath() const
	{
		return mPath;
	}

	std::shared_ptr< BlockImpl > BundleImpl::createBlock( std::string const& name, std::shared_ptr< ThreadpoolImpl_I > threads, std::vector< std::shared_ptr< BlockImpl > > const& dependencies )
	{
		auto it = std::find_if( mLifetimeMgrs.begin(), mLifetimeMgrs.end(), BlockNameCmp( name ) );
		if ( it == mLifetimeMgrs.end() )
		{
			std::ostringstream msg;
			msg << mId << " does not export block " << name;
			throw NotFound( msg.str() );
		}

		auto ctor = it->second;
		auto metainfo = mMetainfo->getServiceMetainfo( name );

		std::shared_ptr< BlockImpl > block = BlockImpl::createFromMetainfo( shared_from_this(), metainfo, threads, dependencies, 0 /* TODO instance */ );
		mBlocks.push_back( block );
		return block;
	}

	boost::signals2::connection BundleImpl::registerToUnload( boost::signals2::signal< void( std::shared_ptr< const BundleImpl > ) >::slot_type slot ) const
	{
		return mUnloaded.connect( slot );
	}
}
