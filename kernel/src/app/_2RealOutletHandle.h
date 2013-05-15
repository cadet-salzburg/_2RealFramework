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
		class AppData;
		class InletHandle;

		class OutletHandle
		{

		public:

			OutletHandle();
			OutletHandle( OutletIO &outletIO );
			~OutletHandle();
			OutletHandle( OutletHandle const& other );
			OutletHandle& operator=( OutletHandle const& other );

			std::string const&	getName() const;
			const std::string	getLongTypename() const;
			std::string const&	getTypename() const;

			bool isValid() const;
			void invalidate();
			bool operator==( OutletHandle const& other ) const;
			bool operator!=( OutletHandle const& other ) const;
			bool operator<( OutletHandle const& other ) const;
			bool operator<=( OutletHandle const& other ) const;
			bool operator>( OutletHandle const& other ) const;
			bool operator>=( OutletHandle const& other ) const;

			bool link( InletHandle &inletHandle );
			bool linkWithConversion( InletHandle &inletHandle );
			void unlinkFrom( InletHandle &inletHandle );

			AppData				getLastOutput() const;

			void registerToNewData( OutletDataCallback callback, void *userData = nullptr ) const;
			void unregisterFromNewData( OutletDataCallback callback, void *userData = nullptr ) const;

			template< typename TCallable >
			void registerToNewData( TCallable &callable, void ( TCallable::*callback )( AppData const& ) ) const
			{
				OutletCallback *cb = new MemberCallback< TCallable, AppData const& >( callable, callback );
				registerToNewDataInternal( *cb );
			}

			template< typename TCallable >
			void unregisterFromNewData( TCallable &callable, void ( TCallable::*callback )( AppData const& ) ) const
			{
				OutletCallback *cb = new MemberCallback< TCallable, AppData const& >( callable, callback );
				unregisterFromNewDataInternal( *cb );
			}

			// new 06/05/13
			BlockHandle getOwningBlock();

		private:

			friend class InletHandle;

			void registerToNewDataInternal( OutletCallback &cb ) const;
			void unregisterFromNewDataInternal( OutletCallback &cb ) const;

			OutletIO			*m_OutletIO;

		};
	}
}
