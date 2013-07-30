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

#include <vector>
#include <string>
#include <memory>

#ifdef _UNIX
	#include <limits.h>
#endif

#define NO_TIMEOUT	LONG_MAX

namespace _2Real
{
	class FunctionBlock;

	namespace app
	{
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;
		struct BlockInfo;

		class BlockHandle
		{

		public:

			typedef std::vector< InletHandle >						InletHandles;
			typedef std::vector< InletHandle >::iterator			InletHandleIterator;
			typedef std::vector< InletHandle >::const_iterator		InletHandleConstIterator;
			typedef std::vector< OutletHandle >						OutletHandles;
			typedef std::vector< OutletHandle >::iterator			OutletHandleIterator;
			typedef std::vector< OutletHandle >::const_iterator		OutletHandleConstIterator;
			typedef std::vector< ParameterHandle >					ParameterHandles;
			typedef std::vector< ParameterHandle >::iterator		ParameterHandleIterator;
			typedef std::vector< ParameterHandle >::const_iterator	ParameterHandleConstIterator;

			BlockHandle();
			BlockHandle( std::shared_ptr< FunctionBlock > );

			bool isValid() const;

			BlockInfo const& getBlockInfo() const;

			void setUpdateRate( const double updatesPerSecond );	// negative or zero: no time based update at all

			void setup();											// will stop the block and then set it up -> a new start is needed
			void start();
			void stop( const long timeout = NO_TIMEOUT );
			void destroy( const long timeout = NO_TIMEOUT );

			InletHandle					getInletHandle( std::string const& );
			OutletHandle				getOutletHandle( std::string const& );
			ParameterHandle				getParameterHandle( std::string const& );
			void						getAllInletHandles( InletHandles & );
			void						getAllOutletHandles( OutletHandles & );
			void						getAllParameterHandles( ParameterHandles & );

			void registerToNewData( BlockDataCallback callback, void *userData = nullptr ) const;
			void unregisterFromNewData( BlockDataCallback callback, void *userData = nullptr ) const;

			template< typename TCallable >
			void registerToNewData( TCallable &callable, void ( TCallable::*callback )( std::vector< std::shared_ptr< const CustomType > > ) ) const
			{
				BlockCallback *cb = new MemberCallback< TCallable, std::vector< std::shared_ptr< const CustomType > > >( callable, callback );
				registerToNewDataInternal( *cb );
			}

			template< typename TCallable >
			void unregisterFromNewData( TCallable &callable, void ( TCallable::*callback )( std::vector< std::shared_ptr< const CustomType > > ) ) const
			{
				BlockCallback *cb = new MemberCallback< TCallable, std::vector< std::shared_ptr< const CustomType > > >( callable, callback );
				unregisterFromNewDataInternal( *cb );
			}

			// true if block was started ( but not if singlestep was called )
			//bool isRunning() const;
			//std::string const& getIdAsString() const;
			//void singleStep();									// if block was not stopped before, this will not do anything at all

		private:

			void registerToNewDataInternal( BlockCallback &cb ) const;
			void unregisterFromNewDataInternal( BlockCallback &cb ) const;

			std::weak_ptr< FunctionBlock >		mImpl;

		};
	}
}
