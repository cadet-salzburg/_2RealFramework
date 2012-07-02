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

#include "app/_2RealParameterHandle.h"
#include "_2RealParameter.h"
#include "_2RealTimestampedData.h"

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "block handle not initialized" );\

namespace _2Real
{
	namespace app
	{
		ParameterHandle::ParameterHandle() :
			ParamHandle(),
			m_Parameter( nullptr )
		{
		}

		ParameterHandle::ParameterHandle( Parameter &param ) :
			ParamHandle( param ),
			m_Parameter( &param )
		{
		}

		void ParameterHandle::setParameterValue( EngineData const& data )
		{
			checkHandle( m_Parameter );
			m_Parameter->setData( TimestampedData( data, 0 ) );
		}
	}
}