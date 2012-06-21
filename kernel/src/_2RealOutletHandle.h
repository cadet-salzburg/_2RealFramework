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

#include "_2RealParameterHandle.h"
#include "_2RealEngineData.h"

namespace _2Real
{

	class Outlet;

	class OutletHandle : public ParameterHandle
	{

	public:

		OutletHandle();
		OutletHandle( Outlet &slot );
		OutletHandle( OutletHandle const& src );
		OutletHandle& operator=( OutletHandle const& src );

		template< typename Datatype >
		Datatype & getDataRef()
		{
			if ( !m_Outlet )
			{
				std::ostringstream msg;
				msg << "output handle was not initialized by framework";
				throw UninitializedHandleException( msg.str() );
			}

			Datatype *ptr = extractFrom< Datatype >( getOutletData() );
			return *ptr;
		}

		//template< typename Datatype >
		//void setOutletData( Datatype const& val )
		//{
		//	EngineData( val );
		//	setOutletData( val );
		//}

	private:

		EngineData	&		getOutletData();
		//void setOutletData( EngineData const& val );
		Outlet				*m_Outlet;

	};

}