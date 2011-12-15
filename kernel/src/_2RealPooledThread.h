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

#include "_2RealRunnable.h"
#include "_2RealException.h"

#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"

#include <ctime>

namespace _2Real
{

	class PooledThread : public Poco::Runnable
	{
	
	public:

		PooledThread(unsigned int stackSize = POCO_THREAD_STACK_SIZE);

		void start();
		void start(Poco::Thread::Priority const& priority, _2Real::Runnable &target);

		bool isIdle() const;
		int idleTime() const;

		void stopAndJoin();
		void join();
		void stop();

		void activate();
		void kill();

		void run();

		const Identifier identifier() const;

	private:

		volatile bool				m_IsIdle;
		volatile std::time_t		m_IdleTime;
		_2Real::Runnable			*m_Target;
		Poco::Thread				m_Thread;
		Poco::Event					m_TargetReady;
		Poco::Event					m_TargetCompleted;
		Poco::Event					m_Started;
		mutable Poco::FastMutex		m_Mutex;

	};

}