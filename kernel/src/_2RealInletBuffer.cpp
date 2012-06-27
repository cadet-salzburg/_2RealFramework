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

#include "_2RealInletBuffer.h"
#include "_2RealInlet.h"
#include "_2RealInletHandle.h"
#include "_2RealInletBasedTrigger.h"

#include <iostream>
#include <assert.h>

using std::cout;
using std::endl;
using std::auto_ptr;

namespace _2Real
{

	RemoveFirst::RemoveFirst( const unsigned int max ) :
		m_Max( max )
	{
#ifdef _DEBUG
		if ( max == 0 )
		{
			assert( NULL );
		}
#endif
	}

	bool RemoveFirst::insertData( TimestampedData const& data, DataBuffer &buffer )
	{
		if ( buffer.size() >= m_Max )
		{
			buffer.pop_back();
		}

		buffer.push_front( data );
		return true;
	}

	InletBuffer::InletBuffer( EngineData const& defaultData ) :
		m_InsertionPolicy( new RemoveFirst( 20 ) ),
		m_Notify( true ),
		m_DefaultData( defaultData, 0 )
	{
	}

	// this may be call simultaneously by many threads
	void InletBuffer::receiveData( TimestampedData &data )
	{
		m_NotificationAccess.lock();
		if ( m_Notify )
		{
			// this will lead to an evaluation of the update condition
			// which in turn can disable 'm_Notify'
			// or it can reject the data completely
			for ( InletTriggerList::iterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); ++it )
			{
				( *it )->tryTriggerUpdate( data );
			}

			// at this point, notificytions might have been disabled
			m_NotificationAccess.unlock();
		}
		else
		{
			m_NotificationAccess.unlock();

			// ok, this data was sent to the buffer when
			// its trigger cond was already fulfilled -> store it!
			Poco::ScopedLock< Poco::FastMutex > lock( m_ReceivedAccess );
			m_InsertionPolicy->insertData( data, m_ReceivedDataItems );
		}
	}

	// called by i/o manager when it's time to actually update the inlets
	TimestampedData const& InletBuffer::getCurrentData() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );

#ifdef _DEBUG
		if ( m_TriggeringData.isEmpty() )
		{
			assert( NULL );
		}
#endif

		return m_TriggeringData;
	}

	// called by i/o manager after an update
	// b/c default values & other data values need to be handled
	// also, this happens after update policy was changed!
	void InletBuffer::updateDataBuffer()
	{
		m_ReceivedAccess.lock();

		// TODO:
		// while iterating over this, it might actually be receiving data
		// since the trigger flag is, at this point, still not set
		// not sure if the mutex here is the best solution?
		for ( DataBuffer::iterator dIt = m_ReceivedDataItems.begin(); dIt != m_ReceivedDataItems.end(); /**/ )
		{
			for ( InletTriggerList::iterator inIt = m_InletTriggers.begin(); inIt != m_InletTriggers.end(); ++inIt )
			{
				( *inIt )->tryTriggerUpdate( *dIt );
			}

			dIt = m_ReceivedDataItems.erase( dIt );
		}
		// oh, and also try with the default value
		m_DataAccess.lock();
		for ( InletTriggerList::iterator inIt = m_InletTriggers.begin(); inIt != m_InletTriggers.end(); ++inIt )
		{
			( *inIt )->tryTriggerUpdate( m_DefaultData );
		}

		m_DataAccess.unlock();

		m_NotificationAccess.lock();
		m_Notify = true;
		m_NotificationAccess.unlock();

		m_ReceivedAccess.unlock();
	}

	// called by an update manager if an update condition for this buffer was fulfilled
	// update manager is responsible for calling this with the right data
	void InletBuffer::disableTriggering( TimestampedData const& data )
	{
		m_Notify = false;		// no sync here, since access is locked from within receiveData
								// TODO: find out if i need to declare this var volatile oO

		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		m_TriggeringData = data;
	}

	// let's just assume that this might, at some point, change
	void InletBuffer::setInsertionPolicy( AbstractInsertionPolicy &policy )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ReceivedAccess );
		m_InsertionPolicy.reset( &policy );
	}

	// setting an inlets' value will set the default data, basically
	void InletBuffer::setDefaultData( TimestampedData const& defaultData )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		m_DefaultData = defaultData;
	}

	void InletBuffer::registerUpdateTrigger( AbstractInletBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_NotificationAccess );
		m_InletTriggers.push_back( &trigger );
	}

	void InletBuffer::unregisterUpdateTrigger( AbstractInletBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_NotificationAccess );
		for ( InletTriggerList::iterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); ++it )
		{
			if ( *it == &trigger )
			{
				m_InletTriggers.erase( it );
				break;
			}
		}
	}

}