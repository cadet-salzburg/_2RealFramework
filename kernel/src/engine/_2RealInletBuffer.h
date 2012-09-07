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

#include "engine/_2RealTimestampedData.h"
#include "helpers/_2RealEvent.h"
#include "helpers/_2RealPoco.h"
#include "helpers/_2RealOptions.h"

#include <list>

namespace _2Real
{

	template< typename T >
	class AbstractCallback;

	class AbstractInsertionPolicy
	{

	public:

		virtual bool insertData( TimestampedData const& data, std::list< TimestampedData > &buffer ) = 0;
		virtual ~AbstractInsertionPolicy() {};
		virtual void setMaxSize( const unsigned int max ) = 0;
		virtual unsigned int getMaxSize() const = 0;

	};

	class RemoveOldest : public AbstractInsertionPolicy
	{

	public:

		RemoveOldest( const unsigned int max );
		bool insertData( TimestampedData const& data, std::list< TimestampedData > &buffer );
		void setMaxSize( const unsigned int max );
		unsigned int getMaxSize() const;

	private:

		mutable Poco::FastMutex		m_Mutex;
		unsigned int				m_Max;

	};

	class EngineImpl;
	class BasicInletBuffer;

	class AbstractInletBuffer
	{

	public:

		typedef std::list< TimestampedData >			DataBuffer;
		typedef std::list< TimestampedData >::iterator	DataBufferIterator;

		AbstractInletBuffer( AnyOptionSet const& options );
		virtual ~AbstractInletBuffer() {}

		virtual BasicInletBuffer & operator[]( const unsigned int index ) = 0;
		AnyOptionSet const& getOptionSet() const { return m_Options; }

	protected:

		EngineImpl										&m_Engine;
		AnyOptionSet									m_Options;

	};

	class BasicInletBuffer : public AbstractInletBuffer
	{

	public:

		BasicInletBuffer( Any const& initialData, AnyOptionSet const& options );
		~BasicInletBuffer();

		BasicInletBuffer & operator[]( const unsigned int index ) { return *this; }

		void receiveData( Any const& data );
		void receiveData( std::string const& data );
		void processBufferedData( const bool enableTriggering );
		void clearBufferedData();
		AnyOptionSet const& getOptionSet() const;

		void setInitialValue( Any const& initialValue );
		Any const& getInitialValue() const;

		TimestampedData const& getTriggeringData() const;
		void setBufferSize( const unsigned int size );
		unsigned int getBufferSize() const;

		// direct linking is based on basic buffers
		void receiveData( TimestampedData const& data );

		// triggers is based on basic buffers only
		void setTrigger( AbstractCallback< TimestampedData const& > &callback );
		void removeTrigger( AbstractCallback< TimestampedData const& > &callback );
		void disableTriggering( TimestampedData const& data );

	private:

		unsigned long									m_Counter;
		DataBuffer										m_ReceivedDataItems;	// holds all received data items
		TimestampedData									m_TriggeringData;		// holds the data item which first triggered the update condition
		CallbackEvent< TimestampedData const& >			m_TriggeringEvent;
		volatile bool									m_NotifyOnReceive;		// if true: try triggering

		Any												m_InitialValue;
		mutable Poco::FastMutex							m_InitialDataAccess;
		mutable Poco::FastMutex							m_BufferAccess;
		mutable Poco::FastMutex							m_NotificationAccess;
		AbstractInsertionPolicy							*m_InsertionPolicy;

	};

	class MultiInletBuffer : public AbstractInletBuffer
	{

	public:

		typedef std::vector< BasicInletBuffer * >						BasicBuffers;
		typedef std::vector< BasicInletBuffer * >::iterator				BasicBufferIterator;
		typedef std::vector< BasicInletBuffer * >::const_iterator		BasicBufferConstInterator;

		MultiInletBuffer( Any const& initialData, AnyOptionSet const& options );
		~MultiInletBuffer();

		BasicInletBuffer & operator[]( const unsigned int index );
		void addBasicBuffer( BasicInletBuffer &buffer );
		void removeBasicBuffer( BasicInletBuffer &buffer );

	private:

		BasicBuffers				m_Buffers;

	};

}
