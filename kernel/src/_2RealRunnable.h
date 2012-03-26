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

#include <sstream>
#include <map>
#include <list>

namespace _2Real
{

	/**
	*	services, sequences and synchronizations are runnable
	*/


	class Graph;
	class SystemImpl;
	class RunnableGraph;
	class PooledThread;
	class RunnableManager;
	class InputSlot;

	typedef std::list< RunnableManager * >				RunnableList;
	typedef std::map< std::string, InputSlot * >		InputMap;

	class Runnable : public Poco::Runnable, public Entity
	{

	public:

		Runnable(Identifier const& id, SystemImpl &system);
		virtual ~Runnable();

		virtual void handleException();
		virtual void setup() = 0;
		virtual void run() = 0;
		virtual void shutdown() = 0;

		virtual void resetData() = 0;

		Runnable & root();
		Runnable const&  root() const;

		void setFather(Graph &father);
		Graph &father();
		Graph const& father() const;

		SystemImpl &system();
		SystemImpl const& system() const;

		void setUpdateRate(float updatesPerSecond);
		long getMaxDelay();

		virtual RunnableList const&	getChildren() const = 0;
		virtual InputMap const&		getInlets() const = 0;

		virtual void prepareForAbort() = 0;

	protected:

		SystemImpl				&m_System;
		Graph					*m_Father;

	private:

		long					m_MaxDelay;
		float					m_UpdatesPerSecond;

	};

	inline long Runnable::getMaxDelay()
	{
		return m_MaxDelay;
	}

	inline void Runnable::setFather(Graph &father)
	{
		m_Father = &father;
	}

	inline Graph & Runnable::father()
	{
		return *m_Father;
	}

	inline Graph const& Runnable::father() const
	{
		return *m_Father;
	}

	inline SystemImpl & Runnable::system()
	{
		return m_System;
	}

	inline SystemImpl const& Runnable::system() const
	{
		return m_System;
	}

	inline void Runnable::setUpdateRate(float updatesPerSecond)
	{
		m_MaxDelay = long(1000.0f/updatesPerSecond);
		m_UpdatesPerSecond = updatesPerSecond;
	}

}