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

#include "Poco/BasicEvent.h"

#define STATE_CREATED	"created"
#define STATE_SETUP		"set up"
#define STATE_RUNNING	"running"
#define STATE_UPDATING	"updating"
#define STATE_SHUTDOWN	"shut down"
#define STATE_ERROR		"halted"

namespace _2Real
{

	class Runnable;
	class RunnableState;
	class Identifier;
	class PooledThread;

	typedef std::map< std::string, RunnableState * >	StateTable;

	class IStateChangeListener;
	typedef void (*StateChangeCallback)(std::string &state);

	class RunnableManager
	{

	public:

		RunnableManager(Runnable &runnable);
		~RunnableManager();

		RunnableState & getState(std::string const& stateName) const;
		Runnable & getManagedRunnable() const;
		Identifier const& getManagedId() const;
		
		void setup();
		void shutdown();
		void startRunning(PooledThread &thread);
		void stopRunning();
		void beginUpdate(PooledThread &thread);
		void endUpdate();
		void handleException();

		/**
		*	this functions are for regsitering state change callbacks
		*/
		void registerStateChangeCallback(StateChangeCallback callback);
		void unregisterStateChangeCallback(StateChangeCallback callback);
		void registerStateChangeListener(IStateChangeListener &listener);
		void unregisterStateChangeListener(IStateChangeListener &listener);

	private:

		Runnable									*m_Runnable;
		StateTable									m_RunnableStates;
		RunnableState								*m_CurrentState;
		PooledThread								*m_Thread;
		Poco::BasicEvent< std::string >				m_StateChangeEvent;

	};

	inline Runnable& RunnableManager::getManagedRunnable() const
	{
		return *m_Runnable;
	}

}