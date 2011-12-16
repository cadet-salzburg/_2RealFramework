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

namespace _2Real
{

	RunnableManager::RunnableManager(Runnable &runnable) :
		m_Runnable(runnable)
	{
		m_CurrentState = new RunnableStateCreated(*this);

		m_RunnableStates[STATE_CREATED] = m_CurrentState;
		m_RunnableStates[STATE_SETUP] = new RunnableStateSetUp(*this);
		m_RunnableStates[STATE_STARTED] = new RunnableStateStarted(*this);
		m_RunnableStates[STATE_SHUTDOWN] = new RunnableStateShutDown(*this);
		m_RunnableStates[STATE_ERROR] = new RunnableStateHalted(*this);
	}

	RunnableManager::~RunnableManager()
	{
		for (std::map< std::string, RunnableState * >::iterator it = m_RunnableStates.begin(); it != m_RunnableStates.end(); ++it)
		{
			delete it->second;
		}
	}

	RunnableState & RunnableManager::getState(std::string const& stateName) const
	{
		StateTable::const_iterator it = m_RunnableStates.find(stateName);

		if (it == m_RunnableStates.end())
		{
		}

		return *it->second;
	}

	Identifier const& RunnableManager::getManagedId() const
	{
		return m_Runnable.identifier();
	}

	void RunnableManager::start(PooledThread &thread, bool runOnce)
	{
		m_CurrentState->start(m_Runnable, thread, runOnce);
	}

	void RunnableManager::stop()
	{
		m_CurrentState->stop(m_Runnable);
	}

	void RunnableManager::setup()
	{
		m_CurrentState->setup(m_Runnable);
	}

	void RunnableManager::shutdown()
	{
		m_CurrentState->shutdown(m_Runnable);
	}

	void RunnableManager::update()
	{
		m_CurrentState->update(m_Runnable);
	}

	void RunnableManager::handleException()
	{
		m_CurrentState->handleException(m_Runnable);
	}

}