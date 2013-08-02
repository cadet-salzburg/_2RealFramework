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

#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	class ThreadPool;
	class FunctionBlockStateManager;
	struct ThreadExecRequest;

	class PooledThread : public Poco::Runnable
	{
	
	public:

		enum Action
		{
			SETUP, UPDATE, SHUTDOWN
		};

		PooledThread( ThreadPool &pool, unsigned int stackSize = POCO_THREAD_STACK_SIZE );

		const bool isIdle() const;
		const bool join();

		void run();
		void run( Poco::Thread::Priority const& priority, ThreadExecRequest &request );
		void reactivate();

		const bool operator<( PooledThread const& rhs ) const;

	private:

		ThreadExecRequest			*m_Request;
		volatile bool				m_IsIdle;
		Poco::Thread				m_Thread;

		ThreadPool					*m_ThreadPool;

		Poco::Event					m_TargetReady;
		Poco::Event					m_TargetCompleted;
		Poco::Event					m_ThreadStarted;
		Poco::Event					m_ThreadStopped;
		mutable Poco::FastMutex		m_Mutex;

	};

}