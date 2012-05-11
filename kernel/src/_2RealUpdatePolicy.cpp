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

#include "_2RealUpdatePolicy.h"
#include "_2RealSingletonHolder.h"
#include "_2RealUpdatePolicyImpl.h"

#include <map>

namespace _2Real
{

	UpdatePolicy::UpdatePolicy() :
		m_Impl(new UpdatePolicyImpl())
	{
	}

	UpdatePolicy::~UpdatePolicy()
	{
		delete m_Impl;
	}

	void UpdatePolicy::clear()
	{
		m_Impl->clear();
		m_Impl->setInletDefault(new InletDefault< ValidTimestamp >());
		m_Impl->setBlockDefault(new BlockDefault< BLOCK_OK >());
	}

	void UpdatePolicy::triggerByUpdateRate(const float updatesPerSecond)
	{
		//timeslice is in microseconds, since that's the resolution of a poco::timestamp
		//however, the maximum timer update rate obtainable is ~ 2 milliseconds,
		//and that's with a totally empty system -> silly update rates greater than 60 hertz won't be met, probably
		double micros = 1000000/updatesPerSecond;
		m_Impl->addTimeBasedTrigger(static_cast< long >(micros));
	}

	void UpdatePolicy::triggerWhenAllDataNew()
	{
		m_Impl->setInletDefault(new InletDefault< NewerTimestamp >());
	}

	void UpdatePolicy::triggerWhenAllDataAvailable()
	{
		m_Impl->setInletDefault(new InletDefault< ValidTimestamp >());
	}

}