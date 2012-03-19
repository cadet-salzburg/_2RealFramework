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

#include "_2RealUpdateTriggersImpl.h"

#include <map>

namespace _2Real
{

	UpdateTriggersImpl::UpdateTriggersImpl() :
		m_Time(LONG_MAX),
		m_HasTime(false),
		m_InletDefault(new InletDefault< ValidTimestamp >()),
		m_ChildDefault(new ChildDefault< 2 >())
	{
	}

	UpdateTriggersImpl::~UpdateTriggersImpl()
	{
		clear();
	}

	void UpdateTriggersImpl::clear()
	{
		for (std::map< std::string, AbstractInletDefault * >::iterator it = m_Inlets.begin(); it != m_Inlets.end(); /**/)
		{
			delete it->second;
			it = m_Inlets.erase(it);
		}

		for (std::map< std::string, AbstractChildDefault * >::iterator it = m_Children.begin(); it != m_Children.end(); /**/)
		{
			delete it->second;
			it = m_Children.erase(it);
		}

		delete m_InletDefault;
		m_InletDefault = NULL;

		delete m_ChildDefault;
		m_ChildDefault = NULL;
	}

	void UpdateTriggersImpl::setInletDefault(AbstractInletDefault *inletDefault)
	{
		if (m_InletDefault != NULL)
		{
			delete m_InletDefault;
		}

		m_InletDefault = inletDefault;
	}

	void UpdateTriggersImpl::setChildDefault(AbstractChildDefault *childDefault)
	{
		if (m_ChildDefault != NULL)
		{
			delete m_ChildDefault;
		}

		m_ChildDefault = childDefault;
	}

	void UpdateTriggersImpl::addTimeBasedTrigger(const long time)
	{
		m_HasTime = true;
		m_Time = time;
	}

	void UpdateTriggersImpl::addInletBasedTrigger(std::string const& inletName, AbstractInletDefault *trigger)
	{
		std::map< std::string, AbstractInletDefault * >::iterator it = m_Inlets.find(inletName);
		if (it != m_Inlets.end())
		{
			delete it->second;
			m_Inlets.erase(it);
		}

		m_Inlets.insert(std::make_pair< std::string, AbstractInletDefault * >(inletName, trigger));
	}

	void UpdateTriggersImpl::addChildBasedTrigger(std::string const& childName, AbstractChildDefault *trigger)
	{
		std::map< std::string, AbstractChildDefault * >::iterator it = m_Children.find(childName);
		if (it != m_Children.end())
		{
			delete it->second;
			m_Children.erase(it);
		}

		m_Children.insert(std::make_pair< std::string, AbstractChildDefault * >(childName, trigger));
	}

	AbstractTimeBasedTrigger * UpdateTriggersImpl::getTimeBasedTrigger()
	{
		if (m_HasTime)
		{
			return new TimeBasedTrigger< std::greater_equal< long > >(m_Time);
		}
		else
		{
			return NULL;
		}
	}

	AbstractInletBasedTrigger * UpdateTriggersImpl::getInletBasedTrigger(std::string const& name)
	{
		std::map< std::string, AbstractInletDefault * >::iterator it = m_Inlets.find(name);
		if (it != m_Inlets.end())
		{
			return it->second->createTrigger(name);
		}
		else
		{
			return m_InletDefault->createTrigger(name);
		}
	}

	AbstractChildBasedTrigger * UpdateTriggersImpl::getChildBasedTrigger(std::string const& name)
	{
		std::map< std::string, AbstractChildDefault * >::iterator it = m_Children.find(name);
		if (it != m_Children.end())
		{
			return it->second->createTrigger(name);
		}
		else
		{
			return m_ChildDefault->createTrigger(name);
		}
	}

}