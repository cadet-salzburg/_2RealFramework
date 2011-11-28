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

#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/ThreadLocal.h"
#include "Poco/ErrorHandler.h"

#include <ctime>
#include <math.h>
#include <sstream>
#include <iostream>

namespace _2Real
{

	class PooledThread : public Poco::Runnable
	{
	
	public:

		PooledThread(std::string const& name, unsigned int const& stackSize = POCO_THREAD_STACK_SIZE);

		void start();
		void start(Poco::Thread::Priority const& priority, _2Real::Runnable *const target);

		bool idle();
		int idleTime();
		void join();
		void activate();
		void release();
		void run();

	private:

		volatile bool			m_IsIdle;
		volatile std::time_t	m_IdleTime;
		_2Real::Runnable		*m_Target;
		Poco::Thread			m_Thread;
		Poco::Event				m_TargetReady;
		Poco::Event				m_TargetCompleted;
		Poco::Event				m_Started;
		Poco::FastMutex			m_Mutex;

	};

	PooledThread::PooledThread(std::string const& name, unsigned int const& stackSize) :
		m_IsIdle(true),
		m_IdleTime(0),
		m_Target(NULL),
		m_Thread(name),
		m_TargetCompleted(false)
	{
		m_Thread.setStackSize(stackSize);
		m_IdleTime = std::time(NULL);
	}

	void PooledThread::start()
	{
		std::cout << "pooled thread: start called" << std::endl;
		m_Thread.start(*this);
		m_Started.wait();
	}

	void PooledThread::start(Poco::Thread::Priority const& priority, _2Real::Runnable *const target)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_Thread.setName(target->name());
		m_Thread.setPriority(priority);
	
		//poco_assert (_pTarget == 0);

		m_Target = target;
		m_TargetReady.set();
	}

	inline bool PooledThread::idle()
	{
		return m_IsIdle;
	}

	int PooledThread::idleTime()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		return (int) (time(NULL) - m_IdleTime);
	}

	void PooledThread::join()
	{
		std::cout << "pooled thread: join" << std::endl;
		m_Mutex.lock();
		_2Real::Runnable *target = m_Target;
		m_Mutex.unlock();
		if (target)
		{
			std::cout << "pooled thread: waiting for completion of " << target->name() << std::endl;
			m_TargetCompleted.wait();
		}
		std::cout << "pooled thread: joined" << std::endl;
	}

	void PooledThread::activate()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
	
		//poco_assert (_idle);
	
		m_IsIdle = false;
		m_TargetCompleted.reset();
	}

	void PooledThread::release()
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

	void PooledThread::run()
	{
		m_Started.set();
		while (1)
		{
			std::cout << "pooled thread: waiting for target" << std::endl;
			
			m_TargetReady.wait();
			m_Mutex.lock();
			if (m_Target)
			{
				std::cout << "pooled thread: acquired target" << std::endl;
				m_Mutex.unlock();
				std::cout << "pooled thread: executing target's run " << m_Target->name() << std::endl;
				m_Target->run();
				std::cout << "pooled thread: target's run finished" << m_Target->name() << std::endl;
				
				Poco::FastMutex::ScopedLock lock(m_Mutex);
				m_Target = NULL;
				m_IdleTime = time(NULL);
				m_IsIdle = true;
				
				std::cout << "pooled thread: flaggining finish" << std::endl;
				m_TargetCompleted.set();
				std::cout << "pooled thread: flagged finish" << std::endl;
				Poco::ThreadLocalStorage::clear();
				std::cout << "pooled thread: cleared local storage" << std::endl;
				m_Thread.setName("");
				m_Thread.setPriority(Poco::Thread::PRIO_NORMAL);
				std::cout << "pooled thread: done" << std::endl;
			}
			else
			{
				m_Mutex.unlock();
				break;
			}
		}
	}

	ThreadPool::ThreadPool(unsigned int const& capacity, unsigned int const& max, unsigned int const& idleTime, unsigned int const& stackSize, std::string const& name) :  
		m_Capacity(capacity),
		m_MaxCapacity(max), 
		m_IdleTime(idleTime),
		m_Age(0),
		m_StackSize(stackSize),
		m_Name(name)
	{
		for (unsigned int i=0; i<m_Capacity; ++i)
		{
			PooledThread *thread = createThread();
			const Identifier id = Entity::NoEntity();
			thread->start();
			m_Threads.insert(NamedThread(id, thread));
		}
	}

	ThreadPool::~ThreadPool()
	{
		stopAll();
	}

	void ThreadPool::addCapacity(int const& n)
	{
		if (m_Capacity + n < 0)
		{
		}
		else if (m_Capacity + n > m_MaxCapacity)
		{
		}
		else
		{
			m_Capacity += n;
		}
	}

	void ThreadPool::join(Identifier const& id)
	{
		std::cout << "thread pool: " << m_Name << ", joining runnable " << id.name() << std::endl;

		PooledThread *thread = NULL;
		ThreadTable::iterator it = m_Threads.find(id);

		if (it == m_Threads.end())
		{
			std::cout << "thread pool " << m_Name << " attempted join of non-existant thread " << id.name() << std::endl;
			return;
		}
		else
		{
			std::cout << "thread pool: " << m_Name << ", joining now " << id.name() << std::endl;
			it->second->join();
			std::cout << "thread pool: " << m_Name << ", joined  " << id.name() << std::endl;
		}
	}

	void ThreadPool::stop(Identifier const& id)
	{
		PooledThread *thread = NULL;
		ThreadTable::iterator it = m_Threads.find(id);

		if (it == m_Threads.end())
		{
			std::cout << "thread pool " << m_Name << " attempted joind of non-existant thread " << id.name() << std::endl;
			return;
		}
		else
		{
			it->second->join();
			it->second->release();
		}
	}

	void ThreadPool::start(Runnable *const target)
	{
		std::cout << "thread pool: " << m_Name << ", starting runnable " << target->name() << std::endl;

		PooledThread *thread = NULL;
		ThreadTable::iterator it = m_Threads.find(target->identifier());
		if (it != m_Threads.end())
		{
			if (it->second->idle())
			{
				std::cout << "thread pool: found idle"  << std::endl;
				thread = it->second;
			}
			else
			{
				std::cout << "thread pool: found running"  << std::endl;
				return;
			}
		}

		if (!thread)
		{
			std::cout << "thread pool: not found & creating"  << std::endl;
			thread = getFreeThread();
			thread->start(Poco::Thread::PRIO_NORMAL, target);
			m_Threads.insert(NamedThread(target->identifier(), thread));
		}
		else
		{
			std::cout << "thread pool: found idle & starting"  << std::endl;
			thread->start(Poco::Thread::PRIO_NORMAL, target);
		}

		std::cout << "thread pool: " << m_Name << ", started runnable " << target->name() << std::endl;
	}

	void ThreadPool::stopAll()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		for (ThreadTable::iterator it=m_Threads.begin(); it!=m_Threads.end(); ++it)
		{
			it->second->release();
		}
		m_Threads.clear();
	}

	void ThreadPool::joinAll()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		for (ThreadTable::iterator it=m_Threads.begin(); it!=m_Threads.end(); ++it)
		{
			it->second->join();
		}
		
		housekeep();
	}

	void ThreadPool::collect()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		housekeep();
	}

	void ThreadPool::housekeep()
	{
		m_Age = 0;

		if (m_Threads.empty())
		{
			return;
		}

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

	PooledThread *const ThreadPool::getFreeThread()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (++m_Age == 32)
		{
			housekeep();
		}

		PooledThread *thread = NULL;
		ThreadTable::iterator it;
		for (it = m_Threads.begin(); it != m_Threads.end(); ++it)
		{
			if (it->second->idle())
			{
				thread = it->second;
				break;
			}
		}

		if (!thread)
		{
			if (m_Threads.size() < m_MaxCapacity)
			{
				thread = createThread();
				thread->start();
			}
			else
			{
				//throw NoThreadAvailableException();
			}
		}
		else
		{
			m_Threads.erase(it);
		}

		thread->activate();
		return thread;
	}

	PooledThread *const ThreadPool::createThread()
	{
		return new PooledThread(m_Name+"_thread", m_StackSize);
	}

}