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
#include "_2RealEntity.h"

#include "Poco/Runnable.h"
#include "Poco/Mutex.h"

#include <sstream>
#include <map>

namespace _2Real
{

	/**
	*	services, sequences and synchronizations are runnable
	*/

	class Graph;
	class SystemGraph;
	class RunnableGraph;
	class PooledThread;

	class Runnable : public Poco::Runnable, public Entity
	{

	public:

		Runnable(Identifier const& id, SystemGraph &system);
		virtual ~Runnable();

		/**
		*	placeholder for now
		*/
		virtual void handleException();

		/**
		*	initialized everything
		*/
		virtual void setup() = 0;

		/**
		*	service update for services / child list update for runnable graphs
		*/
		virtual void run() = 0;

		/**
		*	guess what
		*/
		virtual void shutdown() = 0;

		/**
		*	must be called upon insertion
		*/
		void setFather(Graph &father);

		/**
		*	true if system == father
		*/
		bool isRoot() const;

		Runnable & root();
		Runnable const&  root() const;

		Graph &father();
		Graph const& father() const;

		SystemGraph &system();
		SystemGraph const& system() const;

		void setUpdateRate(float updatesPerSecond);
		long getMaxDelay() const;

	private:

		SystemGraph					&m_System;

		/**
		*	necessary for delay / father / root access
		*/
		mutable Poco::FastMutex		m_Mutex;

		/**
		*	may be identical to the system
		*/
		Graph						*m_Father;

		/**
		*	max amount that threads will be suspended when running this runnable
		*/
		long						m_MaxDelay;

	};

}