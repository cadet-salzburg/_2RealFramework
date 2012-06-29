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

#include "_2RealEngineData.h"
#include "_2RealException.h"

#include <sstream>

namespace _2Real
{
	class Inlet;

	namespace bundle
	{
		class InletHandle
		{

		public:

			InletHandle();
			InletHandle( Inlet &slot );

			template< typename Datatype >
			Datatype const& getReadableRef() const
			{
				if ( m_Inlet == nullptr )
				{
					std::ostringstream msg;
					msg << "input handle not initialized";
					throw UninitializedHandleException( msg.str() );
				}

				EngineData curr = getCurrentData();
				Datatype const& data = extractFrom< Datatype >( curr );
				return data;
			}

			template< typename Datatype >
			Datatype * getWriteableCopy()
			{
				if ( m_Inlet == nullptr )
				{
					std::ostringstream msg;
					msg << "input handle not initialized";
					throw UninitializedHandleException( msg.str() );
				}

				EngineData curr = getCurrentData();
				Datatype const& data = extractFrom< Datatype >( curr );
				return new Datatype( data );
			}

		private:

			EngineData			getCurrentData() const;
			Inlet				const* m_Inlet;

		};
	}
}