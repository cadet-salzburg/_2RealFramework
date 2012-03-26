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
#include <list>
#include <iostream>

#include "Poco/Mutex.h"
#include "Poco/BasicEvent.h"

namespace _2Real
{

	class UpdateTriggersImpl;
	class RunnableManager;

	class Trigger
	{

	public:

		Trigger() : m_IsFulfilled(false) {}
		virtual ~Trigger() {}
		const bool ok() { return m_IsFulfilled; }
		void set() { m_IsFulfilled = true; }
		void reset() { m_IsFulfilled = false; }

	private:

		bool			m_IsFulfilled;

	};

	///////////////////////////////////////////////////////////////////////////

	class AbstractTimeBasedTrigger : public Trigger
	{

	public:

		AbstractTimeBasedTrigger(const long timeslice);
		virtual ~AbstractTimeBasedTrigger();

		virtual const bool tryTrigger(const long time) = 0;

	protected:

		long		const m_DesiredTime;
		long		m_ElapsedTime;

	};

	inline AbstractTimeBasedTrigger::AbstractTimeBasedTrigger(const long timeslice) :
		Trigger(),
		m_ElapsedTime(0),
		m_DesiredTime(timeslice)
	{
	}

	inline AbstractTimeBasedTrigger::~AbstractTimeBasedTrigger()
	{
	}

	template< typename Condition >
	class TimeBasedTrigger : public AbstractTimeBasedTrigger
	{

	public:

		TimeBasedTrigger(const long timeslice) :
			AbstractTimeBasedTrigger(timeslice)
		{
		}

		const bool tryTrigger(const long time)
		{
			m_ElapsedTime += time;
			if (m_TriggerCondition(m_ElapsedTime, m_DesiredTime))
			{
				m_ElapsedTime = 0;
				set();
				return true;
			}

			return false;
		}

	private:

		Condition			m_TriggerCondition;

	};


	///////////////////////////////////////////////////////////////////////////////////////////

	class ValidTimestamp
	{

	public:

		bool operator()(const long oldData, const long newData)
		{
			return newData >= 0;
		}

	};

	typedef std::less< long > NewerTimestamp;

	class AbstractInletBasedTrigger : public Trigger
	{

	public:

		AbstractInletBasedTrigger(std::string const& inletName);
		virtual ~AbstractInletBasedTrigger();

		const bool operator<(AbstractInletBasedTrigger const& rhs) const;

		virtual const bool tryTrigger(const long lastUpdate, const long newData) = 0;

		std::string const& getInletName() { return m_InletName; }

	protected:

		std::string		const m_InletName;

	};

	inline AbstractInletBasedTrigger::AbstractInletBasedTrigger(std::string const& inletName) :
		Trigger(),
		m_InletName(inletName)
	{
	}

	inline AbstractInletBasedTrigger::~AbstractInletBasedTrigger()
	{
	}

	inline const bool AbstractInletBasedTrigger::operator<(AbstractInletBasedTrigger const& rhs) const
	{
		return m_InletName < rhs.m_InletName;
	}

	template< typename Condition >
	class InletBasedTrigger : public AbstractInletBasedTrigger
	{

	public:

		InletBasedTrigger(std::string const& inletName) :
		AbstractInletBasedTrigger(inletName)
		{
		}

		const bool tryTrigger(const long lastUpdate, const long newData)
		{
			if (m_TriggerCondition(lastUpdate, newData))
			{
				set();
				return true;
			}

			return false;
		}

	private:

		Condition			m_TriggerCondition;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////

	class AbstractChildBasedTrigger :  public Trigger
	{

	public:

		AbstractChildBasedTrigger(std::string const& childName);
		virtual ~AbstractChildBasedTrigger();

		const bool operator<(AbstractChildBasedTrigger const& rhs) const;

		virtual const bool tryTrigger(const unsigned int state) = 0;

		std::string const& getChildName() { return m_ChildName; }

	protected:

		std::string		const m_ChildName;

	};

	inline AbstractChildBasedTrigger::AbstractChildBasedTrigger(std::string const& childName) :
		Trigger(),
		m_ChildName(childName)
	{
	}

	inline AbstractChildBasedTrigger::~AbstractChildBasedTrigger()
	{
	}

	inline const bool AbstractChildBasedTrigger::operator<(AbstractChildBasedTrigger const& rhs) const
	{
		return m_ChildName < rhs.m_ChildName;
	}

	template< unsigned int State >
	class ChildBasedTrigger : public AbstractChildBasedTrigger
	{

	public:

		ChildBasedTrigger(std::string const& childName) :
			AbstractChildBasedTrigger(childName),
			m_TriggerState(State)
		{
		}

		const bool tryTrigger(const unsigned int state)
		{
			if (m_TriggerState == state)
			{
				set();
				return true;
			}
			return false;
		}

	private:

		unsigned int		const m_TriggerState;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////

	class TriggerSet
	{

	public:

		AbstractTimeBasedTrigger								*time;
		std::map< std::string, AbstractInletBasedTrigger * >	inlets;
		std::map< std::string, AbstractChildBasedTrigger * >	children;

		TriggerSet();
		~TriggerSet();
		const bool evaluate() const;
		void reset();
	};

	class RunnableTriggers
	{

	public:

		RunnableTriggers(RunnableManager &mgr);

		void addInletBasedTrigger(AbstractInletBasedTrigger &trigger);
		void addTimeBasedTrigger(AbstractTimeBasedTrigger &trigger);
		void addChildBasedTrigger(AbstractChildBasedTrigger &trigger);

		void tryTriggerTime(long &time);
		void tryTriggerInlet(const void *inlet, std::pair< long, long > &times);
		void tryTriggerChild(const void *child, unsigned int &state);

		void stopTriggers();
		void startTriggers();

	private:

		void evaluateTriggers();
		void resetTriggers();
		const bool enabled() const;

		bool									m_TriggersEnabled;
		RunnableManager							&m_Runnable;
		mutable Poco::FastMutex					m_TriggerMutex;
		mutable Poco::FastMutex					m_EnabledMutex;
		TriggerSet								m_ActiveTriggers;

	};
}