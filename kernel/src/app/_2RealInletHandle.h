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
#include "app/_2RealParameterHandle.h"

namespace _2Real
{
	class Inlet;

	namespace app
	{
		class OutletHandle;

		class InletHandle : public ParameterHandle
		{

		public:

			enum InletUpdatePolicy
			{
				ALWAYS,
				NEWER_DATA_SINGLE_WEIGHT,
				NEWER_DATA_GROUP_WEIGHT,
			};

			InletHandle();
			InletHandle( Inlet &inlet );

			using ParameterHandle::getName;
			using ParameterHandle::getTypename;
			using ParameterHandle::getLongTypename;

			void setUpdatePolicy( const InletUpdatePolicy p );

			void linkTo( OutletHandle &outletHandle );
			void unlinkFrom( OutletHandle &outletHandle );

			template< typename T >
			void setDefaultValue( T const& value )
			{
				setDefaultValue( EngineData( value ) );
			}

			// returns the inlet's most recent input data
			// updates right before an update() -> stays the same until next update()
			AppData				getCurrentInput() const;

			void				setBufferSize( const unsigned int size );

		private:

			void				setDefaultValue( EngineData const& data );

			Inlet				*m_Inlet;

		};
	}
}