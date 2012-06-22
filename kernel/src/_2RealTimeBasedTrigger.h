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

#include "_2RealUpdateTrigger.h"

namespace _2Real
{

	class AbstractTimeBasedTrigger : private UpdateCondition
	{

	public:

		using UpdateCondition::isFullfilled;
		using UpdateCondition::set;
		using UpdateCondition::reset;

		AbstractTimeBasedTrigger( const long timeslice ) : m_ElapsedTime( 0 ), m_DesiredTime( timeslice ) {}
		virtual ~AbstractTimeBasedTrigger() {}

		virtual bool tryTrigger( const long time ) = 0;

	protected:

		long	const m_DesiredTime;
		long	m_ElapsedTime;

	};

	template< typename Condition >
	class TimeBasedTrigger : public AbstractTimeBasedTrigger
	{

	public:

		TimeBasedTrigger( const long timeslice ) : AbstractTimeBasedTrigger( timeslice ) {}
		bool tryTrigger( const long time )
		{
			m_ElapsedTime += time;
			if ( m_TriggerCondition( m_ElapsedTime, m_DesiredTime ) && !isFullfilled() )
			{
				m_ElapsedTime = 0;
				set();
				return true;
			}
			return false;
		}

	private:

		Condition		m_TriggerCondition;

	};

}