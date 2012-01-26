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

#include "_2RealPooledThread.h"
#include "_2RealRunnable.h"
#include "_2RealRunnableGraph.h"

#include "Poco/ThreadLocal.h"

#include <iostream>

namespace _2Real
{

	PooledThread::PooledThread(unsigned int stackSize) :
		m_IsIdle(true),
		m_Target(NULL),
		m_Thread("unused thread"),
		m_RunThread(true),
		m_RunTarget(false),
		m_UpdateTarget(false),
		m_TargetReady(true),
		m_TargetCompleted(true),
		m_ThreadStarted(true),
		m_ThreadStopped(true),
		m_Timer()
	{
		m_Thread.setStackSize(stackSize);
		m_Thread.start(*this);
		m_ThreadStarted.wait();
	}

	bool PooledThread::isIdle()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);
		return m_IsIdle;
	}

	bool PooledThread::keepRunning()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);
		return m_RunThread;
	}

	bool PooledThread::keepTargetRunning()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);
		return m_RunThread && m_RunTarget;
	}

	bool PooledThread::updateTarget()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);
		return m_RunThread && m_UpdateTarget;
	}

	void PooledThread::stopRunning()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);
		m_RunThread = false;
		m_RunTarget = false;
		m_TargetReady.set();
	}

	void PooledThread::stopTargetRunning()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);
		m_RunTarget = false;
	}

	void PooledThread::start(Poco::Thread::Priority const& priority, _2Real::Runnable &target)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		m_Thread.setName(target.name());
		m_Thread.setPriority(priority);
		m_Target = &target;
		m_RunTarget = true;
		m_UpdateTarget = false;
		m_TargetReady.set();
	}

	void PooledThread::update(Poco::Thread::Priority const& priority, _2Real::Runnable &target)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		m_Thread.setName(target.name());
		m_Thread.setPriority(priority);
		m_Target = &target;
		m_RunTarget = false;
		m_UpdateTarget = true;
		m_TargetReady.set();
	}

	void PooledThread::reactivate()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);

		m_IsIdle = false;
		m_TargetCompleted.reset();
	}

	//!!!!!!
	bool PooledThread::join()
	{
		m_TargetCompleted.wait();

		const long timeout = 10000;
		return m_Thread.tryJoin(timeout);
	}

	void PooledThread::cleanUp()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Mutex);
		
		m_IsIdle = true;
		m_Target = NULL;
		m_TargetReady.reset();
		Poco::ThreadLocalStorage::clear();
		//m_Thread.setName("");
		m_Thread.setPriority(Poco::Thread::PRIO_NORMAL);
		m_TargetCompleted.set();	
	}

	void PooledThread::run()
	{
		m_ThreadStarted.set();

		while (keepRunning())
		{

			m_TargetReady.wait();

			if (updateTarget())
			{
				m_Target->run();
				RunnableGraph &father = static_cast< RunnableGraph & >(m_Target->father());
				father.childFinished(m_Target->identifier());
				cleanUp();
			}
			else
			{
				while (keepTargetRunning())
				{
					m_Timer.update();
					m_Target->run();

					if (!keepTargetRunning())
					{
						break;
					}
					else
					{
						long delay = m_Target->getMaxDelay();
						long elapsed = (long)m_Timer.elapsed()/1000;
						long sleep = delay - elapsed;
						if (sleep > 0)
						{
							Poco::Thread::sleep(sleep);
						}
						else
						{
						}
					}
				}

				cleanUp();
			}

		}

		std::cout << m_Thread.name() << " THREAD RUN FINISHED" << std::endl;
		m_ThreadStopped.set();
	}

}