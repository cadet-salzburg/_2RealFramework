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
		m_Run(true),
		m_RunTarget(false),
		m_RunTargetOnce(false),
		m_TargetReady(false),
		m_TargetCompleted(false),
		m_ThreadStarted(false),
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
		return m_Run;
	}

	void PooledThread::stopRunning()
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);
		m_Run = false;
	}

	bool PooledThread::keepTargetRunning() const
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);
		return m_RunTarget;
	}

	void PooledThread::stopTargetRunning()
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);
		m_RunTarget = false;
	}

	void PooledThread::start(Poco::Thread::Priority const& priority, _2Real::Runnable &target, bool runOnce)
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);

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

	void PooledThread::stop()
	{
		m_Mutex.lock();

		if (m_Run)
		{
			stopTargetRunning();
			m_Mutex.unlock();
			m_TargetCompleted.wait();
		}
		else
		{
			m_Mutex.unlock();
		}
	}

	void PooledThread::wait()
	{
		m_Mutex.lock();

		if (m_Run)
		{
			m_Mutex.unlock();
			m_TargetCompleted.wait();
		}
		else
		{
			m_Mutex.unlock();
		}
	}

	void PooledThread::reactivate()
	{
		Poco::Mutex::ScopedLock lock(m_Mutex);

		m_IsIdle = false;
		m_TargetCompleted.reset();
	}

	void PooledThread::kill()
	{
		const long timeout = 10000;
	
		stopRunning();
		m_TargetReady.set();

		if (m_Thread.tryJoin(timeout))
		{
			delete this;
		}
		else
		{
			std::cout << m_Thread.getName() << " join failed" << std::endl;
		}
	}

	void PooledThread::run()
	{
		m_ThreadStarted.set();

		while (keepRunning())
		{
			//if (m_Thread.getName() == "subtraction" || m_Thread.getName() == "addition")
			//{
			//	std::cout << m_Thread.getName() << " waiting for a target" << std::endl;
			//}
			
			//this will block until set
			m_TargetReady.wait();

			//if (m_Thread.getName() == "subtraction" || m_Thread.getName() == "addition")
			//{
			//	std::cout << m_Thread.getName() << " acquired target" << std::endl;
			//}

			while (keepTargetRunning())
			{

				//if (m_Thread.getName() == "subtraction" || m_Thread.getName() == "addition")
				//{
				//	std::cout << m_Thread.getName() << " inside run" << std::endl;
				//}

				m_Timer.update();
				m_Target->run();


				//if (m_Thread.getName() == "subtraction" || m_Thread.getName() == "addition")
				//{
				//	std::cout << m_Thread.getName() << " after run" << std::endl;
				//}

				if (m_RunTargetOnce || !keepRunning())
				{
					//std::cout << m_Thread.getName() << "has run once" << std::endl;
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

			//std::cout << m_Thread.getName() << " run finished" << std::endl;
			
			Poco::Mutex::ScopedLock lock(m_Mutex);
			m_TargetCompleted.set();
			
			//std::cout << m_Thread.getName() << " set completion" << std::endl;
			
			m_RunTarget = false;
			m_Target = NULL;

			//std::cout << m_Thread.getName() << " set to idle" << std::endl;

			m_IsIdle = true;
			m_RunTargetOnce = false;
			Poco::ThreadLocalStorage::clear();
			m_Thread.setName("");
			m_Thread.setPriority(Poco::Thread::PRIO_NORMAL);
		}
	}

}