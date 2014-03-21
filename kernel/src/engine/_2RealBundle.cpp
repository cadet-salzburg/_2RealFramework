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
#include "engine/_2RealAbstractInlet.h"

#include "bundle/_2RealBlockIo.h"

#include "engine/_2RealAbstractInlet.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealMultiInlet.h"

namespace _2Real
{

	Bundle::Bundle( std::shared_ptr< BundleCollection > bundles, std::shared_ptr< const SharedLibraryMetainfo > meta, std::shared_ptr< Threadpool > stdthreads, std::shared_ptr< Threadpool > ctxtthreads ) :
		enable_shared_from_this< Bundle >(),
		mBundleCollection( bundles ),
		mMetainfo( meta ), mUnloadNotifier(),
		mStdThreads( stdthreads ), mCtxtThreads( ctxtthreads )
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
		std::vector< std::shared_ptr< AbstractSharedService > > dependencies;
		std::vector< std::string > dependenciesByName;
		info->getDependencies( dependenciesByName );
		for ( auto dependency : dependenciesByName )
		{
		//	if ( mServiceInstances.find( dependency ) == mServiceInstances.end() )
		//		createBlock( dependency );
		}

		/*
		*	TODO: io slots should know which block they belong to ( otherwise, how would i check for self links? ).
		*	-> identifier or something?
		*	-> or maybe 'setUnderlyingObj' on block ....
		*/

		_2Real::bundle::BlockIo bundleio;
		std::shared_ptr< _2Real::BlockIo > io( new BlockIo );
		Block::createIo( info, io->mParameters, io->mInlets, io->mOutlets );

		// anyway, now let's create handles for the inlets
		// ... need to know if the inlet in question is a multiinlet
		for ( auto it : io->mInlets )
		{
			if ( it->isMultiInlet() )
			{
				std::shared_ptr< MultiInlet > inlet = std::dynamic_pointer_cast< MultiInlet, AbstractInlet >( it );
				bundleio.mInlets.push_back( _2Real::bundle::MultiInletHandle( inlet ) );
			}
			else
			{
				std::shared_ptr< Inlet > inlet = std::dynamic_pointer_cast< Inlet, AbstractInlet >( it );
				bundleio.mInlets.push_back( _2Real::bundle::InletHandle( inlet ) );
			}		
		}

		for ( auto it : io->mOutlets )
			bundleio.mOutlets.push_back( _2Real::bundle::OutletHandle( it ) );	

		for ( auto it : io->mParameters )
			bundleio.mParameters.push_back( _2Real::bundle::ParameterHandle( it ) );	
		
		// acquire correct threadpool
		std::shared_ptr< Threadpool > threads;
		if ( info->isSingleton() )
			threads = mCtxtThreads.lock();
		else
			threads = mStdThreads.lock();

		// create the underlying object
		io->mBlockObj = ctor->create( bundleio, dependencies );

		std::shared_ptr< Block > instance( new Block( info, threads, io ) );
		mServiceInstances.push_back( instance );

		return instance;
	}

}
