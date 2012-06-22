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

#include "_2RealInletHandle.h"
#include "_2RealInlet.h"

namespace _2Real
{

	InletHandle::InletHandle() :
		ParameterHandle( "" ),
		m_Inlet( nullptr )
	{
	}

	InletHandle::InletHandle( Inlet &inlet ) :
		ParameterHandle( inlet.getName() ),
		m_Inlet( &inlet )
	{
	}

	InletHandle::InletHandle( InletHandle const& src ) :
		ParameterHandle( src ),
		m_Inlet( src.m_Inlet )
	{
	}

	InletHandle& InletHandle::operator=( InletHandle const& src )
	{
		ParameterHandle::operator=( src );
		m_Inlet = src.m_Inlet;

		return *this;
	}

	EngineData const& InletHandle::getCurrentValue() const
	{
		return m_Inlet->getCurrentValue();
	}

}