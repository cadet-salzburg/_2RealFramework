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
		m_Mutex.lock();
		Runnable *target = m_Target;
		m_Mutex.unlock();
		if (target)
			m_TargetCompleted.wait();
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

	ThreadPool::ThreadPool(unsigned int const& min, unsigned int const& max, unsigned int const& idleTime, unsigned int const& stackSize, std::string const& name) :  
		m_Capacity(min),
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
			m_Threads.insert(NamedThread(id, thread));
		}
	}

	ThreadPool::~ThreadPool()
	{
		stopAll();
	}

	const unsigned int ThreadPool::used() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		unsigned int count = 0;
		ThreadTable::const_iterator it;
		for (it = m_Threads.begin(); it != m_Threads.end(); ++it)
		{
			if (!(it->second->idle()))
			{
				++count;
			}
		}

		return count;
	}

	const unsigned int ThreadPool::allocated() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		return m_Capacity;
	}

	void ThreadPool::start(Runnable *const target)
	{
		PooledThread *thread;
		ThreadTable::iterator it = m_Threads.find(target->identifier());
		if (it != m_Threads.end())
		{
			if (it->second->idle())
			{
				thread = it->second;
			}
			else
			{
				return;
			}
		}

		if (!thread)
		{
			thread = getFreeThread();
			thread->start(Poco::Thread::PRIO_NORMAL, target);
			m_Threads.insert(NamedThread(target->identifier(), target));
		}
		else
		{
			thread->start(Poco::Thread::PRIO_NORMAL, target);
		}
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

		ThreadTable idleThreads;
		ThreadTable expiredThreads;
		ThreadTable activeThreads;
	
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
		return NULL;
	}

	PooledThread *const ThreadPool::createThread()
	{
		return new PooledThread(m_Name+"_thread", m_StackSize);
	}

}