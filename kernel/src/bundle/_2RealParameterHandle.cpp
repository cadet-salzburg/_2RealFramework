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

#include "bundle/_2RealParameterHandle.h"
#include "engine/_2RealParameter.h"

#define checkValidity( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "handle not initialized" );

namespace _2Real
{
	namespace bundle
	{
		ParameterHandle::ParameterHandle() :
			m_Parameter( nullptr )
		{
		}

		ParameterHandle::ParameterHandle( Parameter &param ) :
			m_Parameter( &param )
		{
			m_Parameter->registerHandle( *this );
		}

		ParameterHandle::~ParameterHandle()
		{
			if ( isValid() ) m_Parameter->unregisterHandle( *this );
		}

		ParameterHandle::ParameterHandle( ParameterHandle const& other ) :
			m_Parameter( other.m_Parameter )
		{
			if ( isValid() ) m_Parameter->registerHandle( *this );
		}

		ParameterHandle& ParameterHandle::operator=( ParameterHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_Parameter->unregisterHandle( *this );
			}

			m_Parameter = other.m_Parameter;

			if ( isValid() )
			{
				m_Parameter->registerHandle( *this );
			}

			return *this;
		}

		bool ParameterHandle::isValid() const
		{
			return m_Parameter != nullptr;
		}

		void ParameterHandle::invalidate()
		{
			checkValidity( m_Parameter );
			m_Parameter = nullptr;
		}

		bool ParameterHandle::hasChanged() const
		{
			checkValidity( m_Parameter );
			return m_Parameter->hasChanged();
		}

		std::shared_ptr< const CustomType > ParameterHandle::getReadableRef() const
		{
			checkValidity( m_Parameter );
			return m_Parameter->getData();
		}

		void ParameterHandle::getWriteableCopy( CustomType &writeable ) const
		{
			checkValidity( m_Parameter );
			writeable.cloneFrom( *( m_Parameter->getData().get() ) );
		}
	}
}