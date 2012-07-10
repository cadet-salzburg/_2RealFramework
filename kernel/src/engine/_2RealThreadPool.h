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

namespace _2Real
{

	class EngineImpl;
	class PooledThread;
	class FunctionBlockStateManager;
	class Timer;
	class Logger;

	class ThreadPool
	{

	public:

		ThreadPool( EngineImpl &engine, const unsigned int capacity, const unsigned int stackSize, std::string const& name );
		~ThreadPool();

		void clear();
		void update( long &time );

		void scheduleService( FunctionBlockStateManager &s );
		void serviceIsFinished( FunctionBlockStateManager &s );
		void abortService( FunctionBlockStateManager &s );
		void executeCleanUp();

	private:

		typedef std::list< PooledThread * >									Threads;
		typedef std::list< PooledThread * >::iterator						ThreadIterator;
		typedef std::list< PooledThread * >::const_iterator					ThreadConstIterator;

		typedef std::list< FunctionBlockStateManager * >					BlockBuffer;
		typedef std::list< FunctionBlockStateManager * >::iterator			BufferedBlockIterator;
		typedef std::list< FunctionBlockStateManager * >::const_iterator	BufferedBlockConstIterator;

		typedef std::set< FunctionBlockStateManager * >						Blocks;
		typedef std::set< FunctionBlockStateManager * >::iterator			BlockIterator;
		typedef std::set< FunctionBlockStateManager * >::const_iterator		BlockConstIterator;

		PooledThread * tryGetFreeThread();

		Timer								&m_Timer;
		Logger								&m_Logger;

		std::string							m_Name;
		unsigned int						m_StackSize;

		Threads								m_Threads;

		BlockBuffer							m_ReadyBlocks;
		BlockBuffer							m_FinishedBlocks;
		BlockBuffer							m_FinishedBuffer;

		Blocks								m_ExecutingBlocks;
		Blocks								m_AbortedBlocks;

		mutable Poco::FastMutex				m_ThreadAccess;
		mutable Poco::FastMutex				m_ExecutingAccess;
		mutable Poco::FastMutex				m_AbortedAccess;
		mutable Poco::FastMutex				m_ReadyAccess;
		mutable Poco::FastMutex				m_FinishedAccess;

		long								m_Elapsed;

	};

	class ThreadPoolCallback
	{

	public:

		ThreadPoolCallback( ThreadPool &pool ) : m_ThreadPool(pool) {}
		void invoke( FunctionBlockStateManager &s ) { m_ThreadPool.serviceIsFinished(s); }

	private:

		ThreadPool		&m_ThreadPool;

	};

}