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

#include "_2RealThreadPool.h"
#include "_2RealRunnable.h"
#include "_2RealException.h"

#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/ThreadLocal.h"
#include "Poco/ErrorHandler.h"

#include <ctime>
#include <math.h>
#include <sstream>

namespace _2Real
{

	class PooledThread : public Poco::Runnable
	{
	
	public:

		PooledThread(unsigned int stackSize = POCO_THREAD_STACK_SIZE);

		void start();
		void start(Poco::Thread::Priority const& priority, _2Real::Runnable &target);

		bool isIdle() const;
		int idleTime() const;

		//stops target, waits for completion
		void stopAndJoin();

		//marks idle thread as non-idle
		void activate();

		//kilsl thread
		void kill();

		//runs the target
		void run();

		const Identifier identifier() const;

	private:

		volatile bool				m_IsIdle;
		volatile std::time_t		m_IdleTime;
		_2Real::Runnable			*m_Target;
		Poco::Thread				m_Thread;
		Poco::Event					m_TargetReady;
		Poco::Event					m_TargetCompleted;
		Poco::Event					m_Started;
		mutable Poco::FastMutex		m_Mutex;

	};

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

	ThreadPool::ThreadPool(unsigned int capacity, unsigned int idleTime, unsigned int stackSize, std::string const& name) :
		m_Capacity(capacity),
		m_IdleTime(idleTime),
		m_Age(0),
		m_StackSize(stackSize),
		m_Name(name)
	{
		for (unsigned int i=0; i<m_Capacity; ++i)
		{
			PooledThread *thread = new PooledThread(m_StackSize);
			thread->start();
			m_Threads.push_back(thread);
		}
	}

	ThreadPool::~ThreadPool()
	{
		if (m_Threads.size() > 0)
		{
			clearThreads();
		}
	}

	void ThreadPool::clearThreads()
	{
		joinAll();
		stopAll();
		m_Threads.clear();
	}

	bool ThreadPool::isRunning(Identifier const& runnable)
	{
		for (ThreadList::iterator it = m_Threads.begin(); it != m_Threads.end(); ++it)
		{
			if ((*it)->identifier() == runnable)
			{
				return true;
			}
		}

		return false;
	}

	PooledThread & ThreadPool::find(Identifier const& runnable)
	{
		for (ThreadList::iterator it=m_Threads.begin(); it != m_Threads.end(); ++it)
		{
			if ((*it)->identifier() == runnable)
			{
				return **it;
			}
		}

		std::ostringstream msg;
		msg << "threadpool " << m_Name << " error: runnable " << runnable << " not found";
		throw ThreadpoolException(msg.str());
	}

	void ThreadPool::stop(Identifier const& id)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (isRunning(id))
		{
			PooledThread &thread = find(id);
			thread.stopAndJoin();
		}
	}

	void ThreadPool::start(Runnable &target, bool runOnce)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (!isRunning(target.identifier()))
		{
			target.start(runOnce);
			PooledThread &thread = getFreeThread();
			thread.start(Poco::Thread::PRIO_NORMAL, target);
		}
	}

	void ThreadPool::stopAll()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		for (ThreadList::iterator it=m_Threads.begin(); it!=m_Threads.end(); ++it)
		{
			(*it)->kill();
		}
	}

	void ThreadPool::joinAll()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		for (ThreadList::iterator it=m_Threads.begin(); it!=m_Threads.end(); ++it)
		{
			(*it)->stopAndJoin();
		}
	}

	void ThreadPool::housekeep()
	{
		//m_Age = 0;

		//if (m_Threads.empty())
		//{
		//	return;
		//}

		/*ThreadTable idleThreads;
		ThreadTable expiredThreads;
		ThreadTable activeThreads;*/
	
		//for (ThreadVector::iterator it=m_Threads.begin(); it!=m_Threads.end(); it++)
		//{
		//	if ((*it)->idle())
		//	{
		//		if ((*it)->idleTime() < m_IdleTime)
		//		{
		//			idleThreads.push_back(*it);
		//		}
		//		else
		//		{
		//			expiredThreads.push_back(*it);
		//		}
		//	}
		//	else
		//	{
		//		activeThreads.push_back(*it);
		//	}
		//}

		//unsigned int limit = std::min< unsigned int >(idleThreads.size() + activeThreads.size(), m_MinCapacity);
	
		//m_Threads.clear();
		//m_Threads.reserve(limit);
		//m_Threads.insert(m_Threads.end(), activeThreads.begin(), activeThreads.end());
		//m_Threads.insert(m_Threads.end(), idleThreads.begin(), idleThreads.end());

		//unsigned int count = m_Threads.size();

		////makes sure that there are at leats m_MinCapacity threads running, releases other threads
		//for (ThreadVector::iterator it=expiredThreads.begin(); it!=expiredThreads.end(); it++)
		//{
		//	if (count < limit)
		//	{
		//		m_Threads.push_back(*it);
		//		count++;
		//	}
		//	else
		//	{
		//		(*it)->release();
		//	}
		//}
	}

	PooledThread & ThreadPool::getFreeThread()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		//huh?
		//if (++m_Age == 32)
		//{
		//	housekeep();
		//}

		PooledThread *thread = NULL;
		for (ThreadList::iterator it = m_Threads.begin(); it != m_Threads.end(); ++it)
		{
			if ((*it)->isIdle())
			{
				thread = *it;
				break;
			}
		}

		if (!thread)
		{
			thread = new PooledThread(m_StackSize);
			thread->start();
			m_Threads.push_back(thread);
		}
		else
		{
			thread->activate();
		}
	
		return *thread;
	}

}