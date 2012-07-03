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
#include "engine/_2RealParameter.h"

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "parameter handle not initialized" );\

namespace _2Real
{
	namespace app
	{
		ParameterHandle::ParameterHandle() :
			m_Parameter( nullptr )
		{
		}

		ParameterHandle::ParameterHandle( Parameter &param ) :
			m_Parameter( &param )
		{
		}

		std::string const& ParameterHandle::getName() const
		{
			checkHandle( m_Parameter );
			return m_Parameter->getName();
		}

		std::string const& ParameterHandle::getLongTypename() const
		{
			checkHandle( m_Parameter );
			return m_Parameter->getLongTypename();
		}

		std::string const& ParameterHandle::getTypename() const
		{
			checkHandle( m_Parameter );
			return m_Parameter->getTypename();
		}
	}
}