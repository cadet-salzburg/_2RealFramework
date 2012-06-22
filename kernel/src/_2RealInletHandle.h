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

	class Inlet;

	class InletHandle : public ParameterHandle
	{

	public:

		InletHandle();
		InletHandle( Inlet &slot );
		InletHandle( InletHandle const& src );
		InletHandle& operator=( InletHandle const& src );

		template< typename Datatype >
		Datatype const& getReadableRef() const
		{
			if ( !m_Inlet )
			{
				std::ostringstream msg;
				msg << "input handle was not initialized by framework";
				throw UninitializedHandleException( msg.str() );
			}

			Datatype const& data = extractFrom< Datatype >( getCurrentValue() );
			return data;
		}

		template< typename Datatype >
		Datatype * getWriteableCopy()
		{
			if ( !m_Inlet )
			{
				std::ostringstream msg;
				msg << "input handle was not initialized by framework";
				throw UninitializedHandleException( msg.str() );
			}

			Datatype const& data = extractFrom< Datatype >( getCurrentValue() );
			return new Datatype( data );
		}

	private:

		EngineData const&			getCurrentValue() const;
		Inlet						const* m_Inlet;

	};

}