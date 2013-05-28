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
//
//	Any arithmethicConversion( Any const& src, const Type tDst )
//	{
//		const Type::Code cSrc = src.getType().getCode();
//		const Type::Code cDst = tDst.getCode();
//		switch ( cSrc )
//		{
//		case Type::BYTE:
//		{
//			char val = src.extract< char >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::UBYTE:
//		{
//			unsigned char val = src.extract< unsigned char >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::SHORT:
//		{
//			short val = src.extract< short >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::USHORT:
//		{
//			unsigned short val = src.extract< unsigned short >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::INT:
//		{
//			int val = src.extract< int >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::UINT:
//		{
//			unsigned int val = src.extract< unsigned int >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::LONG:
//		{
//			long val = src.extract< long >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::ULONG:
//		{
//			unsigned long val = src.extract< unsigned long >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::FLOAT:
//		{
//			float val = src.extract< float >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::DOUBLE:
//		{
//			double val = src.extract< double >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		case Type::NUMBER:
//		{
//			Number val = src.extract< Number >();
//			switch ( cDst )
//			{
//			case Type::BYTE:		return Any( static_cast< char >( val ) );
//			case Type::UBYTE:		return Any( static_cast< unsigned char >( val ) );
//			case Type::SHORT:		return Any( static_cast< short >( val ) );
//			case Type::USHORT:		return Any( static_cast< unsigned short >( val ) );
//			case Type::INT:			return Any( static_cast< int >( val ) );
//			case Type::UINT:		return Any( static_cast< unsigned int >( val ) );
//			case Type::LONG:		return Any( static_cast< long >( val ) );
//			case Type::ULONG:		return Any( static_cast< unsigned long >( val ) );
//			case Type::FLOAT:		return Any( static_cast< float >( val ) );
//			case Type::DOUBLE:		return Any( static_cast< double >( val ) );
//			case Type::NUMBER:		return Any( static_cast< Number >( val ) );
//			default:
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//			return Any();
//			}
//		}
//		default:
//#ifdef _DEBUG
//			assert( NULL );
//#endif
//		return Any();
//		}
//	}
//
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

	AbstractInletBuffer::AbstractInletBuffer() :
		m_Engine( EngineImpl::instance() )
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BasicInletBuffer::BasicInletBuffer() :
		AbstractInletBuffer(),
		m_InsertionPolicy( new RemoveOldest( 1 ) ),
		m_NotifyOnReceive( true )
	{
	}

	BasicInletBuffer::~BasicInletBuffer()
	{
		delete m_InsertionPolicy;
	}

	// can ( from buffers' POV ) be called at any time
	void BasicInletBuffer::receiveData( TimestampedData const& data )
	{
		m_NotificationAccess.lock();
		// m_NotifyOnReceive: processBufferedData was called, meaning an update cycle was finished OR start was called
		if ( m_NotifyOnReceive )		m_TriggeringEvent.notify( data );
		// otherwise: move data into buffer
		else
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_BufferAccess );
			m_InsertionPolicy->insertData( data, m_ReceivedDataItems );
		}
		m_NotificationAccess.unlock();
	}

	// no mutexes, because this is for sure called after processBufferedData ( see below )
	std::shared_ptr< const CustomType > BasicInletBuffer::getTriggeringData() const
	{
		return m_TriggeringData;
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

			// calls the trigger -> this may change the m_NotifyOnReceive value via BasicInletBuffer::disableTriggering()
			m_TriggeringEvent.notify( d );

			// if this is no longer true, a value that fulfills the trigger cond has been found & written into m_TriggeringData
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

		// ARGH in case everything else failed, try triggering with the last available data?
		// this is quite weird, should basically only fulfill 'any' condition -> timestamp 0
		m_TriggeringEvent.notify( TimestampedData( m_TriggeringData, 0 ) );

		// the 'enableTriggering' value should be false only in singlestep
		if ( !enableTriggering ) m_NotifyOnReceive = false;
	}

	// only called from within processBufferedData, no locking mutexes b/c everything should already be locked ( see above )
	void BasicInletBuffer::disableTriggering( std::shared_ptr< const CustomType > const& data )
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

	MultiInletBuffer::MultiInletBuffer( std::shared_ptr< const CustomType > initialValue ) : m_InitialValue( initialValue )
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
		// ARGH timestamp & key??
		// adds the initial value to the inlet
		buffer.receiveData( TimestampedData( m_InitialValue, 0 ) );
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