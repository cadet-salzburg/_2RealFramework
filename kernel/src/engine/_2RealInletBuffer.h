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
#include "helpers/_2RealHelpersInternal.h"
#include "app/_2RealCallbacksInternal.h"
#include "helpers/_2RealPoco.h"

#include <list>
#include <memory>

namespace _2Real
{

	class AbstractInsertionPolicy
	{

	public:

		virtual bool insertData( TimestampedData const& data, std::list< TimestampedData > &buffer ) = 0;

	};

	class RemoveOldest : public AbstractInsertionPolicy
	{

	public:

		RemoveOldest( const unsigned int max );
		bool insertData( TimestampedData const& data, std::list< TimestampedData > &buffer );

	private:

		unsigned int	const m_Max;

	};

	class EngineImpl;

	class InletBuffer
	{

	public:

		// within the data buffer, data items are stored by the order which they arrived in;
		// not necessarily in timestamp order
		typedef std::list< TimestampedData >			DataBuffer;
		typedef std::list< TimestampedData >::iterator	DataBufferIterator;

		InletBuffer( EngineData const& defaultData );
		void receiveData( TimestampedData const& data );
		TimestampedData const& getTriggeringData() const;
		void processBufferedData();
		void disableTriggering( TimestampedData const& data );
		void setBufferSize( const unsigned int size );
		void setDefaultData( EngineData const& defaultData );
		void setTrigger( AbstractCallback< TimestampedData const& > &callback );
		void removeTrigger( AbstractCallback< TimestampedData const& > &callback );

	private:

		EngineImpl										&m_Engine;
		DataBuffer										m_ReceivedDataItems;	// holds all received data items
		TimestampedData									m_TriggeringData;		// holds the data item which first triggered the update condition
		CallbackEvent< TimestampedData const& >			m_TriggeringEvent;
		volatile bool									m_Notify;
		TimestampedData									m_DefaultData;
		mutable Poco::FastMutex							m_PolicyAccess;
		mutable Poco::FastMutex							m_DataAccess;
		mutable Poco::FastMutex							m_NotificationAccess;
		std::auto_ptr< AbstractInsertionPolicy >		m_InsertionPolicy;

	};

}