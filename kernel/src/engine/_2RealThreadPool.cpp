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
#include "engine/_2RealLogger.h"
#include "helpers/_2RealCallback.h"
#include "helpers/_2RealEvent.h"

#include <sstream>
#include <algorithm>
#include <assert.h>

namespace _2Real
{

	ThreadPool::ThreadPool( EngineImpl &engine, const unsigned int capacity, const unsigned int stackSize, std::string const& name ) :
		m_Timer( engine.getTimer() ),
		m_Logger( engine.getLogger() ),
		m_Name( name ),
		m_StackSize( stackSize ),
		m_Elapsed( 0 )
	{
		AbstractCallback< long > *callback = new MemberCallback< ThreadPool, long >( *this, &ThreadPool::update );
		m_Timer.registerToTimerSignal( *callback );

		for ( unsigned int i=0; i<capacity; ++i )
		{
			PooledThread *thread = new PooledThread( *this, m_StackSize );
			ThreadQueue q;
			q.isReserved = false;
			q.isUnique = false;
			m_Threads[ thread ] = q;
		}

		//for ( unsigned int i=0; i<capacity; ++i )
		//{
		//	PooledThread *thread = new PooledThread( *this, m_StackSize );
		//	ThreadQueue q;
		//	q.isReserved = true;
		//	q.isUnique = false;
		//	m_Threads[ thread ] = q;
		//}
	}

	ThreadPool::~ThreadPool()
	{
		AbstractCallback< long > *callback = new MemberCallback< ThreadPool, long >( *this, &ThreadPool::update );
		m_Timer.unregisterFromTimerSignal( *callback );

		m_ThreadQueuesAccess.lock();
		for ( ThreadQueueIterator it = m_Threads.begin(); it != m_Threads.end(); ++it )
		{
			if ( ( it->first )->join() )
			{
				delete it->first;
			}
		}
		m_ThreadQueuesAccess.unlock();

		m_RequestQueueAccess.lock();
		for ( RequestIterator it = m_RequestQueue.begin(); it != m_RequestQueue.end(); ++it )
		{
			delete *it;
		}
		m_RequestQueueAccess.unlock();
	}

	void ThreadPool::update( long time )
	{
		m_Elapsed += time;
		if ( m_Elapsed >= 5000000 )
		{
		//	m_FinishedAccess.lock();
		//	size_t finished = m_FinishedBuffer.size();
		//	size_t received = m_FinishedBlocks.size();
		//	m_FinishedAccess.unlock();

		//	m_ReadyAccess.lock();
		//	size_t ready = m_ReadyBlocks.size();
		//	m_ReadyAccess.unlock();

		//	m_ExecutingAccess.lock();
		//	size_t executing = m_ExecutingBlocks.size();
		//	m_ExecutingAccess.unlock();

		//	m_AbortedAccess.lock();
		//	size_t aborted = m_AbortedBlocks.size();
		//	m_AbortedAccess.unlock();

		//	size_t reallyRunning = executing - received - finished;

		//	std::ostringstream msg;
		//	msg << std::endl;
		//	msg << "-------------------------------------------------------------------------\n";
		//	msg << "THREADPOOL: ready: " << ready << std::endl;
		//	msg << "THREADPOOL: executing: " << reallyRunning << std::endl;
		//	msg << "THREADPOOL: aborted: " << aborted << std::endl;
		//	msg << "-------------------------------------------------------------------------";
		//	m_Logger.addLine( msg.str() );

			m_Elapsed = 0;
		}

		executeCleanUp();
	}

	void ThreadPool::executeCleanUp()
	{
		m_ThreadQueuesAccess.lock();

		for ( ThreadQueueIterator it = m_Threads.begin(); it != m_Threads.end(); ++it )
		{
			if ( ( it->first )->isIdle() )
			{
				if ( it->second.isUnique )
				{
					if ( !it->second.localQueue.empty() )
					{
						ThreadExecRequest *req = it->second.localQueue.front();
						it->second.localQueue.pop_front();
						it->first->reactivate();
						it->first->run( Poco::Thread::PRIO_NORMAL, *req );
					}
				}
				else
				{
					m_RequestQueueAccess.lock();
					if ( m_RequestQueue.empty() )
					{
						m_RequestQueueAccess.unlock();
						continue;															// thread queue acc. is unlocked outside of while loop
					}
					else
					{
						ThreadExecRequest *req = m_RequestQueue.front();
						m_RequestQueue.pop_front();
						m_RequestQueueAccess.unlock();

						it->first->reactivate();
						it->first->run( Poco::Thread::PRIO_NORMAL, *req );
					}
				}
				//else if ( it->second.isReserved && !it->second.localQueue.empty() )		// if reserved, favor own queue.. else, use common requests
				//{
				//	std::cout << "free reserved thread!" << std::endl;
				//}
				//else
				//{
				//	m_RequestQueueAccess.lock();
				//	if ( m_RequestQueue.empty() )
				//	{
				//		m_RequestQueueAccess.unlock();
				//		continue;															// thread queue acc. is unlocked outside of while loop
				//	}
				//	else
				//	{
				//		ThreadExecRequest *req = m_RequestQueue.front();
				//		m_RequestQueue.pop_front();
				//		m_RequestQueueAccess.unlock();

				//		it->first->reactivate();
				//		it->first->run( Poco::Thread::PRIO_NORMAL, *req );
				//	}
				//}
			}
		}
		m_ThreadQueuesAccess.unlock();
	}

	void ThreadPool::scheduleRequest( ThreadExecRequest &request, PooledThread *thread )
	{
		if ( thread == nullptr )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_RequestQueueAccess );
			m_RequestQueue.push_back( &request );
		}
		else
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_ThreadQueuesAccess );
			ThreadQueueIterator it = m_Threads.find( thread );
			it->second.localQueue.push_back( &request );
		}
	}

	PooledThread * ThreadPool::requestUniqueThread()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ThreadQueuesAccess );

		PooledThread *thread = new PooledThread( *this, m_StackSize );
		ThreadQueue q;
		q.isReserved = false;
		q.isUnique = true;
		m_Threads[ thread ] = q;
		return thread;
	}
}