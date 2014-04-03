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

#pragma once

#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealCallback_T.h"
#include "helpers/_2RealEvent_T.h"
#include "helpers/_2RealPath.h"

namespace _2Real
{

	class SharedLibraryMetainfo;
	class SharedServiceMetainfo;
	class AbstractSharedServiceLifetimeManager;
	class Block;
	class BundleCollection;
	class Threadpool;

	class Bundle : public std::enable_shared_from_this< Bundle >
	{

	public:

		Bundle( std::shared_ptr< BundleCollection >, std::shared_ptr< const SharedLibraryMetainfo >, std::shared_ptr< Threadpool >, std::shared_ptr< Threadpool > );

		~Bundle();

		void init();
		void initServices();

		// return file path ( from metainfo )
		Path 										getFilePath() const;
		// unload function, via app::bundle handle
		void										unload( const long timeout );

		std::shared_ptr< Block >					createBlock( std::string const& name );

		std::shared_ptr< const SharedLibraryMetainfo >	getMetainfo() const;

		// unload listener.... bundle collection registers here
		///////////////////////////////////////////////////////
		template< typename TCallable >
		void registerToUnload( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< const Bundle > ) )
		{
			std::shared_ptr< AbstractCallback_T< std::shared_ptr< const Bundle > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const Bundle > >( callable, callback ) );
			mUnloadNotifier.addListener( listener );
		}
		template< typename TCallable >
		void unregisterFromUnload( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< const Bundle > ) )
		{
			std::shared_ptr< AbstractCallback_T< std::shared_ptr< const Bundle > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const Bundle > >( callable, callback ) );
			mUnloadNotifier.removeListener( listener );
		}
		///////////////////////////////////////////////////////

	private:

		Bundle( Bundle const& other );
		Bundle operator=( Bundle const& other );

		std::weak_ptr< BundleCollection >						mBundleCollection;

		std::shared_ptr< const SharedLibraryMetainfo >			mMetainfo;
		Event_T< std::shared_ptr< const Bundle > >				mUnloadNotifier;

		typedef std::pair< std::shared_ptr< AbstractSharedServiceLifetimeManager >, std::shared_ptr< const SharedServiceMetainfo > > ServiceInfo;
		typedef std::map< std::string, ServiceInfo >			ServiceMap;

		ServiceMap												mServices;

		typedef std::vector< std::shared_ptr< Block > >			ServiceInstances;

		ServiceInstances										mServiceInstances;

		std::weak_ptr< Threadpool >					mStdThreads;
		std::weak_ptr< Threadpool >					mCtxtThreads;

	};

}

/*
*	TODO: bundle should maybe manage both lib & lib metainfo? -> makes bundle importer useless
*/