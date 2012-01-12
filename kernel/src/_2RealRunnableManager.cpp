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

#include "_2RealRunnableManager.h"
#include "_2RealRunnable.h"
#include "_2RealRunnableStates.h"
#include "_2RealException.h"
#include "_2RealPooledThread.h"

#include <iostream>

namespace _2Real
{

	RunnableManager::RunnableManager(Runnable &runnable) :
		m_Runnable(&runnable),
		m_CurrentState(NULL),
		m_Thread(NULL),
		m_RunnableStates()
	{
		m_CurrentState = new RunnableStateCreated(*this);

		m_RunnableStates[STATE_CREATED] = m_CurrentState;
		m_RunnableStates[STATE_SETUP] = new RunnableStateSetUp(*this);
		m_RunnableStates[STATE_RUNNING] = new RunnableStateRunning(*this);
		m_RunnableStates[STATE_UPDATING] = new RunnableStateUpdating(*this);
		m_RunnableStates[STATE_SHUTDOWN] = new RunnableStateShutDown(*this);
		m_RunnableStates[STATE_ERROR] = new RunnableStateHalted(*this);
	}

	RunnableManager::~RunnableManager()
	{
		for (std::map< std::string, RunnableState * >::iterator it = m_RunnableStates.begin(); it != m_RunnableStates.end(); ++it)
		{
			delete it->second;
		}

		delete m_Runnable;
	}

	bool RunnableManager::isRunning() const
	{
		return m_CurrentState == &getState(STATE_RUNNING);
	}

	bool RunnableManager::isUpdating() const
	{
		return m_CurrentState == &getState(STATE_UPDATING);
	}

	bool RunnableManager::isSetUp() const
	{
		return m_CurrentState == &getState(STATE_SETUP);
	}

	RunnableState & RunnableManager::getState(std::string const& stateName) const
	{
		StateTable::const_iterator it = m_RunnableStates.find(stateName);
		return *it->second;
	}

	Identifier const& RunnableManager::getManagedId() const
	{
		return m_Runnable->identifier();
	}

	void RunnableManager::setup()
	{
		m_CurrentState = &m_CurrentState->setup(*m_Runnable);
	}

	void RunnableManager::start(PooledThread &thread)
	{
		m_CurrentState = &m_CurrentState->start(*m_Runnable, thread);
		m_Thread = &thread;
	}

	void RunnableManager::update(PooledThread &thread)
	{
		m_CurrentState = &m_CurrentState->update(*m_Runnable, thread);
		m_Thread = &thread;
	}

	void RunnableManager::stop()
	{
		if (m_Thread)
		{
			//std::cout << "manager: stopping: " << m_Runnable->id() << std::endl;
			m_CurrentState = &m_CurrentState->stop(*m_Runnable, *m_Thread);
		}
		else
		{
			//std::cout << "manager: stopping NOTHREAD " << m_Runnable->id() << std::endl;
		}
	}

	void RunnableManager::wait()
	{
		if (m_Thread)
		{
			m_CurrentState = &m_CurrentState->wait(*m_Runnable, *m_Thread);
		}
	}

	void RunnableManager::shutdown()
	{
		m_CurrentState = &m_CurrentState->shutdown(*m_Runnable);
	}

	void RunnableManager::handleException()
	{
		m_CurrentState = &m_CurrentState->handleException(*m_Runnable);
	}

}