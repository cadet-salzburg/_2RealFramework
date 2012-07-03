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
#include "engine/_2RealInlet.h"
#include "engine/_2RealAbstractStateManager.h"

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

		AbstractInletBasedTrigger( Inlet &inlet, AbstractStateManager &mgr ) :
			AbstractUpdateTrigger( false ),
			m_Inlet( inlet ),
			m_UpdateManager( mgr ),
			m_LastData( EngineData(), -1 )	// this way, default value will also fulfill 'newer' cond
		{
		}

		AbstractInletBasedTrigger& operator=( AbstractInletBasedTrigger const& src )
		{
			m_LastData = src.m_LastData;
			return *this;
		}

		virtual ~AbstractInletBasedTrigger()
		{
			m_UpdateManager.removeTrigger( *this );
			m_Inlet.unregisterUpdateTrigger( *this );
		}

		virtual bool isSingleWeight() const = 0;

		virtual void tryTriggerUpdate( TimestampedData const& data ) = 0;

		bool operator==( AbstractInletBasedTrigger const& other ) const
		{
			return ( &m_Inlet == &other.m_Inlet );
		}

	protected:

		Inlet					&m_Inlet;
		AbstractStateManager	&m_UpdateManager;
		TimestampedData			m_LastData;

	};

	template< typename Condition, bool SingleWeight >
	class InletBasedTrigger : public AbstractInletBasedTrigger
	{

	public:

		InletBasedTrigger( Inlet &inlet, AbstractStateManager &mgr ) :
			AbstractInletBasedTrigger( inlet, mgr ),
			m_IsSingleWeight( SingleWeight )
		{
			mgr.addTrigger( *this );
			inlet.registerUpdateTrigger( *this );
		}

		void tryTriggerUpdate( TimestampedData const& data )
		{
			if ( !m_IsOk && m_TriggerCondition( data, m_LastData ) )
			{
				m_IsOk = true;
				m_Inlet.disableTriggering( data );
				m_LastData = data;
				m_UpdateManager.tryTriggerInlet( *this );
			}
		}

		bool isSingleWeight() const
		{
			return m_IsSingleWeight;
		}

	private:

		bool					m_IsSingleWeight;
		Condition				m_TriggerCondition;

	};

}