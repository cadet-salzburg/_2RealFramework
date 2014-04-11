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

#include "engine/_2RealBundle.h"
#include "engine/_2RealBundleCollection.h"
#include "engine/_2RealBlock.h"
#include "engine/_2RealSharedLibraryMetainfo.h"
#include "engine/_2RealSharedServiceMetainfo.h"
#include "engine/_2RealAbstractSharedServiceFactory.h"
#include "engine/_2RealSharedServiceLifetimeMgr.h"
#include "helpers/_2RealException.h"

#include "engine/_2RealId.h"

namespace _2Real
{

	std::shared_ptr< Bundle > Bundle::createFromMetainfo( std::shared_ptr< const SharedLibraryMetainfo > meta,  std::shared_ptr< Threadpool > stdthreads, std::shared_ptr< Threadpool > ctxtthreads, Path const& path )
	{
		std::shared_ptr< const InstanceId > bundleId = InstanceId::create( meta->getId(), nullptr, InstanceType::BUNDLE, path.string() );
		std::shared_ptr< Bundle > bundle( new Bundle( meta, bundleId, stdthreads, ctxtthreads, path.string() ) );

		for ( auto serviceMeta : meta->getServiceMetainfos() )
		{
			std::shared_ptr< AbstractSharedServiceLifetimeManager > manager;
			if ( serviceMeta->isSingleton() )
				manager.reset( new SingletonLifetimeManager( serviceMeta->getFactory() ) );
			else
				manager.reset( new ServiceLifetimeManager( serviceMeta->getFactory() ) );

			bundle->mServiceLifetimeMgrs[ serviceMeta->getName() ] = manager;
		}

		return bundle;
	}

	Bundle::Bundle( std::shared_ptr< const SharedLibraryMetainfo > meta, std::shared_ptr< const InstanceId > id, std::shared_ptr< Threadpool > stdthreads, std::shared_ptr< Threadpool > ctxtthreads, Path const& path ) :
		enable_shared_from_this< Bundle >(),
		mMetainfo( meta ),
		mId( id ),
		mPath( path ),
		mUnloadNotifier(),
		mStdThreads( stdthreads ),
		mCtxtThreads( ctxtthreads )
	{
	}

	std::shared_ptr< const InstanceId > Bundle::getId() const
	{
		return mId;
	}

	void Bundle::unload( const long timeout )
	{
		// hold the ptr to the bundle until everything is cleared
		std::shared_ptr< Bundle > tmp = shared_from_this();

		// causes the bundle collection to release the bundle
		// unless the call to unload was issued by the collection itsself
		mUnloadNotifier.notify( shared_from_this() );
		mUnloadNotifier.clear();

		// TODO: kill child blocks etc.
		( void ) ( timeout );

		// this should kill the bundle, finally
		tmp.reset();
	}

	Path Bundle::getFilePath() const
	{
		return mPath;
	}

	std::shared_ptr< Block > Bundle::createBlock( std::string const& name )
	{
		auto it = mServiceLifetimeMgrs.find( name );
		if ( it == mServiceLifetimeMgrs.end() )
		{
			std::ostringstream msg;
			msg << mId << " does not export block " << name;
			throw NotFound( msg.str() );
		}

		auto ctor = it->second;
		auto metainfo = mMetainfo->getServiceMetainfo( name );

		std::shared_ptr< Threadpool > threads;
		if ( metainfo->isSingleton() )
			threads = mCtxtThreads.lock();
		else
			threads = mStdThreads.lock();

		std::shared_ptr< Block > block = Block::createFromMetainfo( shared_from_this(), metainfo, threads, 0 /* TODO instance */ );
		// TODO: block -> nullptr?
		mServiceInstances.push_back( block );
		return block;
	}

}
