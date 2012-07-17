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

#include <vector>
#include <string>

#include "helpers/_2RealHandle.h"
#include "app/_2RealCallbacks.h"



namespace _2Real
{
	template< typename T >
	class FunctionBlock;

	namespace app
	{
		class OutletHandle;
		class BlockInfo;

		class ContextBlockHandle : private Handle
		{

		public:

			typedef std::vector< OutletHandle >					OutletHandles;
			typedef std::vector< OutletHandle >::iterator		OutletHandleIterator;
			typedef std::vector< OutletHandle >::const_iterator	OutletHandleConstIterator;

			ContextBlockHandle();
			ContextBlockHandle( FunctionBlock< ContextBlockHandle > &block );
			virtual ~ContextBlockHandle();
			ContextBlockHandle( ContextBlockHandle const& other );
			ContextBlockHandle& operator=( ContextBlockHandle const& other );

			BlockInfo const& getBlockInfo() const;

			bool isValid() const;
			bool operator==( ContextBlockHandle const& other ) const;
			bool operator!=( ContextBlockHandle const& other ) const;
			bool operator<( ContextBlockHandle const& other ) const;
			bool operator<=( ContextBlockHandle const& other ) const;
			bool operator>( ContextBlockHandle const& other ) const;
			bool operator>=( ContextBlockHandle const& other ) const;

			OutletHandle &			getOutletHandle( std::string const& name ) const;
			OutletHandles const&	getAllOutletHandles() const;

			// callback registration for free functions
			void registerToNewData( BlockDataCallback callback, void *userData = nullptr ) const;
			void unregisterFromNewData( BlockDataCallback callback, void *userData = nullptr ) const;

			// callback registration for member functions
			template< typename TCallable >
			void registerToNewData( TCallable &callable, void ( TCallable::*callback )( std::list< AppData > const& ) ) const
			{
				BlockCallback *cb = new MemberCallback< TCallable, std::list< AppData > const& >( callable, callback );
				registerToNewDataInternal( *cb );
			}

			template< typename TCallable >
			void unregisterFromNewData( TCallable &callable, void ( TCallable::*callback )( std::list< AppData > const& ) ) const
			{
				BlockCallback *cb = new MemberCallback< TCallable, std::list< AppData > const& >( callable, callback );
				unregisterFromNewDataInternal( *cb );
			}

		protected:

			void invalidate();

		private:

			void registerToNewDataInternal( BlockCallback &cb ) const;
			void unregisterFromNewDataInternal( BlockCallback &cb ) const;

			FunctionBlock< ContextBlockHandle >		*m_Block;

		};

	}
}
