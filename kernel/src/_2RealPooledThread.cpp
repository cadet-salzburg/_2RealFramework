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
//#include "_2RealRunnable.h"
//#include "_2RealException.h"
//
#include "Poco/ThreadLocal.h"
//
//#include <math.h>
//#include <sstream>
//#include <iostream>

namespace _2Real
{

	PooledThread::PooledThread(unsigned int stackSize) :
		m_IsIdle(true),
		m_IdleTime(0),
		m_Target(NULL),
		m_Thread(""),
		m_TargetReady(false),
		m_TargetCompleted(false),
		m_Started(false)
	{
		m_Thread.setStackSize(stackSize);
		m_IdleTime = std::time(NULL);
	}

	void PooledThread::start()
	{
		m_Thread.start(*this);
		m_Started.wait();
	}

	void PooledThread::start(Poco::Thread::Priority const& priority, _2Real::Runnable &target)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_Thread.setName(target.name());
		m_Thread.setPriority(priority);

		m_Target = &target;
		m_TargetReady.set();
	}

	bool PooledThread::isIdle() const
	{
		return m_IsIdle;
	}

	int PooledThread::idleTime() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		return (int) (time(NULL) - m_IdleTime);
	}

	void PooledThread::stop()
	{
		m_Mutex.lock();
		_2Real::Runnable *target = m_Target;
		m_Mutex.unlock();

		if (target)
		{
			target->stop();
		}
	}

	void PooledThread::stopAndJoin()
	{
		m_Mutex.lock();
		_2Real::Runnable *target = m_Target;
		m_Mutex.unlock();

		if (target)
		{
			target->stop();
			m_TargetCompleted.wait();
		}
	}

	void PooledThread::join()
	{
		m_Mutex.lock();
		_2Real::Runnable *target = m_Target;
		m_Mutex.unlock();

		if (target)
		{
			m_TargetCompleted.wait();
		}
	}

	void PooledThread::activate()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_IsIdle = false;
		m_TargetCompleted.reset();
	}

	void PooledThread::kill()
	{
		const long timeout = 10000;
	
		m_Mutex.lock();
		m_Target = NULL;
		m_Mutex.unlock();

		m_TargetReady.set();
		if (m_Thread.tryJoin(timeout))
		{
			delete this;
		}
	}

	const Identifier PooledThread::identifier() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (!m_Target)
		{
			return Entity::NoEntity();
		}
		else
		{
			return m_Target->identifier();
		}
	}

	void PooledThread::run()
	{
		m_Started.set();
		while (1)
		{
			m_TargetReady.wait();
			m_Mutex.lock();
			if (m_Target)
			{
				m_Mutex.unlock();

				m_Target->run();
				
				Poco::FastMutex::ScopedLock lock(m_Mutex);
				m_Target = NULL;
				m_IdleTime = time(NULL);
				m_IsIdle = true;
				
				m_TargetCompleted.set();
				Poco::ThreadLocalStorage::clear();
				m_Thread.setName("");
				m_Thread.setPriority(Poco::Thread::PRIO_NORMAL);
			}
			else
			{
				m_Mutex.unlock();
				break;
			}
		}
	}

}