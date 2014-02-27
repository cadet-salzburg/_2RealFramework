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

namespace _2Real
{

	Bundle::Bundle( std::shared_ptr< BundleCollection > bundles, std::shared_ptr< const SharedLibraryMetainfo > meta ) :
		enable_shared_from_this< Bundle >(),
		mBundleCollection( bundles ),
		mMetainfo( meta ), mUnloadNotifier()
	{
	}

	Bundle::~Bundle()
	{
		std::cout << "deleting shared library " << mMetainfo->getName() << std::endl;
		mServices.clear();
		mServiceInstances.clear();
		mMetainfo.reset();
	}

	void Bundle::initServices()	
	{
		std::vector< std::shared_ptr< const SharedServiceMetainfo > > serviceMetainfos;
		mMetainfo->getServiceMetainfos( serviceMetainfos );

		for ( auto info : serviceMetainfos )
		{
			std::shared_ptr< const AbstractSharedServiceFactory > factory = info->getFactory();
			std::shared_ptr< AbstractSharedServiceLifetimeManager > manager;
			if ( info->isSingleton() )
				manager.reset( new SingletonLifetimeManager( factory ) );
			else
				manager.reset( new ServiceLifetimeManager( factory ) );

			mServices[ info->getName() ] = ServiceInfo( manager, info );
		}
	}

	void Bundle::init()
	{
		initServices();
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

	Path const& Bundle::getFilePath() const
	{
		return mMetainfo->getFilePath();
	}

	std::shared_ptr< const SharedLibraryMetainfo > Bundle::getMetainfo() const
	{
		return mMetainfo;
	}

	std::shared_ptr< Block > Bundle::createBlock( std::string const& name )
	{
		auto it = mServices.find( name );
		if ( it == mServices.end() )
		{
			std::ostringstream msg;
			msg << "bundle " << mMetainfo->getName() << " does not export a block named " << name;
			throw NotFoundException( msg.str() );
		}

		auto ctor = it->second.first;
		auto info = it->second.second;

		// get dependencies
		// for all dependencies:	check if instance already was created
		//							if not, create instance

		std::vector< std::string > dependencies;
		info->getDependencies( dependencies );
		for ( auto dependency : dependencies )
		{
			//if ( mServiceInstances.find( dependency ) == mServiceInstances.end() )
				createBlock( dependency );
		}

		std::weak_ptr< Threadpool > threads;
		if ( info->isSingleton() )
			threads = mBundleCollection.lock()->getThreadsForSingletonBlock();
		else
			threads = mBundleCollection.lock()->getThreadsForRegularBlock();

		auto obj = ctor->create();
		std::shared_ptr< Block > instance( new Block( threads.lock(), info, obj ) );
		instance->init();
		mServiceInstances.push_back( instance );

		// setup the block? that's a more general question...

		return instance;
	}

}
