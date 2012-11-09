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

	bool RemoveOldest::insertData( TimestampedData const& data, AbstractInletBuffer::DataBuffer &buffer )
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

	AbstractInletBuffer::AbstractInletBuffer( AnyOptionSet const& options ) :
		m_Engine( EngineImpl::instance() ),
		m_Options( options )
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BasicInletBuffer::BasicInletBuffer( Any const& initialValue, AnyOptionSet const& options ) :
		AbstractInletBuffer( options ),
		m_InsertionPolicy( new RemoveOldest( 1 ) ),
		m_NotifyOnReceive( true ),
		m_Counter( 0 ),
		m_InitialValue( initialValue )
	{
	}

	BasicInletBuffer::~BasicInletBuffer()
	{
		delete m_InsertionPolicy;
	}

	void BasicInletBuffer::setInitialValue( Any const& initialValue )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_InitialDataAccess );
		m_InitialValue = initialValue;
	}

	void BasicInletBuffer::setInitialValueToString( std::string const& dataAsString )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_InitialDataAccess );

		Any data;
		data.createNew( m_InitialValue );

		stringstream s;
		s << dataAsString;
		data.readFrom( s );

		m_InitialValue = data;
	}

	Any const& BasicInletBuffer::getInitialValue() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_InitialDataAccess );
		return m_InitialValue;
	}

	void BasicInletBuffer::receiveData( Any const& data )
	{
		receiveData( TimestampedData( data, m_Engine.getElapsedTime() ) );
	}

	void BasicInletBuffer::receiveData( std::string const& value )
	{
		Any data;

		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_InitialDataAccess );
			data.createNew( m_InitialValue );
		}

		stringstream s;
		s << value;
		data.readFrom( s );

		receiveData( TimestampedData( data, m_Engine.getElapsedTime() ) );
	}

	void BasicInletBuffer::receiveData( TimestampedData const& data )
	{
		// perform conversion, if necessary //////////////////////////////////////////////////////
		m_InitialDataAccess.lock();
		const Type tDst= m_InitialValue.getType();
		const TypeCategory cDst = m_InitialValue.getTypeCategory();
		m_InitialDataAccess.unlock();

		TimestampedData received;
		const Type tSrc = data.anyValue.getType();
		if ( !( tSrc == tDst ) )
		{
			const TypeCategory cSrc = data.anyValue.getTypeCategory();
			if ( cSrc == TypeCategory::ARITHMETHIC && cDst == TypeCategory::ARITHMETHIC )
			{
				Any converted = arithmethicConversion( data.anyValue, tDst );
				received = TimestampedData( converted, data.timestamp, ++m_Counter );
			}
		}
		else received = TimestampedData( data.anyValue, data.timestamp, ++m_Counter );
		/////////////////////////////////////////////////////////////////////////////////////////

		// perform option check, if necessary ///////////////////////////////////////////////////
		if ( !m_Options.isEmpty() )
		{
			Any const& value = received.anyValue;
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

	TimestampedData const& BasicInletBuffer::getTriggeringData() const
	{
		return m_TriggeringData;
	}

	// called right before block setup, will clear all received data
	// since m_Notify should be false, all data that is received afterwards will be buffered
	// meaning the triggering data will stay on default
	void BasicInletBuffer::clearBufferedData()
	{
		m_BufferAccess.lock();
		m_ReceivedDataItems.clear();
		m_BufferAccess.unlock();

		m_TriggeringData = m_InitialValue;
	}

	// called right after block update; re-enables triggering, then tries to fulfill the trigger cond with buffered data / last data / default data
	// receiving data from other sources ( user input, outlets ) is blocked during this time
	void BasicInletBuffer::processBufferedData( const bool enableTriggering )
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

		// should be false only in singlestep
		if ( !enableTriggering ) m_NotifyOnReceive = false;
	}

	void BasicInletBuffer::disableTriggering( TimestampedData const& data )
	{
		m_NotifyOnReceive = false;
		m_TriggeringData = data;
	}

	void BasicInletBuffer::setBufferSize( const unsigned int size )
	{
		m_InsertionPolicy->setMaxSize( size );
	}

	unsigned int BasicInletBuffer::getBufferSize() const
	{
		return m_InsertionPolicy->getMaxSize();
	}

	void BasicInletBuffer::setTrigger( AbstractCallback< TimestampedData const& > &callback )
	{
		m_TriggeringEvent.addListener( callback );
	}

	void BasicInletBuffer::removeTrigger( AbstractCallback< TimestampedData const& > &callback )
	{
		m_TriggeringEvent.removeListener( callback );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MultiInletBuffer::MultiInletBuffer( Any const& initialData, AnyOptionSet const& options ) :
		AbstractInletBuffer( options )
	{
	}

	MultiInletBuffer::~MultiInletBuffer()
	{
	}

	BasicInletBuffer & MultiInletBuffer::operator[]( const unsigned int index )
	{
		try
		{
			return *m_Buffers.at( index );
		}
		catch ( std::out_of_range &e )
		{
			throw Exception( e.what() );
		}
	}

	void MultiInletBuffer::addBasicBuffer( BasicInletBuffer &buffer )
	{
		m_Buffers.push_back( &buffer );
	}

	void MultiInletBuffer::removeBasicBuffer( BasicInletBuffer &buffer )
	{
		for ( BasicBufferIterator it = m_Buffers.begin(); it != m_Buffers.end(); ++it )
		{
			if ( &buffer == *it )
			{
				m_Buffers.erase( it );
				break;
			}
		}
	}

}