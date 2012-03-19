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

#include "_2RealRunnableTriggers.h"
#include "_2RealRunnableManager.h"
#include "_2RealInputSlot.h"
#include "_2RealUpdateTriggersImpl.h"

#include "Poco/Delegate.h"

namespace _2Real
{

	TriggerSet::TriggerSet() :
		time(NULL),
		children(),
		inlets()
	{
	}

	TriggerSet::~TriggerSet()
	{
		if (time != NULL)
		{
			delete time;
		}

		for (std::map< std::string, AbstractInletBasedTrigger * >::iterator it = inlets.begin(); it != inlets.end(); /*skipped*/)
		{
			delete it->second;
			it = inlets.erase(it);
		}

		for (std::map< std::string, AbstractChildBasedTrigger * >::iterator it = children.begin(); it != children.end(); /*skipped*/)
		{
			delete it->second;
			it = children.erase(it);
		}
	}

	const bool TriggerSet::evaluate() const
	{
		bool fulfilled = true;

		fulfilled &= (time == NULL || time->ok());

		for (std::map< std::string, AbstractInletBasedTrigger * >::const_iterator it = inlets.begin(); it != inlets.end(); ++it)
		{
			fulfilled &= it->second->ok();
		}

		for (std::map< std::string, AbstractChildBasedTrigger * >::const_iterator it = children.begin(); it != children.end(); ++it)
		{
			fulfilled &= it->second->ok();
		}

		return fulfilled;
	}

	void TriggerSet::reset()
	{
		if (time != NULL)
		{
			time->reset();
		}

		for (std::map< std::string, AbstractInletBasedTrigger * >::iterator it = inlets.begin(); it != inlets.end(); ++it)
		{
			it->second->reset();
		}

		for (std::map< std::string, AbstractChildBasedTrigger * >::iterator it = children.begin(); it != children.end(); ++it)
		{
			it->second->reset();
		}
	}

	RunnableTriggers::RunnableTriggers(RunnableManager &runnable) :
		m_Runnable(runnable),
		m_TriggerMutex(),
		m_ActiveTriggers(),
		m_TriggersEnabled(false),
		m_EnabledMutex()
	{
	}

	void RunnableTriggers::startTriggers()
	{
		m_TriggerMutex.lock();
		resetTriggers();
		m_TriggerMutex.unlock();

		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledMutex);
		m_TriggersEnabled = true;
	}

	void RunnableTriggers::stopTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledMutex);
		m_TriggersEnabled = false;
	}

	const bool RunnableTriggers::enabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledMutex);
		return m_TriggersEnabled;
	}

	void RunnableTriggers::evaluateTriggers()
	{
		if (m_ActiveTriggers.evaluate())
		{
			m_Runnable.getReady();
		}
	}

	void RunnableTriggers::resetTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TriggerMutex);

		m_ActiveTriggers.reset();
	}

	void RunnableTriggers::addChildBasedTrigger(AbstractChildBasedTrigger &trigger)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TriggerMutex);

		std::map< std::string, AbstractChildBasedTrigger * >::iterator it = m_ActiveTriggers.children.find(trigger.getChildName());
		if (it != m_ActiveTriggers.children.end())
		{
			delete it->second;
			m_ActiveTriggers.children.erase(it);
		}

		m_ActiveTriggers.children.insert(std::make_pair< std::string, AbstractChildBasedTrigger * >(trigger.getChildName(), &trigger));
	}

	void RunnableTriggers::addInletBasedTrigger(AbstractInletBasedTrigger &trigger)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TriggerMutex);

		std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_ActiveTriggers.inlets.find(trigger.getInletName());
		if (it != m_ActiveTriggers.inlets.end())
		{
			delete it->second;
			m_ActiveTriggers.inlets.erase(it);
		}

		m_ActiveTriggers.inlets.insert(std::make_pair< std::string, AbstractInletBasedTrigger * >(trigger.getInletName(), &trigger));
	}

	void RunnableTriggers::addTimeBasedTrigger(AbstractTimeBasedTrigger &trigger)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TriggerMutex);

		if (m_ActiveTriggers.time!= NULL)
		{
			delete m_ActiveTriggers.time;
		}

		m_ActiveTriggers.time = &trigger;
	}

	void RunnableTriggers::tryTriggerInlet(const void *inlet, std::pair< long, long > &times)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TriggerMutex);

		InputSlot const* in = static_cast< InputSlot const* >(inlet);
		std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_ActiveTriggers.inlets.find(in->getName());
		if (enabled() && it != m_ActiveTriggers.inlets.end())
		{
			if (it->second->tryTrigger(times.first, times.second))
			{
				evaluateTriggers();
			}
			//else
			//{
			//	//std::cout << "trigger failed" << std::endl;
			//}
		}
	}

	void RunnableTriggers::tryTriggerChild(const void *child, unsigned int &state)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TriggerMutex);

		RunnableManager const* mgr = static_cast< RunnableManager const* >(child);
		std::map< std::string, AbstractChildBasedTrigger * >::iterator it = m_ActiveTriggers.children.find(mgr->getManagedName());
		if (enabled() && it != m_ActiveTriggers.children.end())
		{
			if (it->second->tryTrigger(state))
			{
				evaluateTriggers();
			}
		}
	}

	void RunnableTriggers::tryTriggerTime(long &time)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_TriggerMutex);

		if(enabled() && m_ActiveTriggers.time != NULL)
		{
			if (m_ActiveTriggers.time->tryTrigger(time))
			{
				evaluateTriggers();
			}
		}
	}

}