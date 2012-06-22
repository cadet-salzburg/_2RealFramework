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

#pragma once

#include "_2RealTimestampedData.h"
#include "_2RealHelpersInternal.h"

#include "Poco/Mutex.h"
#include "Poco/BasicEvent.h"

#include <set>
#include <iostream>
#include <assert.h>

namespace _2Real
{

	// within the data buffer, data items are stored by the order which they arrived in
	typedef std::list< TimestampedData >		DataBuffer;

	// synchronisation is not part of the policy
	class AbstractInsertionPolicy
	{
	public:
		virtual bool insertData( TimestampedData const& data, DataBuffer &buffer ) = 0;
	};

	class RemoveFirst : public AbstractInsertionPolicy
	{

	public:

		RemoveFirst( const unsigned int max ) :
			m_Max( max )
		{
#ifdef _DEBUG
			if ( max == 0 )
			{
				std::cout << "inlet buffer size set to 0, must at least be 1" << std::endl;
				assert( NULL );
			}
#endif
		}

		bool insertData( TimestampedData const& data, DataBuffer &buffer )
		{
			if ( buffer.size() >= m_Max )
			{
				buffer.pop_front();
			}

			buffer.push_back( data );
			return true;
		}

	private:

		unsigned int				m_Max;

	};

	class InletBuffer
	{

		InletBuffer( EngineData const& defaultData ) :
			m_InsertionPolicy( new RemoveFirst( 20 ) ),
			m_Notify( true ),
			m_DefaultData( defaultData, 0 )
		{
		}

		~InletBuffer()
		{
			delete m_InsertionPolicy;
		}

		// this may be call simultaneously by many threads
		void receiveData( TimestampedData &data )
		{
			m_NotificationAccess.lock();
			if ( m_Notify )
			{
				// this will lead to an evaluation of the update condition
				// which in turn can disable 'm_Notify'
				// or it can reject the data completely
				m_DataReceived.notify( this, data );		// blocking notify here ( can't afford more threads )

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
		TimestampedData const& getCurrentData()
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
			return m_TriggeringData;
		}

		// called by i/o manager after an update
		// b/c default values & other data values need to be handled
		void update()
		{
			m_ReceivedAccess.lock();

			// TODO:
			// while iterating over this, it might actually be receiving data
			// since the trigger flag is, at this point, still not set
			// not sure if the mutex here is the best solution?
			for ( DataBuffer::iterator it = m_ReceivedDataItems.end(); it != m_ReceivedDataItems.begin(); --it )
			{
				m_DataReceived.notify( this, *it );
			}

			// oh, and also try with the default value
			m_DataAccess.lock();
			m_DataReceived.notify( this, m_DefaultData );
			m_DataAccess.unlock();

			m_NotificationAccess.lock();
			m_Notify = true;
			m_NotificationAccess.unlock();

			m_ReceivedAccess.unlock();
		}

		// called by an update manager if an update condition for this buffer was fulfilled
		// update manager is responsible for calling this with the right data
		void disableNotifications( TimestampedData const& data )
		{
			m_Notify = false;		// no sync here, since access is locked from within receiveData
									// TODO: find out if i need to declare this var volatile oO

			Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
			m_TriggeringData = data;
		}

		// let's just assume that this might, at some point, change
		void setInsertionPolicy( AbstractInsertionPolicy &policy )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
			delete m_InsertionPolicy;
			m_InsertionPolicy = &policy;
		}

		// setting an inlets' value will set the default data, basically
		void setDefaultData( TimestampedData const& defaultData )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
			m_DefaultData = defaultData;
		}

	private:

		DataBuffer							m_ReceivedDataItems;	// holds all received data items
		TimestampedData						m_TriggeringData;		// holds the data item which triggered the update condition
		Poco::BasicEvent< TimestampedData >	m_DataReceived;			// yay.
		TimestampedData						m_DefaultData;
		bool								m_Notify;				// this is set to false as soon as any received data
		mutable Poco::FastMutex				m_DataAccess;			// i use this for synchronizing policy, data access & default access oO
		mutable Poco::FastMutex				m_ReceivedAccess;
		mutable Poco::FastMutex				m_NotificationAccess;
		AbstractInsertionPolicy				*m_InsertionPolicy;

	};

}