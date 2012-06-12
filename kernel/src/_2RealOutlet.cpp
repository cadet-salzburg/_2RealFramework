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

#include "_2RealOutlet.h"
#include "_2RealInlet.h"
#include "_2RealException.h"
#include "_2RealServiceBlock.h"
#include "_2RealParameterData.h"
#include "_2RealEngineImpl.h"
#include "_2RealTypetable.h"

#include "Poco/Delegate.h"

#include <iostream>

namespace _2Real
{

	Outlet::Outlet( ParameterData const& metadata, Poco::Timestamp const& timestamp ) :
		Parameter( metadata ),
		m_SystemTime( timestamp ),
		m_DiscardCurrent( false ),
		m_HasData( false )
	{
		m_WriteData = metadata.getDefaultValue();
		m_NewestData = TimestampedData( m_WriteData, (long)m_SystemTime.elapsed() );
		m_WriteData.clone( m_WriteData );
		m_HasData = true;
	}

	void Outlet::clearLinks()
	{
		for ( std::list< Inlet * >::iterator it = m_LinkedInlets.begin(); it != m_LinkedInlets.end(); ++it )
		{
			(*it)->unlink( *this );
			m_InletEvent -= Poco::delegate( *it, &Inlet::receiveData );
		}
	}

	void Outlet::update()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		// insert last value into the map of items if it was not explicitly discarded
		if ( !m_DiscardCurrent )
		{
			m_DataItems.insert( std::make_pair(static_cast< long >(m_SystemTime.elapsed()), m_WriteData) );
		}
		else
		{
			m_DiscardCurrent = false;
		}

		m_HasData = !m_DataItems.empty();

		// send all current data items and empty the queue in the process - at the end, only m_NewestData will hold the newest data
		for ( std::multimap< long, EngineData >::iterator it = m_DataItems.begin(); it != m_DataItems.end(); /**/ )
		{
			m_NewestData = TimestampedData( it->second, it->first );
			m_InletEvent.notify( this, m_NewestData );
			OutputData data( m_NewestData.data(), Parameter::getKeyword(), Parameter::getName() );
			m_CallbackEvent.notify( this, data );
			it = m_DataItems.erase( it );
		}

		// this creates an empty object of the same datatype as write data
		m_WriteData.create( m_WriteData );
	}

	EngineData & Outlet::getDataForWriting()
	{
		return m_WriteData;
	}

	const EngineData Outlet::getCurrent() const
	{
		return m_NewestData.data();
	}

	OutputData Outlet::getOutputData() const
	{
		OutputData data( m_NewestData.data(), Parameter::getKeyword(), Parameter::getName() );
		return data;
	}

	void Outlet::createNewDataItem()
	{
		if ( !m_DiscardCurrent )
		{
			m_DataItems.insert( std::make_pair( static_cast< long >( m_SystemTime.elapsed() ), m_WriteData ) );
		}
		else
		{
			m_DiscardCurrent = false;
		}

		m_WriteData.create( m_WriteData );
	}

	void Outlet::discardCurrent()
	{
		m_DiscardCurrent = true;
	}

	void Outlet::addListener( Inlet &inlet )
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		std::list< Inlet * >::iterator it = std::find< std::list< Inlet * >::iterator, Inlet * >(m_LinkedInlets.begin(), m_LinkedInlets.end(), &inlet);
		if (it == m_LinkedInlets.end())
		{
			m_InletEvent += Poco::delegate(&inlet, &Inlet::receiveData);
			m_LinkedInlets.push_back( &inlet );
			if ( m_HasData )
			{
				inlet.receiveData( m_NewestData );
			}
		}
	}

	void Outlet::removeListener( Inlet &inlet )
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		std::list< Inlet * >::iterator it = std::find< std::list< Inlet * >::iterator, Inlet * >(m_LinkedInlets.begin(), m_LinkedInlets.end(), &inlet);
		if (it != m_LinkedInlets.end())
		{
			m_InletEvent -= Poco::delegate(&inlet, &Inlet::receiveData);
			m_LinkedInlets.erase( it );
		}
	}

	void Outlet::registerCallback( OutletFunctionCallback &callback )
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_CallbackEvent += Poco::delegate( &callback, &OutletFunctionCallback::invoke );

		if ( m_HasData )
		{
			OutputData data( m_NewestData.data(), Parameter::getKeyword(), Parameter::getName() );
			callback.invoke( data );
		}
	}

	void Outlet::unregisterCallback( OutletFunctionCallback &callback )
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_CallbackEvent -= Poco::delegate(&callback, &OutletFunctionCallback::invoke);
	}

	void Outlet::registerCallback( AbstractOutletCallbackHandler &handler )
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_CallbackEvent += Poco::delegate(&handler, &AbstractOutletCallbackHandler::invoke);

		if ( m_HasData )
		{
			OutputData data( m_NewestData.data(), Parameter::getKeyword(), Parameter::getName() );
			handler.invoke( data );
		}
	}

	void Outlet::unregisterCallback( AbstractOutletCallbackHandler &handler )
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_CallbackEvent -= Poco::delegate(&handler, &AbstractOutletCallbackHandler::invoke);
	}

	void Outlet::resetLinks()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_InletEvent.clear();

		for(std::list< Inlet * >::iterator it = m_LinkedInlets.begin(); it != m_LinkedInlets.end(); /**/)
		{
			(*it)->unlink(*this);
			it = m_LinkedInlets.erase(it);
			break;
		}
	}

}