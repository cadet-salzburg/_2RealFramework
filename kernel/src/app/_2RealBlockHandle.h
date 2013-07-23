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

#ifdef _UNIX
	#include <limits.h>
#endif

#define NO_TIMEOUT	LONG_MAX

namespace _2Real
{
	template< typename T >
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

			typedef std::vector< InletHandle >					InletHandles;
			typedef std::vector< InletHandle >::iterator		InletHandleIterator;
			typedef std::vector< InletHandle >::const_iterator	InletHandleConstIterator;

			typedef std::vector< OutletHandle >					OutletHandles;
			typedef std::vector< OutletHandle >::iterator		OutletHandleIterator;
			typedef std::vector< OutletHandle >::const_iterator	OutletHandleConstIterator;

			typedef std::vector< ParameterHandle >					ParameterHandles;
			typedef std::vector< ParameterHandle >::iterator		ParameterHandleIterator;
			typedef std::vector< ParameterHandle >::const_iterator	ParameterHandleConstIterator;

			BlockHandle();
			BlockHandle( FunctionBlock< BlockHandle > &block );
			~BlockHandle();
			BlockHandle( BlockHandle const& other );
			BlockHandle& operator=( BlockHandle const& other );

			bool isValid() const;
			void invalidate();
			bool operator==( BlockHandle const& other ) const;
			bool operator!=( BlockHandle const& other ) const;
			bool operator<( BlockHandle const& other ) const;
			bool operator<=( BlockHandle const& other ) const;
			bool operator>( BlockHandle const& other ) const;
			bool operator>=( BlockHandle const& other ) const;

			BlockInfo const& getBlockInfo() const;

			// negative or zero: no time based update at all
			void setUpdateRate( const double updatesPerSecond );

			void setup();										// will stop the block and then set it up -> a new start is needed
			void start();
			void stop( const long timeout = NO_TIMEOUT );		// um, yeah. no timeout is probably a bad idea :)
			void kill( const long timeout = NO_TIMEOUT );		// see above
			void singleStep();									// if block was not stopped before, this will not do anything at all

			InletHandle &			getInletHandle( std::string const& name ) const;
			OutletHandle &			getOutletHandle( std::string const& name ) const;
			ParameterHandle &		getParameterHandle( std::string const& name ) const;
			InletHandles const&		getAllInletHandles() const;
			OutletHandles const&	getAllOutletHandles() const;
			ParameterHandles const&	getAllParameterHandles() const;

			// true if block was started ( but not if singlestep was called )
			bool isRunning() const;

			// callback registration for free functions
			void registerToNewData( BlockDataCallback callback, void *userData = nullptr ) const;
			void unregisterFromNewData( BlockDataCallback callback, void *userData = nullptr ) const;

			// callback registration for member functions
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

			std::string const& getIdAsString() const;

		private:

			void registerToNewDataInternal( BlockCallback &cb ) const;
			void unregisterFromNewDataInternal( BlockCallback &cb ) const;

			FunctionBlock< BlockHandle >		*m_Block;

		};
	}
}
