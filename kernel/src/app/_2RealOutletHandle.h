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

#include "app/_2RealAppData.h"
#include "app/_2RealParamHandle.h"
#include "app/_2RealCallbacks.h"

#include<list>

namespace _2Real
{
	class Outlet;

	namespace app
	{
		class InletHandle;

		class OutletHandle : public ParamHandle
		{

		public:

			OutletHandle();
			OutletHandle( Outlet &outlet );

			void linkTo( InletHandle &inletHandle );
			void unlinkFrom( InletHandle &inletHandle );

			AppData				getLastOutput() const;

			void registerToNewData( app::OutletDataCallback callback, void *userData = nullptr );
			void unregisterFromNewData( app::OutletDataCallback callback, void *userData = nullptr );

			template< typename Callable >
			void registerToNewData( Callable &callable, void ( Callable::*callback )( AppData const& ) )
			{
				AbstractOutletDataCallbackHandler *handler = new OutletDataCallbackHandler< Callable >( callable, callback );
				registerToNewDataInternal( *handler );
			}

			template< typename Callable >
			void unregisterFromNewData( Callable &callable, void ( Callable::*callback )( AppData const& ) )
			{
				AbstractOutletDataCallbackHandler *handler = new OutletDataCallbackHandler< Callable >( callable, callback );
				unregisterFromNewDataInternal( *handler );
			}

		private:

			void registerToNewDataInternal( AbstractOutletDataCallbackHandler &handler );
			void unregisterFromNewDataInternal( AbstractOutletDataCallbackHandler &handler );

			Outlet			*m_Outlet;

		};
	}
}