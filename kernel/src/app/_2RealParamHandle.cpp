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

#include "app/_2RealParamHandle.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealParam.h"

using std::string;

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "handle not initialized" );\

namespace _2Real
{
	namespace app
	{
		ParamHandle::ParamHandle() :
			m_Param( nullptr )
		{
		}

		ParamHandle::ParamHandle( Param &param ) :
			m_Param( &param )
		{
		}

		string const& ParamHandle::getName() const
		{
			checkHandle( m_Param );
			return m_Param->getName();
		}

		string const& ParamHandle::getLongTypename() const
		{
			checkHandle( m_Param );
			return m_Param->getLongTypename();
		}

		string const& ParamHandle::getTypename() const
		{
			checkHandle( m_Param );
			return m_Param->getTypename();
		}
	}
}