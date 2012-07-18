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
		class Engine
		{

			template< typename T >
			friend class _2Real::SingletonHolder;

		public:

			typedef std::pair< InletHandle, OutletHandle >				Link;

			struct SortByInlet
			{
				bool operator()( Link const& l1, Link const& l2 ) { return ( l1.first < l2.first ); }
			};

			struct SortByOutlet
			{
				bool operator()( Link const& l1, Link const& l2 ) { return ( l1.second < l2.second ); }
			};

			// (?) chose set here only b/c of the sorting advantage
			// maybe should be vector, too
			typedef std::set< Link, SortByOutlet >						Links;
			typedef std::set< Link, SortByOutlet >::iterator			LinkIterator;
			typedef std::set< Link, SortByOutlet >::const_iterator		LinkConstIterator;

			typedef std::vector< BundleHandle >							BundleHandles;
			typedef std::vector< BundleHandle >::iterator				BundleHandleIterator;
			typedef std::vector< BundleHandle >::const_iterator			BundleHandleConstIterator;

			typedef std::vector< BlockHandle >							BlockHandles;
			typedef std::vector< BlockHandle >::iterator				BlockHandleIterator;
			typedef std::vector< BlockHandle >::const_iterator			BlockHandleConstIterator;

			static Engine& instance();

			void setBaseDirectory( std::string const& directory );
			app::BundleHandle & loadBundle( std::string const& libraryPath );
			app::BundleHandle & findBundleByPath( std::string const& libraryPath ) const;
			app::BundleHandle & findBundleByName( std::string const& name ) const;

			// either clears everything, incl bundles and contexts, or just the block instances
			void clearAll();
			void clearBlockInstances();

			// these functions give information about the current state of the framework
			Links			getCurrentLinks() const;
			BlockHandles	getCurrentBlocks() const;		// these are only the block instances, not the contexts
			BundleHandles	getCurrentBundles() const;

			// exception callback
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

			// context block exception callback
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

			void safeConfig( std::string const& filePath );
			std::list< std::string > tryConfig( std::string const& filePath );
			void loadConfig( std::string const& filePath );

		private:

			void registerToExceptionInternal( BlockExcCallback &cb );
			void unregisterFromExceptionInternal( BlockExcCallback &cb );
			void registerToExceptionInternal( ContextBlockExcCallback &cb );
			void unregisterFromExceptionInternal( ContextBlockExcCallback &cb );

			Engine();
			Engine( Engine const& src );

			EngineImpl		&m_EngineImpl;

		};
	}
}