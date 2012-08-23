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

#include "helpers/_2RealPoco.h"

#include <list>
#include <set>
#include <string>
#include <deque>
#include <map>

namespace _2Real
{

	class EngineImpl;
	class PooledThread;
	class FunctionBlockStateManager;
	class Timer;
	class Logger;

	struct ThreadExecRequest;

	class ThreadPool
	{

	public:

		ThreadPool( EngineImpl &engine, const unsigned int capacity, const unsigned int stackSize, std::string const& name );
		~ThreadPool();

		PooledThread * requestUniqueThread();

		void update( long &time );
		void scheduleRequest( ThreadExecRequest &request, PooledThread *thread );
		void executeCleanUp();

	private:


		typedef std::deque< ThreadExecRequest * >							RequestQueue;
		typedef std::deque< ThreadExecRequest * >::iterator					RequestIterator;
		typedef std::deque< ThreadExecRequest * >::const_iterator			RequestConstIterator;

		struct ThreadQueue
		{
			RequestQueue		localQueue;
			bool				isReserved;
			bool				isUnique;
		};

		typedef std::map< PooledThread *, ThreadQueue >						ThreadQueues;
		typedef std::map< PooledThread *, ThreadQueue >::iterator			ThreadQueueIterator;
		typedef std::map< PooledThread *, ThreadQueue >::const_iterator		ThreadQueueConstIterator;

		Timer								&m_Timer;
		Logger								&m_Logger;

		std::string							m_Name;
		unsigned int						m_StackSize;

		ThreadQueues						m_Threads;
		RequestQueue						m_RequestQueue;

		mutable Poco::FastMutex				m_ThreadQueuesAccess;
		mutable Poco::FastMutex				m_RequestQueueAccess;

		long								m_Elapsed;

	};

}