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
			// anyway, remove oldest elem from buffer
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

	InletBuffer::InletBuffer( Any const& defaultData, AnyOptionSet const& options ) :
		m_InsertionPolicy( new RemoveOldest( 1 ) ),
		m_Notify( false ),
		m_DefaultData( defaultData, 0 ),
		m_Engine( EngineImpl::instance() ),
		m_TriggeringData( defaultData, 0 ),
		m_Options( options ),
		m_Counter( 0 )
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
		TimestampedData rec;
		const Type tSrc = data.getData().getType();
		const Type tDst = m_DefaultData.getData().getType();
		if ( !( tSrc == tDst ) )
		{
			const TypeCategory cSrc = data.getData().getTypeCategory();
			const TypeCategory cDst = m_DefaultData.getData().getTypeCategory();
			if ( cSrc == TypeCategory::ARITHMETHIC && cDst == TypeCategory::ARITHMETHIC )
			{
				Any converted = arithmethicConversion( data.getData(), tDst );
				rec = TimestampedData( converted, data.getTimestamp(), ++m_Counter );
			}
		}
		else
		{
			rec = TimestampedData( data.getData(), data.getTimestamp(), ++m_Counter );
		}

		if ( !m_Options.isEmpty() )
		{
			Any const& value = rec.getData();
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
			m_TriggeringEvent.notify( rec );
			m_NotificationAccess.unlock();
		}
		else
		{
			m_NotificationAccess.unlock();
			// once the cond was fulfilled, buffering starts
			//Poco::ScopedLock< Poco::FastMutex > pLock( m_PolicyAccess );
			Poco::ScopedLock< Poco::FastMutex > dLock( m_DataAccess );
			m_InsertionPolicy->insertData( rec, m_ReceivedDataItems );
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
		//Poco::ScopedLock< Poco::FastMutex > lock( m_PolicyAccess );
		//delete m_InsertionPolicy;
		//m_InsertionPolicy = new RemoveOldest( size );
		//m_BufferSize = size;
		m_InsertionPolicy->setMaxSize( size );
	}

	unsigned int InletBuffer::getBufferSize() const
	{
		//Poco::ScopedLock< Poco::FastMutex > lock( m_PolicyAccess );
		//return m_BufferSize;
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