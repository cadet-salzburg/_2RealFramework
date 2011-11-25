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
#include "_2RealException.h"

#include "Poco/Runnable.h"

namespace _2Real
{

	/**
	*	services, sequences and synchronizations are runnable
	*/

	class Graph;
	class SystemGraph;
	class RunnableGraph;

	class Runnable : public Poco::Runnable, public Entity
	{

	public:

		Runnable(Identifier const& id, SystemGraph *const system) :
			Entity(id),
			m_System(system)
		{
		}

		virtual ~Runnable()
		{
		}

		void start(bool const& runOnce)
		{
			m_RunOnce = runOnce;
			m_Run = !runOnce;
		}

		void stop()
		{
			m_RunOnce = false;
			m_Run = false;
		}

		virtual void run() = 0;
		virtual void update() = 0;
		virtual void shutdown() = 0;
		virtual void checkConfiguration() = 0;

		virtual const bool hasParameters() const = 0;

		RunnableGraph *const root()
		{
			return m_Root;
		}

		RunnableGraph const *const root() const
		{
			return m_Root;
		}

		void setFather(Graph *const father)
		{
			m_Father = father;
		}

		Graph *const father()
		{
			return m_Father;

		}

		Graph const *const father() const
		{
			return m_Father;
		}

		SystemGraph *const system()
		{
			return m_System;
		}

		SystemGraph const *const system() const
		{
			return m_System;
		}

	protected:

		bool					m_Run;
		bool					m_RunOnce;

		SystemGraph				*const m_System;
		RunnableGraph			*m_Root;
		Graph					*m_Father;

	};

}