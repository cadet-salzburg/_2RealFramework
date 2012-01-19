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
		m_MaxDelay(long(1000.0f/30.0f)),
		m_Mutex()
	{
	}

	Runnable::~Runnable()
	{
	}

	void Runnable::handleException()
	{
		//empty for now
	}

	Runnable & Runnable::root()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

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
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		if (m_Father == &m_System)
		{
			return *this;
		}
		else
		{
			return static_cast< RunnableGraph * >(m_Father)->root();
		}
	}

	bool Runnable::isRoot() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		return (m_Father == &m_System);
	}

	long Runnable::getMaxDelay() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		return m_MaxDelay;
	}

	void Runnable::setFather(Graph &father)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		m_Father = &father;
	}

	Graph & Runnable::father()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		return *m_Father;
	}

	Graph const& Runnable::father() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		return *m_Father;
	}

	SystemGraph & Runnable::system()
	{
		return m_System;
	}

	SystemGraph const& Runnable::system() const
	{
		return m_System;
	}

	void Runnable::setUpdateRate(float updatesPerSecond)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		m_MaxDelay = long(1000.0f/updatesPerSecond);
	}

}