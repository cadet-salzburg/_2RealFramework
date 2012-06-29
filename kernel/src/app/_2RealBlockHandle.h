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

#include "_2RealCallbacks.h"

#include <string>

namespace _2Real
{
	class FunctionBlock;
	class BlockData;

	namespace app
	{
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;

		class BlockHandle
		{

		public:

			enum InletUpdatePolicy
			{
				ALL_DATA_VALID,
				ALL_DATA_NEW,
			};

			BlockHandle();
			BlockHandle( FunctionBlock &block );

			BlockData const& getBlockData() const;

			void setUpdateRate( const double updatesPerSecond );
			void setInletUpdatePolicy( const InletUpdatePolicy p );

			void setUp();
			void start();
			void stop();
			//void destroy();
			//void singleStep();

			InletHandle		getInletHandle( std::string const& name );
			OutletHandle	getOutletHandle( std::string const& name );
			ParameterHandle	getParameterHandle( std::string const& name );

			void registerToNewData( BlockDataCallback callback, void *userData = nullptr );
			void unregisterFromNewData( BlockDataCallback callback, void *userData = nullptr );

			template< typename Callable >
			void registerToNewData( Callable &callable, void ( Callable::*callback )( std::list< AppData > const& ) )
			{
				AbstractBlockDataCallbackHandler *handler = new BlockDataCallbackHandler< Callable >( callable, callback );
				registerToNewDataInternal( *handler );
			}

			template< typename Callable >
			void unregisterFromNewData( Callable &callable, void ( Callable::*callback )( std::list< AppData > const& ) )
			{
				AbstractBlockDataCallbackHandler *handler = new BlockDataCallbackHandler< Callable >( callable, callback );
				unregisterFromNewDataInternal( *handler );
			}

		private:

			void registerToNewDataInternal( AbstractBlockDataCallbackHandler &handler );
			void unregisterFromNewDataInternal( AbstractBlockDataCallbackHandler &handler );

		private:

			FunctionBlock		*m_Block;

		};
	}
}