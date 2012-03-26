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

#include "_2RealUpdateTriggers.h"
#include "_2RealSingletonHolder.h"
#include "_2RealUpdateTriggersImpl.h"

#include <map>

namespace _2Real
{

	UpdateTriggers& UpdateTriggers::ServiceDefault()
	{
		SingletonHolder< UpdateTriggers > holder;
		holder.instance().clear();
		holder.instance().triggerWhenAllDataAvailable();
		return holder.instance();
	}

	UpdateTriggers& UpdateTriggers::GraphDefault()
	{
		SingletonHolder< UpdateTriggers > holder;
		holder.instance().clear();
		holder.instance().triggerWhenAllChildrenReady();
		return holder.instance();
	}

	UpdateTriggers& UpdateTriggers::EmptyTriggers()
	{
		SingletonHolder< UpdateTriggers > holder;
		holder.instance().clear();
		return holder.instance();
	}

	UpdateTriggers::UpdateTriggers() :
		m_Impl(new UpdateTriggersImpl())
	{
	}

	UpdateTriggers::~UpdateTriggers()
	{
		delete m_Impl;
	}

	void UpdateTriggers::clear()
	{
		m_Impl->clear();

		m_Impl->setChildDefault(new ChildDefault< 2 >());
		m_Impl->setInletDefault(new InletDefault< ValidTimestamp >());
	}

	void UpdateTriggers::triggerByUpdateRate(const float updatesPerSecond)
	{
		//timeslice is in microseconds, since that's the resolution of a poco::timestamp
		//however, the maximum timer update rate obtainable is ~ 2 milliseconds,
		//and that's with a totally empty system -> silly update rates greater than 60 hertz won't be met, probably
		double micros = 1000000/updatesPerSecond;
		m_Impl->addTimeBasedTrigger(static_cast< long >(micros));
	}

	void UpdateTriggers::triggerWhenAllDataNew()
	{
		m_Impl->setInletDefault(new InletDefault< NewerTimestamp >());
	}

	void UpdateTriggers::triggerWhenAllDataAvailable()
	{
		m_Impl->setInletDefault(new InletDefault< ValidTimestamp >());
	}

	void UpdateTriggers::triggerWhenAllChildrenReady()
	{
		m_Impl->setChildDefault(new ChildDefault< 2 >());
	}

}