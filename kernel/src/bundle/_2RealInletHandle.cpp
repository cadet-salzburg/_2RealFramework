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

#include "bundle/_2RealInletHandle.h"
#include "engine/_2RealInlet.h"

#define checkValidity( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "inlet handle not initialized" );

namespace _2Real
{
	namespace bundle
	{
		InletHandle::InletHandle() :
			m_Inlet( nullptr )
		{
		}

		InletHandle::InletHandle( Inlet &inlet ) :
			m_Inlet( &inlet )
		{
			m_Inlet->registerHandle( *this );
		}

		InletHandle::~InletHandle()
		{
			if ( isValid() ) m_Inlet->unregisterHandle( *this );
		}

		InletHandle::InletHandle( InletHandle const& other ) :
			m_Inlet( other.m_Inlet )
		{
			if ( isValid() ) m_Inlet->registerHandle( *this );
		}

		InletHandle& InletHandle::operator=( InletHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_Inlet->unregisterHandle( *this );
			}

			m_Inlet = other.m_Inlet;

			if ( isValid() )
			{
				m_Inlet->registerHandle( *this );
			}

			return *this;
		}

		bool InletHandle::isValid() const
		{
			return m_Inlet != nullptr;
		}

		Any InletHandle::getCurrentData() const
		{
			return m_Inlet->getData().getData();
		}

		void InletHandle::invalidate()
		{
			m_Inlet = nullptr;
		}

		bool InletHandle::hasUpdated() const
		{
			checkValidity( m_Inlet );
			return m_Inlet->hasUpdated();
		}

		bool InletHandle::valueChanged() const
		{
			checkValidity( m_Inlet );
			return m_Inlet->valueChanged();
		}
	}
}