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

#include <string.h>

#include "app/_2RealCallbacks.h"

namespace _2Real
{
	class OutletIO;

	namespace app
	{
		class InletHandle;
		class TypeMetainfo;

		class OutletHandle
		{

		public:

			OutletHandle();
			OutletHandle( std::shared_ptr< OutletIO > );

			bool isValid() const;

			std::string const&		getName() const;
			app::TypeMetainfo		getType() const;

			std::shared_ptr< CustomType >	makeData() const;

			bool					link( InletHandle );
			void					unlinkFrom( InletHandle );

			std::shared_ptr< const CustomType >		getCurrentData() const;

			void registerToNewData( OutletDataCallback callback, void *userData = nullptr ) const;
			void unregisterFromNewData( OutletDataCallback callback, void *userData = nullptr ) const;

			template< typename TCallable >
			void registerToNewData( TCallable &callable, void ( TCallable::*callback )( std::shared_ptr< const CustomType > ) ) const
			{
				OutletCallback *cb = new MemberCallback< TCallable, std::shared_ptr< const CustomType > >( callable, callback );
				registerToNewDataInternal( *cb );
			}

			template< typename TCallable >
			void unregisterFromNewData( TCallable &callable, void ( TCallable::*callback )( std::shared_ptr< const CustomType > ) ) const
			{
				OutletCallback *cb = new MemberCallback< TCallable, std::shared_ptr< const CustomType > >( callable, callback );
				unregisterFromNewDataInternal( *cb );
			}

		private:

			friend class InletHandle;

			void registerToNewDataInternal( OutletCallback &cb ) const;
			void unregisterFromNewDataInternal( OutletCallback &cb ) const;

			std::weak_ptr< OutletIO >		mImpl;

		};
	}
}
