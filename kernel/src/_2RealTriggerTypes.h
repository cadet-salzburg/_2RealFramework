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

#include <string>
#include <map>
#include <iostream>

namespace _2Real
{

	class Trigger
	{

	public:

		Trigger() : m_IsFulfilled(false) {}
		virtual ~Trigger() {}
		bool ok() { return m_IsFulfilled; }
		virtual void set() { m_IsFulfilled = true; }
		virtual void reset() { m_IsFulfilled = false; }

	private:

		bool		m_IsFulfilled;

	};

	class AbstractTimeBasedTrigger : public Trigger
	{

	public:

		AbstractTimeBasedTrigger(const long timeslice) : m_ElapsedTime(0), m_DesiredTime(timeslice) {}
		virtual ~AbstractTimeBasedTrigger() {}

		virtual const bool tryTrigger(const long time) = 0;

	protected:

		long		const m_DesiredTime;
		long		m_ElapsedTime;

	};

	struct AlwaysTrue
	{
		bool operator()( const long t1, const long t2 ) { return true; }
	};

	struct AlwaysFalse
	{
		bool operator()( const long t1, const long t2 ) { return false; }
	};

	template< typename Condition >
	class TimeBasedTrigger : public AbstractTimeBasedTrigger
	{

	public:

		TimeBasedTrigger(const long timeslice) : AbstractTimeBasedTrigger(timeslice) {}
		const bool tryTrigger(const long time)
		{
			m_ElapsedTime += time;
			if ( m_TriggerCondition(m_ElapsedTime, m_DesiredTime) && !ok() )
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

	struct ValidTimestamp
	{
		bool operator()(const long oldData, const long newData) { return newData >= 0; }
	};

	typedef std::less< long > NewerTimestamp;

	class AbstractInletBasedTrigger : public Trigger
	{

	public:

		AbstractInletBasedTrigger(std::string const& inletName) : m_InletName(inletName) {}
		virtual ~AbstractInletBasedTrigger() {}
		std::string const& getInletName() { return m_InletName; }

		virtual const bool tryTrigger(const long lastUpdate, const long newData) = 0;

	protected:

		std::string		const m_InletName;

	};

	template< typename Condition >
	class InletBasedTrigger : public AbstractInletBasedTrigger
	{

	public:

		InletBasedTrigger(std::string const& inletName) : AbstractInletBasedTrigger(inletName) {}
		const bool tryTrigger(const long lastUpdate, const long newData)
		{
			if ( m_TriggerCondition(lastUpdate, newData) && !ok() )
			{
				set();
				return true;
			}
			return false;
		}

	private:

		Condition			m_TriggerCondition;

	};

	enum BlockMessage
	{
		BLOCK_OK						=	0x00,
		BLOCK_READY						=	0x01,
		BLOCK_FINISHED					=	0x02,
		BLOCK_NOT_OK					=	0x04,
	};

	class AbstractBlockBasedTrigger : public Trigger
	{

	public:

		AbstractBlockBasedTrigger(std::string const& blockName) : m_BlockName(blockName) {}
		virtual ~AbstractBlockBasedTrigger() {}
		std::string const& getBlockName() { return m_BlockName; }

		virtual const bool tryTrigger(const BlockMessage state) = 0;

	protected:

		std::string		const m_BlockName;

	};

	template< BlockMessage TriggerState >
	class BlockBasedTrigger : public AbstractBlockBasedTrigger
	{

	public:

		BlockBasedTrigger(std::string const& blockName) : AbstractBlockBasedTrigger(blockName) {}
		const bool tryTrigger(const BlockMessage state)
		{
			if ( state == TriggerState && !ok() )
			{
				set();
				return true;
			}
			return false;
		}

	};

	class AbstractStateManager;

	typedef std::map< std::string, AbstractInletBasedTrigger * >	InletTriggerMap;
	typedef std::map< std::string, std::pair< AbstractBlockBasedTrigger *, AbstractStateManager * > >	BlockTriggerMap;

}