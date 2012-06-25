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

#include <map>
#include <functional>

namespace _2Real
{

	struct ValidTimestamp
	{
		bool operator()( const long oldData, const long newData ) { return newData >= 0; }
	};

	typedef std::less< long > NewerTimestamp;

	class AbstractInletBasedTrigger : public UpdateTrigger
	{

	public:

		AbstractInletBasedTrigger( std::string const& inletName ) : m_InletName( inletName ), m_Condition( false ) {}
		virtual ~AbstractInletBasedTrigger() {}
		std::string const& getInletName() { return m_InletName; }
		void reset() { m_Condition = false; }
		bool isOk() const { return m_Condition; }

		virtual bool tryTriggerUpdate( const long lastUpdate, const long newData ) = 0;

	protected:

		bool				m_Condition;
		std::string			const m_InletName;

	};

	typedef std::map< std::string, AbstractInletBasedTrigger * >	InletBasedTriggerMap;

	template< typename Condition >
	class InletBasedTrigger : public AbstractInletBasedTrigger
	{

	public:

		InletBasedTrigger( std::string const& inletName ) :
			AbstractInletBasedTrigger( inletName )
		{
		}

		bool tryTriggerUpdate( const long lastUpdate, const long newData )
		{
			if ( !m_Condition && m_TriggerCondition(lastUpdate, newData) )
			{
				m_Condition = true;
			}
			return m_Condition;
		}

	private:

		Condition			m_TriggerCondition;

	};

}