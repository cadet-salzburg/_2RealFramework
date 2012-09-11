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

#include "engine/_2RealAbstractUpdateTrigger.h"
#include "engine/_2RealTimestampedData.h"
#include "engine/_2RealAbstractStateManager.h"
#include "engine/_2RealInletBuffer.h"

namespace _2Real
{
	struct ValidData
	{
		bool operator()( TimestampedData const& newData, TimestampedData const& oldData )
		{
			return !newData.anyValue.isNull();
		}
	};

	struct NewerTimestamp
	{
		bool operator()( TimestampedData const& newData, TimestampedData const& oldData )
		{
			return ( newData.timestamp > oldData.timestamp );
		}
	};

	class AbstractInletBasedTrigger
	{

	public:

		AbstractInletBasedTrigger( BasicInletBuffer &buffer, AbstractStateManager &mgr ) :
			m_Condition( false ),
			m_Buffer( buffer ),
			m_UpdateManager( mgr ),
			m_LastData( Any(), 0 )			// default data has timestamp 0 and thus will not fulfill the newer cond
		{
		}

		AbstractInletBasedTrigger& operator=( AbstractInletBasedTrigger const& src )
		{
			m_LastData = src.m_LastData;
			return *this;
		}

		bool operator==( AbstractInletBasedTrigger const& other ) const
		{
			return ( &m_Buffer == &other.m_Buffer );
		}

		virtual ~AbstractInletBasedTrigger() {}

		virtual void tryTriggerUpdate( TimestampedData const& data ) = 0;

		void resetData()
		{
			m_LastData = TimestampedData( Any(), 0 );
		}

		bool isFulfilled() const { return m_Condition.isFulfilled(); }
		void set( const bool fulfilled ) { m_Condition.set( fulfilled ); }

	protected:

		UpdateCondition			m_Condition;
		BasicInletBuffer		&m_Buffer;
		AbstractStateManager	&m_UpdateManager;
		TimestampedData			m_LastData;

	};

	template< typename TCond >
	class InletBasedTrigger : public AbstractInletBasedTrigger
	{

	public:

		InletBasedTrigger( BasicInletBuffer &buffer, AbstractStateManager &mgr, const bool isOr ) :
			AbstractInletBasedTrigger( buffer, mgr ),
			m_IsOr( isOr )
		{
			mgr.addTrigger( *this, isOr );
			AbstractCallback< TimestampedData const& > *cb =
				new MemberCallback< InletBasedTrigger< TCond >, TimestampedData const& >( *this, &InletBasedTrigger< TCond >::tryTriggerUpdate );
			buffer.setTrigger( *cb );
		}

		~InletBasedTrigger()
		{
			m_UpdateManager.removeTrigger( *this, m_IsOr );
			AbstractCallback< TimestampedData const& > *cb =
				new MemberCallback< InletBasedTrigger< TCond >, TimestampedData const& >( *this, &InletBasedTrigger< TCond >::tryTriggerUpdate );
			m_Buffer.removeTrigger( *cb );

			// this has to be done, otherwise removing a basic inlet from a multiinlet might lead to a deadlock
			// if the block owning the inlet was shutdown already, nothing should happen
			m_UpdateManager.tryTriggerInlet( *this );
		}

		void tryTriggerUpdate( TimestampedData const& data )
		{
			if ( !m_Condition.isFulfilled() && m_ConditionCheck( data, m_LastData ) )
			{
				m_Condition.set( true );
				m_Buffer.disableTriggering( data );
				m_LastData = data;
				m_UpdateManager.tryTriggerInlet( *this );
			}
		}

	private:

		bool				m_IsOr;
		TCond				m_ConditionCheck;

	};

}