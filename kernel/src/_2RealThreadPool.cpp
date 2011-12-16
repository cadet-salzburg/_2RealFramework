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
#include "_2RealPooledThread.h"
#include "_2RealRunnable.h"
#include "_2RealException.h"

#include <sstream>

namespace _2Real
{

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

	PooledThread * ThreadPool::find(Identifier const& runnable)
	{
		for (ThreadList::iterator it=m_Threads.begin(); it != m_Threads.end(); ++it)
		{
			if ((*it)->identifier() == runnable)
			{
				return *it;
			}
		}

		return NULL;
	}

	void ThreadPool::join(Identifier const& id)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		PooledThread *thread = find(id);
		if (thread)
		{
			thread->join();
		}
	}

	void ThreadPool::stop(Identifier const& id)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		PooledThread *thread = find(id);
		if (thread)
		{
			thread->stop();
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