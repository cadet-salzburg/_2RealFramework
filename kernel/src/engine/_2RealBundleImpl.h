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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealPath.h"

#include "common/_2RealCallback_T.h"
#include "common/_2RealEvent_T.h"

namespace _2Real
{

	class BundleMetainfoImpl;
	class BlockLifetimeMgr_I;
	class BlockImpl;
	class ThreadpoolImpl_I;
	class InstanceId;
	class SharedLibrary;

	class BundleImpl : public std::enable_shared_from_this< BundleImpl >
	{

	public:

		static std::shared_ptr< BundleImpl > createFromMetainfo( std::shared_ptr< const BundleMetainfoImpl >, std::shared_ptr< SharedLibrary >, Path const& );

		BundleImpl() = delete;
		BundleImpl( BundleImpl const& other ) = delete;
		BundleImpl( BundleImpl && other ) = delete;
		BundleImpl operator=( BundleImpl const& other ) = delete;
		BundleImpl operator=( BundleImpl && other ) = delete;

		~BundleImpl() = default;

		std::shared_ptr< const InstanceId >			getId() const;

		Path 										getFilePath() const;
		void										unload( const long timeout );
		std::shared_ptr< BlockImpl >				createBlock( std::string const& name, std::shared_ptr< ThreadpoolImpl_I > system, std::vector< std::shared_ptr< BlockImpl > > const& dependencies );

		// unload listener.... bundle collection registers here
		///////////////////////////////////////////////////////
		template< typename TCallable >
		void registerToUnload( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< const BundleImpl > ) )
		{
			std::shared_ptr< AbstractCallback_T< std::shared_ptr< const BundleImpl > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const BundleImpl > >( callable, callback ) );
			mUnloadNotifier.addListener( listener );
		}
		template< typename TCallable >
		void unregisterFromUnload( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< const BundleImpl > ) )
		{
			std::shared_ptr< AbstractCallback_T< std::shared_ptr< const BundleImpl > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const BundleImpl > >( callable, callback ) );
			mUnloadNotifier.removeListener( listener );
		}
		///////////////////////////////////////////////////////

	private:

		BundleImpl( std::shared_ptr< SharedLibrary >, std::shared_ptr< const BundleMetainfoImpl >, std::shared_ptr< const InstanceId >, Path const& );

		std::shared_ptr< SharedLibrary >						mLibrary;

		std::shared_ptr< const BundleMetainfoImpl >				mMetainfo;
		std::shared_ptr< const InstanceId >						mId;

		Path													mPath;
		Event_T< std::shared_ptr< const BundleImpl > >			mUnloadNotifier;

		std::vector< std::pair< std::string, std::shared_ptr< BlockLifetimeMgr_I > > >	mLifetimeMgrs;
		std::vector< std::shared_ptr< BlockImpl > >										mBlocks;

	};

}