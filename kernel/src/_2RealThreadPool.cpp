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

#include <iostream>

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
		for (ThreadList::iterator it=m_Threads.begin(); it!=m_Threads.end(); ++it)
		{
			(*it)->stopRunning();
		}

		for (ThreadList::iterator it=m_Threads.begin(); it!=m_Threads.end(); ++it)
		{
			if ((*it)->join())
			{
				delete *it;
			}
		}

		m_Threads.clear();
	}

	PooledThread & ThreadPool::getFreeThread()
	{
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
			std::cout << "threadpool: CREATED NEW THREAD " << m_Threads.size() << std::endl;

			thread = new PooledThread(m_StackSize);
			m_Threads.push_back(thread);
		}
		else
		{
			thread->reactivate();
		}
	
		return *thread;
	}

}