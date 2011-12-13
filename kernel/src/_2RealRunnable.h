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

#include "_2RealEntity.h"

#include "Poco/Runnable.h"
#include "Poco/Timestamp.h"
#include "Poco/Thread.h"

namespace _2Real
{

	/**
	*	services, sequences and synchronizations are runnable
	*/

	class Graph;
	class SystemGraph;
	class RunnableGraph;

	class Runnable : public Poco::Runnable, public Entity
	{

	public:

		Runnable(Identifier const& id, SystemGraph &system);
		virtual ~Runnable();

		virtual void start(bool runOnce);
		virtual void stop();
		virtual void setup() = 0;
		virtual void run() = 0;
		virtual void update() = 0;
		virtual void shutdown() = 0;
		virtual bool checkForSetup() = 0;
		virtual bool checkForUpdate() = 0;

		Runnable & root();
		Runnable const&  root() const;

		void setFather(Graph &father);
		Graph &father();
		Graph const& father() const;

		SystemGraph &system();
		SystemGraph const& system() const;

		void setUpdateRate(float updatesPerSecond);
		void updateTimer();
		void suspend();

	protected:

		bool					m_Run;
		bool					m_RunOnce;

		SystemGraph				&m_System;
		Graph					*m_Father;

	private:

		Poco::Timestamp			m_Timer;
		long					m_MaxDelay;
		float					m_UpdatesPerSecond;

	};

	inline void Runnable::updateTimer()
	{
		m_Timer.update();
	}

	inline void Runnable::suspend()
	{
		long elapsed = (long)m_Timer.elapsed()/1000;
		long sleep = m_MaxDelay - elapsed;
		if (sleep > 0)
		{
			Poco::Thread::sleep(sleep);
		}
	}

	inline void Runnable::setFather(Graph &father)
	{
		m_Father = &father;
	}

	inline Graph & Runnable::father()
	{
		return *m_Father;
	}

	inline Graph const& Runnable::father() const
	{
		return *m_Father;
	}

	inline SystemGraph & Runnable::system()
	{
		return m_System;
	}

	inline SystemGraph const& Runnable::system() const
	{
		return m_System;
	}

	inline void Runnable::setUpdateRate(float updatesPerSecond)
	{
		m_MaxDelay = long(1000.0f/updatesPerSecond);
		m_UpdatesPerSecond = updatesPerSecond;
	}

}