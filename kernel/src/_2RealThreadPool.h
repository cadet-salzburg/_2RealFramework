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

#pragma once

#include "_2RealException.h"

#include <deque>
#include <list>
#include <map>
#include <iostream>
#include <string>

#include "Poco/Mutex.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"

namespace _2Real
{

	/**
	*	within the framework, the threadpool is the first subsystem to be created, and the last to be deleted!
	*/

	class PooledThread;
	class RunnableManager;
	class Timer;

	typedef std::list< PooledThread * >						ThreadList;
	typedef std::deque< RunnableManager * >					RunnableDeque;
	typedef std::map< unsigned int, RunnableManager * >		RunnableMap;
	typedef std::pair< unsigned int, RunnableManager * >	NamedRunnable;

	class ThreadPool
	{

	public:

		ThreadPool(const unsigned int capacity, const unsigned int stackSize, std::string const& name);
		~ThreadPool();

		/**
		*	kills all threads
		*/
		void clear();

		/**
		*	the threadpool regularly receives a timer signal
		*/
		void update(long &time);

		/**
		*	tries to execute a runnable
		*/
		void scheduleRunnable(RunnableManager &runnable);

		/**
		*	unschedules scheduled runnable
		*/
		const bool unscheduleRunnable(RunnableManager &runnable);

		/**
		*	signals that a runnable is finished
		*/
		void runnableIsFinished(RunnableManager &runnable);

		/**
		*	kill a runnable
		*/
		void abortRunnable(RunnableManager &runnable, _2Real::Exception &e);

		/**
		*	yay
		*/
		void executeCleanUp();

		void registerTimeListener(_2Real::Timer &timer);
		void unregisterTimeListener(_2Real::Timer &timer);

	private:

		PooledThread * tryGetFreeThread();

		/**
		*	name (for debugging)
		*/
		std::string							m_Name;

		/*
		*	default stack size for the threads
		*/
		unsigned int						m_StackSize;

		/**
		*	the pooled threads
		*/
		ThreadList							m_Threads;

		/**
		*	runnables waiting for a thread
		*/
		RunnableDeque						m_ReadyRunnables;

		std::list< unsigned int >			m_ReceivedRunnables;
		std::list< unsigned int >			m_FinishedRunnables;

		/**
		*	runnables in execution
		*/
		RunnableMap							m_ExecutingRunnables;

		/**
		*	runnables that were aborted
		*/
		RunnableMap							m_AbortedRunnables;

		/**
		*	sync stuff
		*/

		/**
		*	access to the pooled threads
		*/
		mutable Poco::FastMutex				m_ThreadAccess;

		/**
		*	access to executing runnables
		*/
		mutable Poco::FastMutex				m_ExecutingAccess;

		/**
		*	access to aborted runnables
		*/
		mutable Poco::FastMutex				m_AbortedAccess;

		/**
		*	access to ready runnables
		*/
		mutable Poco::FastMutex				m_ReadyAccess;

		/**
		*	access to finished runnables
		*/
		mutable Poco::FastMutex				m_FinishedAccess;

#ifdef _2REAL_DEBUG
		long								m_Elapsed;
#endif

	};

	/**
	*	maybe I'll just give the pooled threads references to their pool, instead of this
	*/
	class ThreadPoolCallback
	{

	public:

		ThreadPoolCallback(ThreadPool &pool) : m_ThreadPool(pool) {}

		void invoke(RunnableManager &runnable)
		{
			m_ThreadPool.runnableIsFinished(runnable);
		}

	private:

		ThreadPool		&m_ThreadPool;

	};

}