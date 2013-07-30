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

#include "app/_2RealCallbacks.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealBundleHandle.h"
#include "app/_2RealBlockHandle.h"
#include "helpers/_2RealNonCopyable.h"

#include <string>
#include <set>
#include <vector>

namespace _2Real
{

	template< typename T >
	class SingletonHolder;

	class EngineImpl;

	namespace app
	{
		class SystemState;

		class Engine : private NonCopyable< Engine >
		{

			template< typename T >
			friend class _2Real::SingletonHolder;

		public:

			//typedef std::pair< InletHandle, OutletHandle >				Link;

			//struct SortByInlet
			//{
			//	bool operator()( Link const& l1, Link const& l2 ) { return ( l1.first < l2.first ); }
			//};

			//struct SortByOutlet
			//{
			//	bool operator()( Link const& l1, Link const& l2 ) { return ( l1.second < l2.second ); }
			//};

			//typedef std::set< Link, SortByOutlet >						Links;
			//typedef std::set< Link, SortByOutlet >::iterator			LinkIterator;
			//typedef std::set< Link, SortByOutlet >::const_iterator		LinkConstIterator;

			typedef std::vector< BundleHandle >							BundleHandles;
			typedef std::vector< BundleHandle >::iterator				BundleHandleIterator;
			typedef std::vector< BundleHandle >::const_iterator			BundleHandleConstIterator;

			typedef std::vector< BlockHandle >							BlockHandles;
			typedef std::vector< BlockHandle >::iterator				BlockHandleIterator;
			typedef std::vector< BlockHandle >::const_iterator			BlockHandleConstIterator;

			static Engine& instance();

			//--------------------- bundle loading
			// returns the absolute path to the bundle directory
			std::string getBundleDirectory() const;
			// loads a bundle, path must be relative to bundle dir
			app::BundleHandle loadBundle( std::string const& relativePath );
			//--------------------- bundle loading

			// clears the whole framework - i.e. all bundles, context blocks & blocks
			void clearAll();
			// clears just the block instances
			void clearBlockInstances();

			//--------------------- exception callbacks
			void registerToException( BlockExceptionCallback callback, void *userData = nullptr );
			void unregisterFromException( BlockExceptionCallback callback, void *userData = nullptr );

			template< typename TCallable >
			void registerToException( TCallable &callable, void ( TCallable::*callback )( std::pair< Exception, BlockHandle > const& ) )
			{
				BlockExcCallback *cb = new MemberCallback< TCallable, std::pair< Exception, BlockHandle > const& >( callable, callback );
				registerToExceptionInternal( *cb );
			}

			template< typename TCallable >
			void unregisterFromException( TCallable &callable, void ( TCallable::*callback )( std::pair< Exception, BlockHandle > const& ) )
			{
				BlockExcCallback *cb = new MemberCallback< TCallable, std::pair< Exception, BlockHandle > const& >( callable, callback );
				unregisterFromExceptionInternal( *cb );
			}
			//--------------------- exception callbacks

			//--------------------- context block exception callbacks
			void registerToException( ContextBlockExceptionCallback callback, void *userData = nullptr );
			void unregisterFromException( ContextBlockExceptionCallback callback, void *userData = nullptr );

			template< typename TCallable >
			void registerToException( TCallable &callable, void ( TCallable::*callback )( std::pair< Exception, ContextBlockHandle > const& ) )
			{
				ContextBlockExcCallback *cb = new MemberCallback< TCallable, std::pair< Exception, ContextBlockHandle > const& >( callable, callback );
				registerToExceptionInternal( *cb );
			}

			template< typename TCallable >
			void unregisterFromException( TCallable &callable, void ( TCallable::*callback )( std::pair< Exception, ContextBlockHandle > const& ) )
			{
				ContextBlockExcCallback *cb = new MemberCallback< TCallable, std::pair< Exception, ContextBlockHandle > const& >( callable, callback );
				unregisterFromExceptionInternal( *cb );
			}
			//--------------------- context block exception callbacks

			//--------------------- loading / saving
			// new 02/05/13
			//void getCurrentSystemState( SystemState &state );
			// this basically tests whether or not all bundles are there
			// not doing this & just loading results in an exception
			//std::list< std::string > testConfiguration( std::string const& dataSource );
			// returns the difference to the previous system state
			//SystemState *loadConfiguration( std::string const& dataSource );
			//--------------------- loading / saving

		private:

			void registerToExceptionInternal( BlockExcCallback & );
			void unregisterFromExceptionInternal( BlockExcCallback & );
			void registerToExceptionInternal( ContextBlockExcCallback & );
			void unregisterFromExceptionInternal( ContextBlockExcCallback & );

			Engine();
			~Engine();

			Engine( Engine const& src );
			Engine& operator=( Engine const& other );

			EngineImpl		*const mImpl;

		};
	}
}