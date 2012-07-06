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

#include "app/_2RealOutletHandle.h"
#include "app/_2RealInletHandle.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealAbstractUberBlock.h"

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "outlet handle not initialized" );\

namespace _2Real
{
	namespace app
	{
		OutletHandle::OutletHandle() :
			Handle(),
			m_OutletIO( nullptr )
		{
		}

		OutletHandle::OutletHandle( OutletIO &outletIO ) :
			Handle( &outletIO ),
			m_OutletIO( &outletIO )
		{
			m_OutletIO->registerHandle( *this );
		}

		OutletHandle::~OutletHandle()
		{
			if ( isValid() ) m_OutletIO->unregisterHandle( *this );
		}

		OutletHandle::OutletHandle( OutletHandle const& other ) :
			Handle( other.m_OutletIO ),
			m_OutletIO( other.m_OutletIO )
		{
			if ( isValid() ) m_OutletIO->registerHandle( *this );
		}

		OutletHandle& OutletHandle::operator=( OutletHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_OutletIO->unregisterHandle( *this );
			}

			Handle::operator=( other );
			m_OutletIO = other.m_OutletIO;

			if ( isValid() )
			{
				m_OutletIO->registerHandle( *this );
			}

			return *this;
		}

		AppData OutletHandle::getLastOutput() const
		{
			checkHandle( m_OutletIO );
			TimestampedData data = m_OutletIO->m_Outlet->getData();
			return AppData( data, m_OutletIO->m_Outlet->getTypename(), m_OutletIO->m_Outlet->getName() );
		}

		void OutletHandle::linkTo( InletHandle &inlet )
		{
			checkHandle( m_OutletIO );
			EngineImpl::instance().createLink( *( inlet.m_InletIO ), *m_OutletIO );
		}

		void OutletHandle::unlinkFrom( InletHandle &inlet )
		{
			checkHandle( m_OutletIO );
			EngineImpl::instance().destroyLink( *( inlet.m_InletIO ), *m_OutletIO );
		}

		void OutletHandle::registerToNewData( OutletDataCallback callback, void *userData ) const
		{
			checkHandle( m_OutletIO );
			OutletCallback *cb = new FunctionCallback< AppData const& >( callback, userData );
			m_OutletIO->m_AppEvent->addListener( *cb );
		}

		void OutletHandle::unregisterFromNewData( OutletDataCallback callback, void *userData ) const
		{
			checkHandle( m_OutletIO );
			OutletCallback *cb = new FunctionCallback< AppData const& >( callback, userData );
			m_OutletIO->m_AppEvent->removeListener( *cb );
		}

		void OutletHandle::registerToNewDataInternal( OutletCallback &cb ) const
		{
			checkHandle( m_OutletIO );
			m_OutletIO->m_AppEvent->addListener( cb );
		}

		void OutletHandle::unregisterFromNewDataInternal( OutletCallback &cb ) const
		{
			checkHandle( m_OutletIO );
			m_OutletIO->m_AppEvent->removeListener( cb );
		}

		std::string const& OutletHandle::getName() const
		{
			checkHandle( m_OutletIO );
			return m_OutletIO->m_Outlet->getName();
		}

		std::string const& OutletHandle::getLongTypename() const
		{
			checkHandle( m_OutletIO );
			return m_OutletIO->m_Outlet->getLongTypename();
		}

		std::string const& OutletHandle::getTypename() const
		{
			checkHandle( m_OutletIO );
			return m_OutletIO->m_Outlet->getTypename();
		}
	}
}