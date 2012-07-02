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

#include "app/_2RealParamHandle.h"
#include "helpers/_2RealEngineData.h"

namespace _2Real
{
	class Parameter;

	namespace app
	{
		class ParameterHandle : public ParamHandle
		{

		public:

			ParameterHandle();
			ParameterHandle( Parameter &param );

			using ParamHandle::getName;
			using ParamHandle::getTypename;
			using ParamHandle::getLongTypename;

			template< typename T >
			void setValue( T const& value )
			{
				setParameterValue( EngineData( value ) );
			}

		private:

			void		setParameterValue( EngineData const& data );
			Parameter	*m_Parameter;

		};
	}
}