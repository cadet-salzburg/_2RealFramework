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

#include "_2RealRunnable.h"
#include "_2RealGraph.h"
#include "_2RealRunnableGraph.h"
#include "_2RealSystemGraph.h"

namespace _2Real
{

	Runnable::Runnable(Identifier const& id, SystemGraph &system) :
		Entity(id),
		m_System(system),
		m_Father(&system),
		m_Run(false),
		m_RunOnce(false),
		m_MaxDelay(long(1000.0f/30.0f)),
		m_UpdatesPerSecond(30.0f),
		m_Timer()
	{
	}

	Runnable::~Runnable()
	{
	}

	void Runnable::start(bool runOnce)
	{
		if (m_Father == &m_System)
		{
			m_RunOnce = runOnce;
			m_Run = !runOnce;
		}
		else
		{
			//TODO
			//what is the desired effect here?
		}
	}

	void Runnable::stop()
	{
		m_RunOnce = false;
		m_Run = false;
	}

	Runnable & Runnable::root()
	{
		if (m_Father == &m_System)
		{
			return *this;
		}
		else
		{
			return static_cast< RunnableGraph * >(m_Father)->root();
		}
	}

	Runnable const& Runnable::root() const
	{
		if (m_Father == &m_System)
		{
			return *this;
		}
		else
		{
			return static_cast< RunnableGraph * >(m_Father)->root();
		}
	}

}