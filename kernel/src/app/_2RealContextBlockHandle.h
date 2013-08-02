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

#include "app/_2RealCommon.h"
#include "app/_2RealCallbacks.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class FunctionBlock;

	namespace app
	{
		class BlockMetainfo;
		class OutletHandle;

		class ContextBlockHandle
		{

		public:

			typedef std::vector< OutletHandle >					OutletHandles;
			typedef std::vector< OutletHandle >::iterator		OutletHandleIterator;
			typedef std::vector< OutletHandle >::const_iterator	OutletHandleConstIterator;

			ContextBlockHandle();
			explicit ContextBlockHandle( std::shared_ptr< FunctionBlock > );

			bool					isValid() const;

			BlockMetainfo			getBlockMetainfo() const;

			OutletHandle			getOutletHandle( std::string const& );
			void					getAllOutletHandles( OutletHandles & );

			void registerToNewData( BlockDataCallback callback, void *userData = nullptr ) const;
			void unregisterFromNewData( BlockDataCallback callback, void *userData = nullptr ) const;

			template< typename TCallable >
			void registerToNewData( TCallable &callable, void ( TCallable::*callback )( std::vector< std::shared_ptr< const CustomType > > ) ) const
			{
				mHandle.registerToNewData< TCallable >( callable, callback );
			}

			template< typename TCallable >
			void unregisterFromNewData( TCallable &callable, void ( TCallable::*callback )( std::vector< std::shared_ptr< const CustomType > > ) ) const
			{
				mHandle.registerToNewData< TCallable >( callable, callback );
			}

		private:

			std::weak_ptr< FunctionBlock >		mImpl;

		};
	}
}
