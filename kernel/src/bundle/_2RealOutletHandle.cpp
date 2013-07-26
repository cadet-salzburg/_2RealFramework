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

#include "bundle/_2RealOutletHandle.h"
#include "engine/_2RealOutlet.h"

#define checkValidity( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "handle not initialized" );

namespace _2Real
{
	namespace bundle
	{
		OutletHandle::OutletHandle() :
			m_Outlet( nullptr )
		{
		}

		OutletHandle::OutletHandle( Outlet &outlet ) :
			m_Outlet( &outlet )
		{
			outlet.registerHandle( *this );
		}

		OutletHandle::~OutletHandle()
		{
			if ( isValid() ) m_Outlet->unregisterHandle( *this );
		}

		OutletHandle::OutletHandle( OutletHandle const& src ) :
			m_Outlet( src.m_Outlet )
		{
			if ( isValid() ) m_Outlet->registerHandle( *this );
		}

		OutletHandle& OutletHandle::operator=( OutletHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_Outlet->unregisterHandle( *this );
			}

			m_Outlet = other.m_Outlet;

			if ( isValid() )
			{
				m_Outlet->registerHandle( *this );
			}

			return *this;
		}

		bool OutletHandle::isValid() const
		{
			return m_Outlet != nullptr;
		}

		void OutletHandle::invalidate()
		{
			m_Outlet = nullptr;
		}

		std::shared_ptr< CustomType > OutletHandle::getWriteableRef()
		{
			checkValidity( m_Outlet );
			return m_Outlet->getWriteableData();
		}

		void OutletHandle::discard()
		{
			checkValidity( m_Outlet );
			m_Outlet->discard();
		}
	}
}