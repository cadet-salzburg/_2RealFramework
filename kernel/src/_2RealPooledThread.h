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
#include "Poco/Timestamp.h"

#include <ctime>

namespace _2Real
{

	class PooledThread : public Poco::Runnable
	{
	
	public:

		PooledThread(unsigned int stackSize = POCO_THREAD_STACK_SIZE);

		/**
		*	starts a new target
		*/
		void start(Poco::Thread::Priority const& priority, _2Real::Runnable &target, bool runOnce = false);

		/**
		*	blocks until the thread has finished its target's current update
		*/
		void waitForTarget();

		/**
		*	blocks until the thread has exited from it's run method
		*/
		void joinThread();

		/**
		*	returns true if the thread is still running, but currently has no target
		*/
		bool isIdle() const;

		/**
		*	signals to the thread that it should exit from its run function
		*/
		void stopRunning();

		/**
		*	signals to the thread that it should no longer keep updating its target
		*	--> after the next update, it will cleanup & wait for a new target
		*/
		void stopTargetRunning();

		/**
		*	kills the thread
		*/
		void kill();

		/**
		*	used to reactivate a thread after its target has finished
		*/
		void reactivate();

		/**
		*	the thread's run function
		*/
		void run();

	private:

		/**
		*	queries if the thread should continue running
		*/
		bool keepRunning() const;

		/**
		*	queries if the thread should contiue updating the target
		*/
		bool keepTargetRunning() const;

		/**
		*	cleanup function after the thread has its target reset
		*/
		void cleanUp();

		volatile bool				m_RunThread;
		volatile bool				m_RunTarget;
		volatile bool				m_RunTargetOnce;
		volatile bool				m_IsIdle;

		_2Real::Runnable			*m_Target;
		Poco::Thread				m_Thread;
		Poco::Event					m_TargetReady;
		Poco::Event					m_TargetCompleted;
		Poco::Event					m_ThreadStarted;
		Poco::Event					m_ThreadStopped;
		mutable Poco::Mutex			m_Mutex;
		Poco::Timestamp				m_Timer;

	};

}