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

#include "_2RealAbstractUpdateTrigger.h"
#include "_2RealInlet.h"
#include "_2RealAbstractStateManager.h"

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

		virtual void tryTriggerUpdate( TimestampedData const& data ) = 0;

	protected:

		Inlet					&m_Inlet;
		AbstractStateManager	&m_UpdateManager;
		TimestampedData			m_LastData;

	};

	template< typename Condition >
	class InletBasedTrigger : public AbstractInletBasedTrigger
	{

	public:

		InletBasedTrigger( Inlet &inlet, AbstractStateManager &mgr ) :
			AbstractInletBasedTrigger( inlet, mgr )
		{
			mgr.addTrigger( *this );
			inlet.registerUpdateTrigger( *this );
		}

		void tryTriggerUpdate( TimestampedData const& data )
		{
			if ( !m_IsOk && m_TriggerCondition( data, m_LastData ) )
			{
				//std::cout << m_Inlet.getName() << " succeded in triggering" << std::endl;

				m_IsOk = true;
				m_Inlet.disableTriggering( data );
				//std::cout << m_Inlet.getName() << " disabled trigger attempts" << std::endl;
				m_LastData = data;
				m_UpdateManager.tryTrigger( *this );
			}
		}

	private:

		Condition				m_TriggerCondition;

	};

}