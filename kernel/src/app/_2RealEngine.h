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

#include <string>

namespace _2Real
{

	template< typename T >
	class SingletonHolder;
	class EngineImpl;

	namespace app
	{
		class BundleHandle;

		class Engine
		{

			template< typename T >
			friend class _2Real::SingletonHolder;

		public:

			static Engine& instance();

			void setBaseDirectory( std::string const& directory );

			app::BundleHandle loadBundle( std::string const& libraryPath );

			void clear();
			void clearBlockInstances();

			void registerToException( ExceptionCallback callback, void *userData = nullptr );
			void unregisterFromException( ExceptionCallback callback, void *userData = nullptr );

			template< typename Callable >
			void registerToException( Callable &callable, void ( Callable::*callback )( Exception const&, BlockHandle const& ) )
			{
				AbstractExceptionCallbackHandler *handler = new ExceptionCallbackHandler< Callable >( callable, callback );
				registerToExceptionInternal( *handler );
			}

			template< typename Callable >
			void unregisterFromException( Callable &callable, void ( Callable::*callback )( Exception const&, BlockHandle const& ) )
			{
				AbstractExceptionCallbackHandler *handler = new ExceptionCallbackHandler< Callable >( callable, callback );
				registerToExceptionInternal( *handler );
			} 

		private:

			void registerToExceptionInternal( AbstractExceptionCallbackHandler &handler );
			void unregisterFromExceptionInternal( AbstractExceptionCallbackHandler &handler );

			Engine();
			Engine( Engine const& src );

			EngineImpl		&m_EngineImpl;

		};
	}
}