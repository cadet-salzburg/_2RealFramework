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

#include "engine/_2RealInletBuffer.h"
#include "engine/_2RealEngineImpl.h"

#include <assert.h>
#include <sstream>

using std::stringstream;

namespace _2Real
{

	RemoveOldest::RemoveOldest( const unsigned int max ) :
		m_Max( max )
	{
	}

	bool RemoveOldest::insertData( TimestampedData const& data, InletBuffer::DataBuffer &buffer )
	{
		while ( buffer.size() >= m_Max && !buffer.empty() )
		{
			// TODO: some sort of overflow cb for the app i guess
			// anyway, remove oldest elem from buffer
			buffer.pop_front();
		}

		buffer.push_back( data );
		return true;
	}

	InletBuffer::InletBuffer( Any const& defaultData, AnyOptionSet const& options ) :
		m_InsertionPolicy( new RemoveOldest( 1 ) ),
		m_Notify( false ),
		m_DefaultData( defaultData, 0 ),
		m_Engine( EngineImpl::instance() ),
		m_TriggeringData( defaultData, 0 ),
		m_BufferSize( 1 ),
		m_Options( options )
	{
		m_InsertionPolicy->insertData( m_DefaultData, m_ReceivedDataItems );
	}

	InletBuffer::~InletBuffer()
	{
		delete m_InsertionPolicy;
	}

	void InletBuffer::receiveData( Any const& data )
	{
		receiveData( TimestampedData( data, m_Engine.getElapsedTime() ) );
	}

	void InletBuffer::receiveData( std::string const& value )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );

		Any data;
		data.createNew( m_TriggeringData.getData() );

		stringstream s;
		s << value;
		data.readFrom( s );

		receiveData( TimestampedData( data, m_Engine.getElapsedTime() ) );
	}

	// this may be call simultaneously by many threads
	void InletBuffer::receiveData( TimestampedData const& data )
	{
		if ( !m_Options.isEmpty() )
		{
			Any const& value = data.getData();
			if ( !m_Options.isOption( value ) )
			{
				std::cout << "NOT AN OPTION!!!!!!!!!!!!" << std::endl;
				return;
			}
		}

		// sync point: only one of many threads can cause the update cond to be evaluated at once
		m_NotificationAccess.lock();
		if ( m_Notify )
		{
			m_TriggeringEvent.notify( data );
			m_NotificationAccess.unlock();
		}
		else
		{
			m_NotificationAccess.unlock();
			// once the cond was fulfilled, buffering starts
			Poco::ScopedLock< Poco::FastMutex > pLock( m_PolicyAccess );
			Poco::ScopedLock< Poco::FastMutex > dLock( m_DataAccess );
			m_InsertionPolicy->insertData( data, m_ReceivedDataItems );
		}
	}

	TimestampedData const& InletBuffer::getTriggeringData() const
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

	//TimestampedData const& InletBuffer::getOldestBufferData()
	//{
	//	Poco::ScopedLock< Poco::FastMutex > pLock( m_PolicyAccess );
	//	Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
	//	if ( m_ReceivedDataItems.empty() )
	//	{
	//		return m_TriggeringData;
	//	}
	//	else
	//	{
	//		TimestampedData const& data = m_ReceivedDataItems.front();
	//		m_ReceivedDataItems.pop_front();
	//		return data;
	//	}
	//}

	void InletBuffer::processBufferedData()
	{
		// no reception of data allowed during this time
		m_NotificationAccess.lock();
		m_Notify = true;
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		for ( DataBufferIterator dIt = m_ReceivedDataItems.begin(); dIt != m_ReceivedDataItems.end(); /**/ )
		{
			TimestampedData d = *dIt;
			m_TriggeringEvent.notify( d );

			if ( !m_Notify )
			{
				dIt = m_ReceivedDataItems.erase( dIt );
				break;
			}
			else
			{
				dIt = m_ReceivedDataItems.erase( dIt );
			}
		}

		m_TriggeringEvent.notify( m_TriggeringData ); // try fulfilling available cond!
		m_NotificationAccess.unlock();
	}

	// called by an update manager if an update condition for this buffer was fulfilled
	// update manager is responsible for calling this with the right data
	void InletBuffer::disableTriggering( TimestampedData const& data )
	{
		m_Notify = false;		// no sync here, since access is locked from within receiveData
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		m_TriggeringData = data;
	}

	void InletBuffer::setBufferSize( const unsigned int size )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_PolicyAccess );
		delete m_InsertionPolicy;
		m_InsertionPolicy = new RemoveOldest( size );
		m_BufferSize = size;
	}

	unsigned int InletBuffer::getBufferSize() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_PolicyAccess );
		return m_BufferSize;
	}

	//void InletBuffer::setDefaultData( Any const& defaultData )
	//{
	//	//Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
	//	//m_DefaultData = TimestampedData( defaultData, m_Engine.getElapsedSeconds() );
	//	receiveData( TimestampedData( defaultData, m_Engine.getElapsedTime() ) );
	//}

	void InletBuffer::setTrigger( AbstractCallback< TimestampedData const& > &callback )
	{
		m_TriggeringEvent.addListener( callback );
	}

	void InletBuffer::removeTrigger( AbstractCallback< TimestampedData const& > &callback )
	{
		m_TriggeringEvent.removeListener( callback );
	}

	AnyOptionSet const& InletBuffer::getOptionSet() const
	{
		return m_Options;
	}

}