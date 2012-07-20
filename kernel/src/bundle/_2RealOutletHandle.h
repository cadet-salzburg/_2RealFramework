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

#include "helpers/_2RealAny.h"
#include "helpers/_2RealException.h"

#include <sstream>

namespace _2Real
{
	class Outlet;

	namespace bundle
	{
		class OutletHandle
		{

		public:

			OutletHandle();
			OutletHandle( Outlet &outlet );
			OutletHandle( OutletHandle const& other );
			OutletHandle& operator=( OutletHandle const& other );
			virtual ~OutletHandle();

			template< typename Datatype >
			Datatype & getWriteableRef()
			{
				if ( !m_Outlet )
				{
					std::ostringstream msg;
					msg << "output handle not initialized";
					throw UninitializedHandleException( msg.str() );
				}

				Any &curr = getCurrentData();
				Datatype &data = extractFrom< Datatype >( curr );
				return data;
			}

			void discard();

			bool isValid() const;
			void invalidate();

		private:

			Any &		getCurrentData();
			Outlet				*m_Outlet;

		};
	}
}