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

#include <string>
#include <map>

#include "_2RealRunnableTriggers.h"
#include "_2RealCallbacks.h"

#include "Poco/BasicEvent.h"
#include "Poco/Mutex.h"

namespace _2Real
{

	enum RunnableStateName
	{
		RUNNABLE_CREATED,		//right after the ctor was called
		RUNNABLE_SETUP,			//setup was called
		RUNNABLE_READY,			//triggered -> waiting for thread
		RUNNABLE_UPDATING,		//calling update
		RUNNABLE_SHUTDOWN,		//shut down
		RUNNABLE_ERROR			//in error state
	};

	class Runnable;
	class Exception;
	class AbstractRunnableState;
	class Identifier;
	class PooledThread;
	class ThreadPool;
	class IStateChangeListener;
	class SystemImpl;
	class UpdateTriggersImpl;
	class AbstractThreadCallback;

	class RunnableManager
	{

	public:

		RunnableManager(Runnable &runnable, UpdateTriggersImpl &triggers);
		~RunnableManager();

		Runnable & getManagedRunnable() const;
		Identifier const& getManagedId() const;
		std::string const& getManagedName() const;

		void setUp();
		void getReady();
		const bool beginUpdate();
		void finishUpdate();
		bool isSetUp() const;
		void abort(_2Real::Exception &e);

		/**
		*	disables all triggers
		*/
		void prepareForShutDown();

		/**
		*	unlinks everything
		*/
		void prepareForAbort();

		/**
		*	performs the shut down, flags the event once done
		*/
		const bool shutDown();

		unsigned int getState();

		void registerToStateChange(StateChangeCallback callback);
		void unregisterFromStateChange(StateChangeCallback callback);
		void registerToStateChange(IStateChangeListener &listener);
		void unregisterFromStateChange(IStateChangeListener &listener);
		void registerToStateChange(RunnableTriggers &triggers);
		void unregisterFromStateChange(RunnableTriggers &triggers);

		void debug();

	private:

		void handleStateChangeException(_2Real::Exception &e);

		mutable Poco::FastMutex						m_Mutex;
		Runnable									*m_Runnable;
		AbstractRunnableState						*m_CurrentState;
		Poco::BasicEvent< unsigned int >			m_StateChangeEvent;
		RunnableTriggers							m_Triggers;

		//todo: make clear func for triggers
		bool										m_Waiting;
		bool										m_StopRunning;
		Poco::Event									m_StopEvent;

		/**
		*	ref to the engine's thread pool
		*/
		ThreadPool									&m_Threads;
		SystemImpl									&m_System;

		long										m_LastUpdateInitialized;
		long										m_LastUpdateFinalized;
		long										m_LastTriggersReady;
		long										m_ShutDownReceived;

	};

}