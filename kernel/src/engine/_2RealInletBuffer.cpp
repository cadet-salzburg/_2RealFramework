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

	Any arithmethicConversion( Any const& src, const Type tDst )
	{
		const Type::Code cSrc = src.getType().getCode();
		const Type::Code cDst = tDst.getCode();
		switch ( cSrc )
		{
		case Type::BYTE:
		{
			char val = src.extract< char >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::UBYTE:
		{
			unsigned char val = src.extract< unsigned char >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::SHORT:
		{
			short val = src.extract< short >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::USHORT:
		{
			unsigned short val = src.extract< unsigned short >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::INT:
		{
			int val = src.extract< int >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::UINT:
		{
			unsigned int val = src.extract< unsigned int >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::LONG:
		{
			long val = src.extract< long >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::ULONG:
		{
			unsigned long val = src.extract< unsigned long >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::FLOAT:
		{
			float val = src.extract< float >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::DOUBLE:
		{
			double val = src.extract< double >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		case Type::NUMBER:
		{
			Number val = src.extract< Number >();
			switch ( cDst )
			{
			case Type::BYTE:		return Any( static_cast< char >( val ) );
			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
			case Type::SHORT:		return Any( static_cast< short >( val ) );
			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
			case Type::INT:			return Any( static_cast< int >( val ) );
			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
			case Type::LONG:		return Any( static_cast< long >( val ) );
			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
			case Type::FLOAT:		return Any( static_cast< float >( val ) );
			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
			default:
#ifdef _DEBUG
		assert( NULL );
#endif
			return Any();
			}
		}
		default:
#ifdef _DEBUG
			assert( NULL );
#endif
		return Any();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RemoveOldest::RemoveOldest( const unsigned int max ) :
		m_Max( max )
	{
	}

	bool RemoveOldest::insertData( TimestampedData const& data, InletBuffer::DataBuffer &buffer )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );

		while ( buffer.size() >= m_Max && !buffer.empty() )
		{
			// TODO: some sort of overflow cb for the app i guess
			buffer.pop_front();
		}

		buffer.push_back( data );
		return true;
	}

	void RemoveOldest::setMaxSize( const unsigned int max )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );
		m_Max = max;
	}

	unsigned int RemoveOldest::getMaxSize() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );
		return m_Max;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	InletBuffer::InletBuffer( Any const& defaultData, AnyOptionSet const& options ) :
		m_InsertionPolicy( new RemoveOldest( 1 ) ),
		m_NotifyOnReceive( false ),								// initially, no notifications are allowed
		m_Engine( EngineImpl::instance() ),
		m_Options( options ),
		m_Counter( 0 )
	{
		m_DefaultData = TimestampedData( defaultData, 0, ++m_Counter );
		m_TriggeringData = TimestampedData( defaultData, 0, m_DefaultData.getKey() );
	}

	InletBuffer::~InletBuffer()
	{
		delete m_InsertionPolicy;
	}

	void InletBuffer::setDefaultValue( Any const& defaultValue )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DefaultAccess );
		m_DefaultData = TimestampedData( defaultValue, m_Engine.getElapsedTime(), ++m_Counter );
	}

	void InletBuffer::receiveData( Any const& data )
	{
		receiveData( TimestampedData( data, m_Engine.getElapsedTime() ) );
	}

	void InletBuffer::receiveData( std::string const& value )
	{
		Any data;

		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_DefaultAccess );
			data.createNew( m_DefaultData.getData() );
		}

		stringstream s;
		s << value;
		data.readFrom( s );

		receiveData( TimestampedData( data, m_Engine.getElapsedTime() ) );
	}

	void InletBuffer::receiveData( TimestampedData const& data )
	{
		// perform conversion, if necessary //////////////////////////////////////////////////////
		m_DefaultAccess.lock();
		const Type tDst= data.getData().getType();
		const TypeCategory cDst = m_DefaultData.getData().getTypeCategory();
		m_DefaultAccess.unlock();

		TimestampedData received;
		const Type tSrc = data.getData().getType();
		if ( !( tSrc == tDst ) )
		{
			const TypeCategory cSrc = data.getData().getTypeCategory();
			if ( cSrc == TypeCategory::ARITHMETHIC && cDst == TypeCategory::ARITHMETHIC )
			{
				Any converted = arithmethicConversion( data.getData(), tDst );
				received = TimestampedData( converted, data.getTimestamp(), ++m_Counter );
			}
		}
		else received = TimestampedData( data.getData(), data.getTimestamp(), ++m_Counter );
		/////////////////////////////////////////////////////////////////////////////////////////

		// perform option check, if necessary ///////////////////////////////////////////////////
		if ( !m_Options.isEmpty() )
		{
			Any const& value = received.getData();
			if ( !m_Options.isOption( value ) )
			{
				// TODO: callback to app
				return;
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////////

		// m_Notify -> true: processBufferedData was called, meaning an update cycle was finished OR start was called
		// otherwise: move data into buffer
		m_NotificationAccess.lock();
		if ( m_NotifyOnReceive )		m_TriggeringEvent.notify( received );
		else
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_BufferAccess );
			m_InsertionPolicy->insertData( received, m_ReceivedDataItems );
		}
		m_NotificationAccess.unlock();
	}

	TimestampedData const& InletBuffer::getTriggeringData() const
	{
		return m_TriggeringData;
	}

	// called right before block setup, will clear all received data
	// since m_Notify should be false, all data that is received afterwards will be buffered
	// meaning the triggering data will stay on default
	void InletBuffer::clearBufferedData()
	{
		m_BufferAccess.lock();
		m_ReceivedDataItems.clear();
		m_BufferAccess.unlock();

		Poco::ScopedLock< Poco::FastMutex > lock( m_DefaultAccess );
		m_TriggeringData = TimestampedData( m_DefaultData.getData(), m_DefaultData.getTimestamp(), m_DefaultData.getKey() );
	}

	// called right after block update; re-enables triggering, then tries to fulfill the trigger cond with buffered data / last data / default data
	// receiving data from other sources ( user input, outlets ) is blocked during this time
	void InletBuffer::processBufferedData( const bool enableTriggering )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_NotificationAccess );
		m_NotifyOnReceive = true;

		m_BufferAccess.lock();
		for ( DataBufferIterator dIt = m_ReceivedDataItems.begin(); dIt != m_ReceivedDataItems.end(); /**/ )
		{
			TimestampedData d = *dIt;
			m_TriggeringEvent.notify( d );

			if ( !m_NotifyOnReceive )
			{
				dIt = m_ReceivedDataItems.erase( dIt );
				break;
			}
			else
			{
				dIt = m_ReceivedDataItems.erase( dIt );
			}
		}
		m_BufferAccess.unlock();

		m_TriggeringEvent.notify( m_TriggeringData );

		Poco::ScopedLock< Poco::FastMutex > dLock( m_DefaultAccess );
		m_TriggeringEvent.notify( m_DefaultData );

		// should be false only in singlestep
		if ( !enableTriggering ) m_NotifyOnReceive = false;
	}

	void InletBuffer::disableTriggering( TimestampedData const& data )
	{
		m_NotifyOnReceive = false;
		m_TriggeringData = data;
	}

	void InletBuffer::setBufferSize( const unsigned int size )
	{
		m_InsertionPolicy->setMaxSize( size );
	}

	unsigned int InletBuffer::getBufferSize() const
	{
		return m_InsertionPolicy->getMaxSize();
	}

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