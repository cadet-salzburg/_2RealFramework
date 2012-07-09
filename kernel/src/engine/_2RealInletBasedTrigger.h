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
#include "engine/_2RealAbstractStateManager.h"
#include "engine/_2RealInletBuffer.h"

namespace _2Real
{

	struct ValidData
	{
		bool operator()( TimestampedData const& newData, TimestampedData const& oldData )
		{
			return !newData.getData().isEmpty();
		}
	};

	struct NewerTimestamp
	{
		bool operator()( TimestampedData const& newData, TimestampedData const& oldData )
		{
			return ( newData.getTimestamp() > oldData.getTimestamp() );
		}
	};

	class AbstractInletBasedTrigger : public AbstractUpdateTrigger
	{

	public:

		AbstractInletBasedTrigger( InletBuffer &buffer, AbstractStateManager &mgr ) :
			AbstractUpdateTrigger( false ),
			m_Buffer( buffer ),
			m_UpdateManager( mgr ),
			m_LastData( Any(), 0 )	// this way, default value will not fulfill 'newer' cond
		{
		}

		AbstractInletBasedTrigger& operator=( AbstractInletBasedTrigger const& src )
		{
			// this copies the triggering info, kind of imprtant for policy changes
			m_LastData = src.m_LastData;
			return *this;
		}

		bool operator==( AbstractInletBasedTrigger const& other ) const
		{
			return ( &m_Buffer == &other.m_Buffer );
		}

		virtual ~AbstractInletBasedTrigger() {}
		virtual bool isOr() const = 0;
		virtual bool isSingleStep() const = 0;
		virtual void tryTriggerUpdate( TimestampedData const& data ) = 0;

	protected:

		InletBuffer				&m_Buffer;
		AbstractStateManager	&m_UpdateManager;
		TimestampedData			m_LastData;

	};

	template< typename TCond, bool IsOr, bool IsSingleStep  >
	class InletBasedTrigger : public AbstractInletBasedTrigger
	{

	public:

		InletBasedTrigger( InletBuffer &buffer, AbstractStateManager &mgr ) :
			AbstractInletBasedTrigger( buffer, mgr )
		{
			mgr.addTrigger( *this );
			AbstractCallback< TimestampedData const& > *cb =
				new MemberCallback< InletBasedTrigger< TCond, IsOr, IsSingleStep >, TimestampedData const& >( *this, &InletBasedTrigger< TCond, IsOr, IsSingleStep >::tryTriggerUpdate );
			buffer.setTrigger( *cb );
		}

		~InletBasedTrigger()
		{
			m_UpdateManager.removeTrigger( *this );
			AbstractCallback< TimestampedData const& > *cb =
				new MemberCallback< InletBasedTrigger< TCond, IsOr, IsSingleStep  >, TimestampedData const& >( *this, &InletBasedTrigger< TCond, IsOr, IsSingleStep >::tryTriggerUpdate );
			m_Buffer.removeTrigger( *cb );
		}

		void tryTriggerUpdate( TimestampedData const& data )
		{
			if ( !m_IsOk && m_TriggerCondition( data, m_LastData ) )
			{
				m_IsOk = true;
				m_Buffer.disableTriggering( data );
				m_LastData = data;
				m_UpdateManager.tryTriggerInlet( *this );
			}
		}

		bool isOr() const
		{
			return IsOr;
		}

		bool isSingleStep() const
		{
			return IsSingleStep;
		}

	private:

		TCond			m_TriggerCondition;

	};

}