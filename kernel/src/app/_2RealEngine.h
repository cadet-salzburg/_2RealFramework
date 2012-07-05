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

#include <string>
#include <set>

namespace _2Real
{

	template< typename T >
	class SingletonHolder;
	class EngineImpl;

	namespace app
	{
		class BundleHandle;
		class InletHandle;
		class OutletHandle;

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

			typedef std::set< Link, SortByOutlet >						Links;
			typedef std::set< Link, SortByOutlet >::iterator			LinkIterator;
			typedef std::set< Link, SortByOutlet >::const_iterator		LinkConstIterator;

			static Engine& instance();

			void setBaseDirectory( std::string const& directory );

			app::BundleHandle & loadBundle( std::string const& libraryPath );

			void clearFully();
			void clearBlockInstances();

			// (?) if we start with this, we may need to do the same for current bundles as well as current blocks
			Links getCurrentLinks() const;

			void registerToException( ExceptionCallback callback, void *userData = nullptr );
			void unregisterFromException( ExceptionCallback callback, void *userData = nullptr );

			template< typename TCallable >
			void registerToException( TCallable &callable, void ( TCallable::*callback )( std::pair< Exception, BlockHandle > const& ) )
			{
				ErrorCallback *cb = new MemberCallback< TCallable, std::pair< Exception, BlockHandle > const& >( callable, callback );
				registerToExceptionInternal( *cb );
			}

			template< typename TCallable >
			void unregisterFromException( TCallable &callable, void ( TCallable::*callback )( std::pair< Exception, BlockHandle > const& ) )
			{
				ErrorCallback *cb = new MemberCallback< TCallable, std::pair< Exception, BlockHandle > const& >( callable, callback );
				unregisterFromExceptionInternal( *cb );
			} 

		private:

			void registerToExceptionInternal( ErrorCallback &cb );
			void unregisterFromExceptionInternal( ErrorCallback &cb );

			Engine();
			Engine( Engine const& src );

			EngineImpl		&m_EngineImpl;

		};
	}
}