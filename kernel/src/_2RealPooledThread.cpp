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

#include "Poco/ThreadLocal.h"

#include <iostream>

namespace _2Real
{

	PooledThread::PooledThread(unsigned int stackSize) :
		m_IsIdle(true),
		m_Target(NULL),
		m_Thread(""),
		m_RunThread(true),
		m_RunTarget(false),
		m_RunTargetOnce(false),
		m_TargetReady(true),			//target ready can be queried excately once per target
		m_TargetCompleted(true),		//target completed can be queried exactely once per target
		m_ThreadStarted(false),			//thread started can be queried as often as one likes
		m_ThreadStopped(false),			//thread completed can be queried as often as one likes
		m_Timer()
	{
		m_Thread.setStackSize(stackSize);
		m_Thread.start(*this);
		m_ThreadStarted.wait();
	}

	bool PooledThread::isIdle() const
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);
		return m_IsIdle;
	}

	bool PooledThread::keepRunning() const
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);
		return m_RunThread;
	}

	bool PooledThread::keepTargetRunning() const
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);
		return m_RunTarget && m_RunThread;
	}

	void PooledThread::stopRunning()
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);
		m_RunThread = false;
	}

	void PooledThread::stopTargetRunning()
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);
		m_RunTarget = false;
	}

	void PooledThread::start(Poco::Thread::Priority const& priority, _2Real::Runnable &target, bool runOnce)
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);

		//std::cout << "thread starting target: " << target.name() << std::endl;

		m_Thread.setName(target.name());
		m_Thread.setPriority(priority);
		m_Target = &target;
		m_RunTarget = true;

		if (runOnce)
		{
			m_RunTargetOnce = true;
		}

		m_TargetReady.set();
	}

	void PooledThread::waitForTarget()
	{
		//m_Mutex.lock();
		//if (!m_IsIdle)
		//{
			//std::cout << "thread waiting on target NOT_IDLE name: " << m_Target->name() << std::endl;
			//m_Mutex.unlock();
		m_TargetCompleted.wait();
		//}
		//else
		//{
			//std::cout << "thread waiting on target IS_IDLE noname" << std::endl;
			//m_Mutex.unlock();
		//}
	}

	void PooledThread::joinThread()
	{
		//m_Mutex.lock();
		//if (!m_IsIdle)
		//{
			//std::cout << "thread waiting on target NOT_IDLE name: " << m_Target->name() << std::endl;
			//m_Mutex.unlock();
		m_ThreadStopped.wait();
		//}
		//else
		//{
			//std::cout << "thread waiting on target IS_IDLE noname" << std::endl;
			//m_Mutex.unlock();
		//}
	}

	void PooledThread::reactivate()
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);

		m_IsIdle = false;
		m_TargetCompleted.reset();
	}

	void PooledThread::kill()
	{
		//joinThread();
		Poco::Mutex::ScopedLock lock(m_Mutex);

	//	m_ThreadStopped.wait();
		std::cout << "attempt to join" << std::endl;

		const long timeout = 10000;
		if (m_Thread.tryJoin(timeout))
		{
			delete this;
		}
		else
		{
			std::cout << m_Thread.getName() << " join failed" << std::endl;
		}
	}

	void PooledThread::cleanUp()
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);

		m_IsIdle = true;
		m_Target = NULL;
		m_TargetReady.reset();
		Poco::ThreadLocalStorage::clear();
		m_Thread.setName("");
		m_Thread.setPriority(Poco::Thread::PRIO_NORMAL);
		m_TargetCompleted.set();
	}

	void PooledThread::run()
	{
		m_ThreadStarted.set();

		while (keepRunning())
		{

			m_TargetReady.wait();

			while (keepTargetRunning())
			{
				m_Timer.update();
				m_Target->run();

				if (m_RunTargetOnce)
				{
					m_RunTargetOnce = false;
					break;
				}
				else if (!keepTargetRunning())
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
				}
			}

			cleanUp();
		}

		std::cout << "THREAD RUN END" << std::endl;
		m_ThreadStopped.set();
	}

}