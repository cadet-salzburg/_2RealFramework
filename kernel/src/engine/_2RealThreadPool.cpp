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

#include "engine/_2RealThreadPool.h"
#include "engine/_2RealPooledThread.h"
#include "engine/_2RealFunctionBlockStateManager.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealTimer.h"
#include "app/_2RealCallbacksInternal.h"

#include <iostream>
#include <algorithm>

namespace _2Real
{

	ThreadPool::ThreadPool( EngineImpl &engine, const unsigned int capacity, const unsigned int stackSize, std::string const& name ) :
		m_StackSize(stackSize),
		m_Name(name),
		m_Threads(),
		m_ReadyRunnables(),
		m_ExecutingRunnables(),
		m_AbortedRunnables(),
		m_Timer( engine.getTimer() )
#ifdef _2REAL_DEBUG
		, m_Elapsed(0)
#endif
	{
		AbstractCallback< long > *callback = new MemberCallback< ThreadPool, long >( *this, &ThreadPool::update );
		m_Timer.registerToTimerSignal( *callback );

		for (unsigned int i=0; i<capacity; ++i)
		{
			ThreadPoolCallback *callback = new ThreadPoolCallback(*this);
			PooledThread *thread = new PooledThread(*callback, m_StackSize);
			m_Threads.push_back(thread);
		}
	}

	ThreadPool::~ThreadPool()
	{
		AbstractCallback< long > *callback = new MemberCallback< ThreadPool, long >( *this, &ThreadPool::update );
		m_Timer.unregisterFromTimerSignal( *callback );

		clear();
	}

	void ThreadPool::clear()
	{
		m_ReadyAccess.lock();
		m_ReadyRunnables.clear();
		m_ReadyAccess.unlock();

		m_ExecutingAccess.lock();
		m_ExecutingRunnables.clear();
		m_ExecutingAccess.unlock();

		m_AbortedAccess.lock();
		m_AbortedRunnables.clear();
		m_AbortedAccess.unlock();

		m_FinishedAccess.lock();
		m_FinishedRunnables.clear();
		m_ReceivedRunnables.clear();
		m_FinishedAccess.unlock();

		m_ThreadAccess.lock();
		for (ThreadList::iterator it=m_Threads.begin(); it!=m_Threads.end(); /**/)
		{
			if ((*it)->join())
			{
				delete *it;
			}

			it = m_Threads.erase(it);
		}
		m_ThreadAccess.unlock();
	}

	void ThreadPool::update(long &time)
	{
#ifdef _2REAL_DEBUG
		m_Elapsed += time;
		if (m_Elapsed >= 10000000)
		{
			m_FinishedAccess.lock();
			size_t finished = m_FinishedRunnables.size();
			size_t received = m_ReceivedRunnables.size();
			m_FinishedAccess.unlock();

			m_ReadyAccess.lock();
			size_t ready = m_ReadyRunnables.size();
			m_ReadyAccess.unlock();

			m_ExecutingAccess.lock();
			size_t executing = m_ExecutingRunnables.size();
			m_ExecutingAccess.unlock();

			m_AbortedAccess.lock();
			size_t aborted = m_AbortedRunnables.size();
			m_AbortedAccess.unlock();

			size_t reallyRunning = executing - received - finished;

			std::cout << "ready: " << ready << std::endl;
			std::cout << "executing: " << reallyRunning << std::endl;
			std::cout << "aborted: " << aborted << std::endl;
			m_Elapsed = 0;
		}
#endif
		executeCleanUp();
	}

	void ThreadPool::executeCleanUp()
	{
		m_FinishedAccess.lock();
		m_FinishedRunnables.splice(m_FinishedRunnables.begin(), m_ReceivedRunnables, m_ReceivedRunnables.begin(), m_ReceivedRunnables.end());
		m_FinishedAccess.unlock();

		for (std::list< unsigned int >::iterator it = m_FinishedRunnables.begin(); it != m_FinishedRunnables.end(); /**/)
		{
			m_ExecutingAccess.lock();
			RunnableMap::iterator r = m_ExecutingRunnables.find(*it);
			if (r != m_ExecutingRunnables.end())
			{
				FunctionBlockStateManager *service = r->second;
				m_ExecutingRunnables.erase(r);
				m_ExecutingAccess.unlock();

				service->finishUpdate();
			}
			else
			{
				m_ExecutingAccess.unlock();
				
				m_AbortedAccess.lock();
				RunnableMap::iterator r = m_AbortedRunnables.find(*it);
				if (r != m_AbortedRunnables.end())
				{
					FunctionBlockStateManager *service = r->second;
					m_AbortedRunnables.erase(r);
					m_AbortedAccess.unlock();

#ifdef _2REAL_DEBUG
					std::cout << "threadpool finishing aborted service" << std::endl;
#endif
					service->finishUpdate();
					if ( service->shutDown( 10000 ) )
					{
						delete service;
					}
				}
				else
				{
					m_AbortedAccess.unlock();
				}
			}

			it = m_FinishedRunnables.erase(it);
		}

		PooledThread *thread = nullptr;
		m_ThreadAccess.lock();
		while ((thread = tryGetFreeThread()) != nullptr)
		{
			m_ReadyAccess.lock();
			while (!m_ReadyRunnables.empty() && m_ReadyRunnables.front() == nullptr)
			{
				m_ReadyRunnables.pop_front();
			}

			if (m_ReadyRunnables.empty())
			{
				m_ReadyAccess.unlock();
				break; //thread acc. is unlocked outside of while loop
			}
			else
			{
				FunctionBlockStateManager *service = m_ReadyRunnables.front();
				service->beginUpdate();
				
				m_ReadyRunnables.pop_front();
				m_ReadyAccess.unlock();

				thread->reactivate();
				m_ThreadAccess.unlock();

				m_ExecutingAccess.lock();
				m_ExecutingRunnables.insert(NamedRunnable(service->getId(), service));
				m_ExecutingAccess.unlock();
				
				thread->run( Poco::Thread::PRIO_NORMAL, *service );
			}

			m_ThreadAccess.lock();
		}
		m_ThreadAccess.unlock();
	}

	void ThreadPool::serviceIsFinished(FunctionBlockStateManager &s)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_FinishedAccess);
		m_ReceivedRunnables.push_back(s.getId());
	}

	void ThreadPool::abortService(FunctionBlockStateManager &s)
	{
		m_ExecutingAccess.lock();
		RunnableMap::iterator it = m_ExecutingRunnables.find(s.getId());
		if (it != m_ExecutingRunnables.end())
		{
			m_ExecutingRunnables.erase(it);
			m_ExecutingAccess.unlock();

			Poco::ScopedLock< Poco::FastMutex > lock(m_AbortedAccess);
			m_AbortedRunnables.insert(NamedRunnable(s.getId(), &s));
		}
		else
		{
			m_ExecutingAccess.unlock();
		}
	}

	void ThreadPool::scheduleService(FunctionBlockStateManager &s)
	{
		//m_ThreadAccess.lock();
		//PooledThread *thread = tryGetFreeThread();
		//if (thread != nullptr)
		//{
		//	s.beginExecution();

		//	thread->reactivate();
		//	m_ThreadAccess.unlock();

		//	m_ExecutingAccess.lock();
		//	m_ExecutingRunnables.insert(NamedRunnable(s.getId(), &s));
		//	m_ExecutingAccess.unlock();

		//	thread->run(Poco::Thread::PRIO_NORMAL, s);
		//}
		//else
		//{
			//m_ThreadAccess.unlock();
			Poco::ScopedLock< Poco::FastMutex > lock( m_ReadyAccess );
			m_ReadyRunnables.push_back( &s );
		//}
	}

	const bool ThreadPool::unscheduleService(FunctionBlockStateManager &s)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_ReadyAccess);
		RunnableDeque::iterator it = std::find< RunnableDeque::iterator, FunctionBlockStateManager * >(m_ReadyRunnables.begin(), m_ReadyRunnables.end(), &s);
		if (it != m_ReadyRunnables.end())
		{
			*it = nullptr;
			return true;
		}

		return false;
	}

	PooledThread * ThreadPool::tryGetFreeThread()
	{
		for (ThreadList::iterator it = m_Threads.begin(); it != m_Threads.end(); ++it)
		{
			if ((*it)->isIdle())
			{
				return *it;
			}
		}

		return nullptr;
	}

}