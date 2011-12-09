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

#include <list>

#include "Poco/Mutex.h"

namespace _2Real
{

	class Runnable;
	class PooledThread;
	class Identifier;

	class ThreadPool
	{

	public:

		ThreadPool(unsigned int capacity, unsigned int idleTime, unsigned int stackSize, std::string const& name);
		~ThreadPool();

		void start(Runnable &target, bool runOnce);
		void stopAll();
		void joinAll();

		//stops the runnable & joins the thread
		void stop(Identifier const& runnable);

		bool isRunning(Identifier const& runnable);
		PooledThread & find(Identifier const& runnable);

		//shutdown function
		void clearThreads();

	private:

		PooledThread & getFreeThread();
		void housekeep();

		typedef std::list< PooledThread * >	ThreadList;
		ThreadList							m_Threads;
		std::string							m_Name;
		unsigned int						m_Capacity;
		unsigned int						m_IdleTime;
		unsigned int						m_StackSize;
		mutable Poco::FastMutex				m_Mutex;
		unsigned int						m_Age;
		bool								m_IsUpdated;

	};

}