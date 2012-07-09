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

#include <iostream>
#include <algorithm>

namespace _2Real
{

	ThreadPool::ThreadPool( EngineImpl &engine, const unsigned int capacity, const unsigned int stackSize, std::string const& name ) :
		m_StackSize( stackSize ),
		m_Name( name ),
		m_Timer( engine.getTimer() ),
		m_Logger( engine.getLogger() ),
		m_Elapsed(0)
	{
		AbstractCallback< long > *callback = new MemberCallback< ThreadPool, long >( *this, &ThreadPool::update );
		m_Timer.registerToTimerSignal( *callback );

		for ( unsigned int i=0; i<capacity; ++i )
		{
			ThreadPoolCallback *callback = new ThreadPoolCallback( *this );
			PooledThread *thread = new PooledThread( *callback, m_StackSize );
			m_Threads.push_back( thread );
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
		m_ReadyBlocks.clear();
		m_ReadyAccess.unlock();

		m_ExecutingAccess.lock();
		m_ExecutingBlocks.clear();
		m_ExecutingAccess.unlock();

		m_AbortedAccess.lock();
		m_AbortedBlocks.clear();
		m_AbortedAccess.unlock();

		m_FinishedAccess.lock();
		m_FinishedBlocks.clear();
		m_FinishedBuffer.clear();
		m_FinishedAccess.unlock();

		m_ThreadAccess.lock();
		for ( ThreadIterator it = m_Threads.begin(); it != m_Threads.end(); /**/ )
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
		m_Elapsed += time;
		if ( m_Elapsed >= 5000000 )
		{
			m_FinishedAccess.lock();
			size_t finished = m_FinishedBuffer.size();
			size_t received = m_FinishedBlocks.size();
			m_FinishedAccess.unlock();

			m_ReadyAccess.lock();
			size_t ready = m_ReadyBlocks.size();
			m_ReadyAccess.unlock();

			m_ExecutingAccess.lock();
			size_t executing = m_ExecutingBlocks.size();
			m_ExecutingAccess.unlock();

			m_AbortedAccess.lock();
			size_t aborted = m_AbortedBlocks.size();
			m_AbortedAccess.unlock();

			size_t reallyRunning = executing - received - finished;

			std::ostringstream msg;
			msg << std::endl;
			msg << "-------------------------------------------------------------------------\n";
			msg << "THREADPOOL: ready: " << ready << std::endl;
			msg << "THREADPOOL: executing: " << reallyRunning << std::endl;
			msg << "THREADPOOL: aborted: " << aborted << std::endl;
			msg << "-------------------------------------------------------------------------";
			m_Logger.addLine( msg.str() );

			m_Elapsed = 0;
		}

		executeCleanUp();
	}

	void ThreadPool::executeCleanUp()
	{
		m_FinishedAccess.lock();
		m_FinishedBuffer.splice( m_FinishedBuffer.begin(), m_FinishedBlocks, m_FinishedBlocks.begin(), m_FinishedBlocks.end() );
		m_FinishedAccess.unlock();

		for ( BufferedBlockIterator it = m_FinishedBuffer.begin(); it != m_FinishedBuffer.end(); /**/ )
		{
			m_ExecutingAccess.lock();
			BlockIterator bIt = m_ExecutingBlocks.find( *it );
			if ( bIt != m_ExecutingBlocks.end() )
			{
				FunctionBlockStateManager *block = *bIt;
				m_ExecutingBlocks.erase( bIt );
				m_ExecutingAccess.unlock();

				block->finishUpdate();
			}
			else
			{
				m_ExecutingAccess.unlock();
				
				m_AbortedAccess.lock();
				BlockIterator bIt = m_AbortedBlocks.find( *it );
				if ( bIt != m_AbortedBlocks.end() )
				{
					FunctionBlockStateManager *block = *bIt;
					m_AbortedBlocks.erase( bIt );
					m_AbortedAccess.unlock();

					m_Logger.addLine( "THREADPOOL: finishing aborted block" );

					block->finishUpdate();
					if ( block->shutDown( 10000 ) )
					{
						delete block;
					}
				}
				else
				{
					m_AbortedAccess.unlock();
				}
			}

			it = m_FinishedBuffer.erase( it );
		}

		PooledThread *thread = nullptr;
		m_ThreadAccess.lock();
		while ( ( thread = tryGetFreeThread() ) != nullptr )
		{
			m_ReadyAccess.lock();
			while ( !m_ReadyBlocks.empty() && m_ReadyBlocks.front() == nullptr )
			{
				m_ReadyBlocks.pop_front();
			}

			if ( m_ReadyBlocks.empty() )
			{
				m_ReadyAccess.unlock();
				break; // thread acc. is unlocked outside of while loop
			}
			else
			{
				FunctionBlockStateManager *block = m_ReadyBlocks.front();
				block->beginUpdate();
				
				m_ReadyBlocks.pop_front();
				m_ReadyAccess.unlock();

				thread->reactivate();
				m_ThreadAccess.unlock();

				m_ExecutingAccess.lock();
				m_ExecutingBlocks.insert( block );
				m_ExecutingAccess.unlock();
				
				thread->run( Poco::Thread::PRIO_NORMAL, *block );
			}
			m_ThreadAccess.lock();
		}
		m_ThreadAccess.unlock();
	}

	void ThreadPool::serviceIsFinished( FunctionBlockStateManager &mgr )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_FinishedAccess );
		m_FinishedBlocks.push_back( &mgr );
	}

	void ThreadPool::abortService( FunctionBlockStateManager &mgr )
	{
		m_ExecutingAccess.lock();
		BlockIterator it = m_ExecutingBlocks.find( &mgr );
		if ( it != m_ExecutingBlocks.end() )
		{
			m_ExecutingBlocks.erase( it );
			m_ExecutingAccess.unlock();

			Poco::ScopedLock< Poco::FastMutex > lock( m_AbortedAccess );
			m_AbortedBlocks.insert( &mgr );
		}
		else
		{
			m_ExecutingAccess.unlock();
		}
	}

	void ThreadPool::scheduleService( FunctionBlockStateManager &mgr )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ReadyAccess );
		m_ReadyBlocks.push_back( &mgr );
	}

	PooledThread * ThreadPool::tryGetFreeThread()
	{
		for ( ThreadIterator it = m_Threads.begin(); it != m_Threads.end(); ++it )
		{
			if ( (*it)->isIdle() )
			{
				return *it;
			}
		}

		return nullptr;
	}

}