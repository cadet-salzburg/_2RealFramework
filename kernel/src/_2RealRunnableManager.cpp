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
#include "_2RealRunnableState.h"
#include "_2RealRunnableStateCreated.h"
#include "_2RealRunnableStateSetUp.h"
#include "_2RealRunnableStateReady.h"
#include "_2RealRunnableStateUpdating.h"
#include "_2RealRunnableStateShutDown.h"
#include "_2RealRunnableStateError.h"
#include "_2RealException.h"
#include "_2RealPooledThread.h"
#include "_2RealIStateChangeListener.h"
#include "_2RealEngineImpl.h"
#include "_2RealTimer.h"
#include "_2RealSystemImpl.h"
#include "_2RealUpdateTriggersImpl.h"
#include "_2RealInputSlot.h"

#include "Poco/Delegate.h"

namespace _2Real
{

	RunnableManager::RunnableManager(Runnable &runnable, UpdateTriggersImpl &triggers) :
		m_Runnable(&runnable),
		m_Threads(EngineImpl::instance().getThreadPool()),
		m_CurrentState(new RunnableStateCreated()),
		m_Triggers(*this),
		m_StopRunning(false),
		m_StopEvent(false),
		m_LastUpdateInitialized(0),
		m_LastUpdateFinalized(0),
		m_LastTriggersReady(0),
		m_ShutDownReceived(0),
		m_System(runnable.system()),
		m_Waiting(false)
	{
		AbstractTimeBasedTrigger *t = triggers.getTimeBasedTrigger();
		if (t != NULL)
		{
			EngineImpl::instance().getTimer().registerToTimerSignal(m_Triggers);
			m_Triggers.addTimeBasedTrigger(*t);
		}

		//InputMap const& inlets = m_Runnable->getInlets();

		//for (InputMap::const_iterator it = inlets.begin(); it != inlets.end(); ++it)
		//{
		//	if (triggers.getInletBasedTrigger(it->second->getName()) != NULL)
		//	{
		//		std::cout << "NULL TRIGGER for inlet " << it->second->getName() << std::endl;
		//	}
		//	else
		//	{
		//		std::cout << "VALID TRIGGER for inlet " << it->second->getName() << std::endl;
		//	}
		//	it->second->registerToDataReceived(m_Triggers);
		//	m_Triggers.addInletBasedTrigger(*triggers.getInletBasedTrigger(it->second->getName()));
		//}
	}

	void RunnableManager::debug()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		std::cout << m_Runnable->name() << ":" << std::endl;
		std::cout << "received last trigger: " << m_LastTriggersReady << std::endl;
		std::cout << "began last update: " << m_LastUpdateInitialized << std::endl;
		std::cout << "finished last update: " << m_LastUpdateFinalized << std::endl;
		std::cout << "received shutdown: " << m_ShutDownReceived << std::endl;
	}

	RunnableManager::~RunnableManager()
	{
		EngineImpl::instance().getTimer().unregisterFromTimerSignal(m_Triggers);

		delete m_Runnable;
		delete m_CurrentState;
	}

	Identifier const& RunnableManager::getManagedId() const
	{
		return m_Runnable->identifier();
	}

	std::string const& RunnableManager::getManagedName() const
	{
		return m_Runnable->name();
	}

	Runnable& RunnableManager::getManagedRunnable() const
	{
		return *m_Runnable;
	}

	void RunnableManager::registerToStateChange(StateChangeCallback callback)
	{
		m_StateChangeEvent += Poco::delegate(callback);
	}

	void RunnableManager::unregisterFromStateChange(StateChangeCallback callback)
	{
		m_StateChangeEvent -= Poco::delegate(callback);
	}

	void RunnableManager::registerToStateChange(IStateChangeListener &listener)
	{
		m_StateChangeEvent += Poco::delegate(&listener, &IStateChangeListener::receiveStateChange);
	}

	void RunnableManager::unregisterFromStateChange(IStateChangeListener &listener)
	{
		m_StateChangeEvent -= Poco::delegate(&listener, &IStateChangeListener::receiveStateChange);
	}

	void RunnableManager::registerToStateChange(RunnableTriggers &triggers)
	{
		m_StateChangeEvent += Poco::delegate(&triggers, &RunnableTriggers::tryTriggerChild);
	}

	void RunnableManager::unregisterFromStateChange(RunnableTriggers &triggers)
	{
		m_StateChangeEvent -= Poco::delegate(&triggers, &RunnableTriggers::tryTriggerChild);
	}

	bool RunnableManager::isSetUp() const
	{
		return m_CurrentState->getStateId() == 1;
	}

	unsigned int RunnableManager::getState() 
	{ 
		return m_CurrentState->getStateId();
	}

	void RunnableManager::handleStateChangeException(_2Real::Exception &e)
	{
		std::cout << m_Runnable->name() << " IS NOW IN ERROR STATE" << std::endl;

		//welcome to error state, enjoy your stay
		delete m_CurrentState;
		m_CurrentState = new RunnableStateError();
		unsigned int state = m_CurrentState->getStateId();
		m_StateChangeEvent.notify(this, state);
	}

	void RunnableManager::setUp()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		try
		{
			m_CurrentState->setUp(*m_Runnable);
			m_Triggers.startTriggers();
		}
		catch (_2Real::Exception &e)
		{
			std::cout << "exception on setup" << std::endl;
			handleStateChangeException(e);
		}

		delete m_CurrentState;
		m_CurrentState = new RunnableStateSetUp();
		unsigned int state = m_CurrentState->getStateId();
		m_StateChangeEvent.notify(this, state);
	}

	void RunnableManager::getReady()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		try
		{
			m_LastTriggersReady = m_System.getElapsedTime();

			if (m_CurrentState->tryGetReady())
			{
				m_Triggers.stopTriggers();

				delete m_CurrentState;
				m_CurrentState = new RunnableStateReady();
				unsigned int state = m_CurrentState->getStateId();
				m_StateChangeEvent.notify(this, state);

				m_Threads.scheduleRunnable(*this);
			}
		}
		catch (_2Real::Exception &e)
		{
			std::cout << "exception on ready" << std::endl;
			handleStateChangeException(e);
		}
	}

	const bool RunnableManager::beginUpdate()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		try
		{
			m_LastUpdateInitialized = m_System.getElapsedTime();

			if (!m_StopRunning && m_CurrentState->tryBeginUpdate())
			{
				delete m_CurrentState;
				m_CurrentState = new RunnableStateUpdating();
				unsigned int state = m_CurrentState->getStateId();
				m_StateChangeEvent.notify(this, state);

				return true;
			}

			return false;
		}
		catch (_2Real::Exception &e)
		{
			std::cout << "exception on begin update" << std::endl;
			handleStateChangeException(e);
			return false;
		}
	}

	void RunnableManager::finishUpdate()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		try
		{
			m_LastUpdateFinalized = m_System.getElapsedTime();

			if (m_CurrentState->tryFinishUpdate())
			{
				delete m_CurrentState;
				m_CurrentState = new RunnableStateSetUp();
				unsigned int state = m_CurrentState->getStateId();
				m_StateChangeEvent.notify(this, state);

				if (m_Waiting)
				{
					m_StopEvent.set();
					std::cout << m_Runnable->name() << " received delayed finish" << std::endl;
				}
				if (!m_StopRunning)
				{
					m_Triggers.startTriggers();
				}
			}
		}
		catch (_2Real::Exception &e)
		{
			std::cout << "exception on finish" << std::endl;
			handleStateChangeException(e);
		}
	}

	void RunnableManager::prepareForShutDown()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		m_StopRunning = true;

		EngineImpl::instance().getTimer().unregisterFromTimerSignal(m_Triggers);

		m_Triggers.stopTriggers();

		if (m_Threads.unscheduleRunnable(*this))
		{
			delete m_CurrentState;
			m_CurrentState = new RunnableStateSetUp();
			unsigned int state = m_CurrentState->getStateId();
			m_StateChangeEvent.notify(this, state);
		}
	}

	void RunnableManager::prepareForAbort()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		delete m_CurrentState;
		m_CurrentState = new RunnableStateError();
		unsigned int state = m_CurrentState->getStateId();
		m_StateChangeEvent.notify(this, state);

		//disable all inlet / outlet events
		m_Runnable->prepareForAbort();
	}

	void RunnableManager::abort(_2Real::Exception &e)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		m_Threads.abortRunnable(*this, e);
	}

	const bool RunnableManager::shutDown()
	{
		m_Mutex.lock();

		try
		{
			m_ShutDownReceived = m_System.getElapsedTime();

			if (m_CurrentState->shutDown(*m_Runnable) || m_StopEvent.tryWait(500))
			{
				delete m_CurrentState;
				m_CurrentState = new RunnableStateShutDown();
				unsigned int state = m_CurrentState->getStateId();
				m_StateChangeEvent.notify(this, state);

				m_Mutex.unlock();
				return true;
			}
			else
			{
				m_Waiting = true;
				m_Mutex.unlock();
				if (m_StopEvent.tryWait(500))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		catch (_2Real::Exception &e)
		{
			std::cout << "exception on shutdown" << std::endl;
			m_Mutex.unlock();
			handleStateChangeException(e);
			return false;
		}
	}

}