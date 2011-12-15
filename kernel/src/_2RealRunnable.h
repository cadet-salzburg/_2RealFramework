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

#include "_2RealPooledThread.h"
#include "_2RealEntity.h"

#include "Poco/Runnable.h"
#include "Poco/Timestamp.h"
#include "Poco/Thread.h"

#include <sstream>
#include <map>

namespace _2Real
{

	/**
	*	services, sequences and synchronizations are runnable
	*/

	class Graph;
	class SystemGraph;
	class RunnableGraph;
	class PooledThread;

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
		virtual void performStartCheck() const = 0;

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

	class RunnableState;

	/**
	*	oh dear
	*/

	class RunnableManager
	{

	public:

		RunnableManager(Runnable &runnable) :
			m_Runnable(runnable)
		{
			m_States["created"] = new RunnableCreated(*this);
			m_States["set up"] = new RunnableSetUp(*this);
			m_States["started"] = new RunnableStarted(*this);
			m_States["shut down"] = new RunnableShutDown(*this);
			m_States["halted"] = new RunnableHalted(*this);

			m_Current = 
		}

		~RunnableManager()
		{
			for (std::map< std::string, RunnableState * >::iterator it = m_States.begin(); it != m_States.end(); ++it)
			{
				delete it->second;
			}
		}

		//RunnableManager

	private:

		Runnable									&m_Runnable;
		std::map< std::string, RunnableState * >	m_States;
		RunnableState								*m_Current;

	};

	class RunnableState
	{

	public:

		RunnableState(RunnableManager const& manager, std::string const& description);
		std::string const& getDescription();
		virtual ~RunnableState();
		virtual RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce) = 0;
		virtual RunnableState & update(Runnable &runnable) = 0;
		virtual RunnableState & stop(Runnable &runnable) = 0;
		virtual RunnableState & setup(Runnable &runnable) = 0;
		virtual RunnableState & shutdown(Runnable &runnable) = 0;
		virtual RunnableState & handleException(Runnable &runnable) = 0;

	protected:

		std::string			const m_Description;
		RunnableManager		const& m_Manager;

	};

	inline RunnableState::RunnableState(RunnableManager const& manager, std::string const& description) :
		m_Description(description),
		m_Manager(manager)
	{
	}

	inline std::string const& RunnableState::getDescription()
	{
		return m_Description;
	}

	inline RunnableState::~RunnableState()
	{
	}

	class RunnableCreated : public RunnableState
	{

	public:

		RunnableCreated(RunnableManager const& manager) :
			RunnableState(manager, "runnable was just created and should be set up now")
		{
		}

		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce)
		{
			std::ostringstream msg;
			msg << "runnable " << runnable.name() << " can't be started, needs to be set up first";
			throw StartException(msg.str());
		}

		RunnableState & update(Runnable &runnable)
		{
			std::ostringstream msg;
			msg << "runnable " << runnable.name() << " can't be updated, needs to be set up first";
			throw StartException(msg.str());
		}

		RunnableState & stop(Runnable &runnable)
		{
			//do nothing
			return *this;
		}

		RunnableState & setup(Runnable &runnable)
		{
			//setup might fail
			runnable.setup();
			return runnable.getState("set up");
		}

		RunnableState & shutdown(Runnable &runnable)
		{
			runnable.shutdown();
			return runnable.getState("shut down");
		}

		RunnableState & handleException()
		{
		}

	};

	class RunnableSetUp : public RunnableState
	{

	public:

		RunnableSetUp(RunnableManager const& manager) :
			RunnableState(manager, "runnable is set up ready to run")
		{
		}

		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce)
		{
			//start might fail
			runnable.start(runOnce);
			thread.start(Poco::Thread::PRIO_NORMAL, runnable);
			return runnable.getState("started");
		}

		RunnableState & update(Runnable &runnable)
		{
			runnable.update();
			return *this;
		}

		RunnableState & stop(Runnable &runnable)
		{
			//do nothing
		}

		RunnableState & setup(Runnable &runnable)
		{
			runnable.setup();
			return *this;
		}

		RunnableState & shutdown(Runnable &runnable)
		{
			runnable.shutdown();
			return runnable.getState("shut down");
		}

		RunnableState & handleException()
		{
		}

	};

	class RunnableStarted : public RunnableState
	{

	public:

		RunnableStarted(RunnableManager const& manager) :
			RunnableState(manager, "runnable is running currently")
		{
		}

		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce)
		{
		}

		RunnableState & update(Runnable &runnable)
		{
			//this should not happen, evar
		}

		RunnableState & stop(Runnable &runnable)
		{
			//do nothing
		}

		RunnableState & setup(Runnable &runnable)
		{
			//what to do here?
		}

		RunnableState & shutdown(Runnable &runnable)
		{
			runnable.shutdown();
			return runnable.getState("shut down");
		}

		RunnableState & handleException()
		{
		}

	};

	class RunnableHalted : public RunnableState
	{

	public:

		RunnableHalted(RunnableManager const& manager) :
			RunnableState(manager, "an exception occured in the runnable, try setting it up again")
		{
		}

		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce)
		{
		}

		RunnableState & update(Runnable &runnable)
		{
			//this should not happen, evar
		}

		RunnableState & stop(Runnable &runnable)
		{
			//do nothing
		}

		RunnableState & setup(Runnable &runnable)
		{
			//what to do here?
		}

		RunnableState & shutdown(Runnable &runnable)
		{
			runnable.shutdown();
			return runnable.getState("shut down");
		}

		RunnableState & handleException()
		{
		}

	};

	class RunnableShutDown: public RunnableState
	{

	public:

		RunnableShutDown(RunnableManager const& manager) :
			RunnableState(manager, "an exception occured in the runnable, try setting it up again")
		{
		}

		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce)
		{
		}

		RunnableState & update(Runnable &runnable)
		{
			//this should not happen, evar
		}

		RunnableState & stop(Runnable &runnable)
		{
			//do nothing
		}

		RunnableState & setup(Runnable &runnable)
		{
			//what to do here?
		}

		RunnableState & shutdown(Runnable &runnable)
		{
			runnable.shutdown();
			return runnable.getState("shut down");
		}

		RunnableState & handleException()
		{
		}

	};

}