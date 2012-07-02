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
#include "helpers/_2RealPoco.h"

#include <list>
#include <memory>

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

	// on overflow, throws out the very first element
	class RemoveFirst : public AbstractInsertionPolicy
	{

	public:

		RemoveFirst( const unsigned int max );
		bool insertData( TimestampedData const& data, DataBuffer &buffer );

	private:

		unsigned int	const m_Max;

	};

	class Inlet;
	class InletHandle;
	class AbstractInletBasedTrigger;

	class InletBuffer
	{

	public:

		InletBuffer( EngineData const& defaultData );
		void receiveData( TimestampedData &data );
		TimestampedData const& getCurrentData() const;
		void updateDataBuffer();
		void disableTriggering( TimestampedData const& data );
		void setInsertionPolicy( AbstractInsertionPolicy &policy );
		void setDefaultData( TimestampedData const& defaultData );
		void unregisterUpdateTrigger( AbstractInletBasedTrigger &trigger );
		void registerUpdateTrigger( AbstractInletBasedTrigger &trigger );

	private:

		typedef std::list< AbstractInletBasedTrigger * > InletTriggerList;

		DataBuffer									m_ReceivedDataItems;	// holds all received data items
		TimestampedData								m_TriggeringData;		// holds the data item which triggered the update condition
		InletTriggerList							m_InletTriggers;
		TimestampedData								m_DefaultData;
		volatile bool								m_Notify;				// this is set to false as soon as any received data
		mutable Poco::FastMutex						m_DataAccess;			// i use this for synchronizing policy, data access & default access oO
		mutable Poco::FastMutex						m_ReceivedAccess;
		mutable Poco::FastMutex						m_NotificationAccess;
		std::auto_ptr< AbstractInsertionPolicy >	m_InsertionPolicy;

	};

}