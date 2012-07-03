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
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealTimer.h"

namespace _2Real
{

	class AbstractTimeBasedTrigger : public AbstractUpdateTrigger
	{

	public:

		AbstractTimeBasedTrigger() : AbstractUpdateTrigger( false ) {}

		virtual ~AbstractTimeBasedTrigger() {}
		virtual void tryTriggerUpdate( long &time ) = 0;

	};

	template< typename Condition >
	class TimeBasedTrigger : public AbstractTimeBasedTrigger
	{

	public:

		TimeBasedTrigger( AbstractStateManager &mgr, const long timeslice ) :
			AbstractTimeBasedTrigger(),
			m_UpdateManager( mgr ),
			m_DesiredTime( timeslice ),
			m_ElapsedTime( 0 )
		{
			AbstractCallback< long > *cb = new MemberCallback< TimeBasedTrigger< Condition >, long >( *this, &TimeBasedTrigger< Condition >::tryTriggerUpdate );
			EngineImpl::instance().getTimer().registerToTimerSignal( *cb );
			m_UpdateManager.addTrigger( *this );
		}

		~TimeBasedTrigger()
		{
			AbstractCallback< long > *cb = new MemberCallback< TimeBasedTrigger< Condition >, long >( *this, &TimeBasedTrigger< Condition >::tryTriggerUpdate );
			EngineImpl::instance().getTimer().unregisterFromTimerSignal( *cb );
			m_UpdateManager.removeTrigger( *this );
		}

		void tryTriggerUpdate( long &time )
		{
			m_ElapsedTime += time;
			if ( !isOk() && m_TriggerCondition( m_ElapsedTime, m_DesiredTime ) )
			{
				m_ElapsedTime = 0;
				m_IsOk = true;
				m_UpdateManager.tryTrigger( *this );
			}
		}

	private:

		AbstractStateManager	&m_UpdateManager;
		long					const m_DesiredTime;
		long					m_ElapsedTime;
		Condition				m_TriggerCondition;

	};

}