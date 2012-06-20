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

#include "Poco/Delegate.h"

using std::string;
using std::find;
using std::ostringstream;

namespace _2Real
{

	Outlet::Outlet( string const& name, string const& longTypename, string const& typeName, EngineData const& emptyData ) :
		Parameter( name, longTypename, typeName ),
		m_Engine( EngineImpl::instance() ),
		m_CurrentDataItem( emptyData, 0 ),
		m_LastDataItem( emptyData, 0 )
	{
	}

	void Outlet::update()
	{
		Poco::FastMutex::ScopedLock lock( m_Access );

		if ( !m_CurrentDataItem.isEmpty() )
		{
			m_LastDataItem = TimestampedData( m_CurrentDataItem.getData(), m_Engine.getElapsedTime() );
			m_CurrentDataItem.cloneData( m_LastDataItem );

			m_InletEvent.notify( this, m_LastDataItem );
			OutputData data( m_LastDataItem.getData(), Parameter::getTypename(), Parameter::getName() );
			m_CallbackEvent.notify( this, data );
		}

		//// send all current data items and empty the queue in the process - at the end, only m_NewestData will hold the newest data
		//for ( DataItemList::iterator it = m_DataItemsToSend.begin(); it != m_DataItemsToSend.end(); /**/ )
		//{
		//	// first notify the inlets
		//	m_InletEvent.notify( this, *it );

		//	// create OutletData for application callbacks
		//	OutputData data( m_LastDataItem.getData(), Parameter::getTypename(), Parameter::getName() );
		//	m_CallbackEvent.notify( this, data );

		//	m_LastDataItem = *it;
		//	it = m_DataItemsToSend.erase( it );
		//}

		// copy the value that was sent into the current data
		m_CurrentDataItem.cloneData( m_LastDataItem );
	}

	const EngineData Outlet::getLastData() const
	{
		return m_LastDataItem.getData();
	}

	const OutputData Outlet::getLastOutputData() const
	{
		return OutputData( m_LastDataItem.getData(), Parameter::getTypename(), Parameter::getName() );
	}

	//void Outlet::setCurrentData( EngineData const& value )
	//{
	//	if ( value.getTypeinfo().name() != Parameter.getLongTypename() )
	//	{
	//		ostringstream msg;
	//		msg << "datatype mismatch: " << Parameter::getLongTypename() << " vs. value type " << value.getTypeinfo().name();
	//		throw TypeMismatchException(msg.str());
	//	}

	//	m_DataItemsToSend.push_back( TimestampedData( value, m_Engine.getElapsedTime() ) );
	//}

	EngineData & Outlet::getCurrentData()
	{
		return m_CurrentDataItem.getData();
	}

	//void Outlet::discardCurrent()
	//{
	//	m_DiscardCurrent = true;
	//}

	void Outlet::addListener( Inlet &inlet )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );

		LinkList::iterator it = find< LinkList::iterator, Inlet * >( m_Links.begin(), m_Links.end(), &inlet );
		if ( it == m_Links.end() )
		{
			m_InletEvent += Poco::delegate(&inlet, &Inlet::receiveData);
			m_Links.push_back( &inlet );

			if ( !m_LastDataItem.isEmpty() )
			{
				inlet.receiveData( m_LastDataItem );
			}
		}
	}

	void Outlet::removeListener( Inlet &inlet )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );

		LinkList::iterator it = find< LinkList::iterator, Inlet * >( m_Links.begin(), m_Links.end(), &inlet );
		if ( it != m_Links.end() )
		{
			m_InletEvent -= Poco::delegate(&inlet, &Inlet::receiveData);
			m_Links.erase( it );
		}
	}

	void Outlet::registerCallback( OutletFunctionCallback &callback )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );

		m_CallbackEvent += Poco::delegate( &callback, &OutletFunctionCallback::invoke );

		if ( !m_LastDataItem.isEmpty() )
		{
			OutputData data( m_LastDataItem.getData(), Parameter::getTypename(), Parameter::getName() );
			callback.invoke( data );
		}
	}

	void Outlet::unregisterCallback( OutletFunctionCallback &callback )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );

		m_CallbackEvent -= Poco::delegate( &callback, &OutletFunctionCallback::invoke );
	}

	void Outlet::registerCallback( AbstractOutletCallbackHandler &handler )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );

		m_CallbackEvent += Poco::delegate( &handler, &AbstractOutletCallbackHandler::invoke );

		if ( !m_LastDataItem.isEmpty() )
		{
			OutputData data( m_LastDataItem.getData(), Parameter::getTypename(), Parameter::getName() );
			handler.invoke( data );
		}
	}

	void Outlet::unregisterCallback( AbstractOutletCallbackHandler &handler )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );

		m_CallbackEvent -= Poco::delegate(&handler, &AbstractOutletCallbackHandler::invoke);
	}

	// hm
	void Outlet::clearLinks()
	{
		for ( LinkList::iterator it = m_Links.begin(); it != m_Links.end(); ++it )
		{
			(*it)->unlink( *this );
			m_InletEvent -= Poco::delegate( *it, &Inlet::receiveData );
		}
	}

	void Outlet::resetLinks()
	{
		Poco::FastMutex::ScopedLock lock( m_Access );

		m_InletEvent.clear();

		for( LinkList::iterator it = m_Links.begin(); it != m_Links.end(); /**/ )
		{
			(*it)->unlink(*this);
			it = m_Links.erase(it);
			break;
		}
	}

}