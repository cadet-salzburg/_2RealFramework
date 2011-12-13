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

#include "_2RealSynchronization.h"
#include "_2RealSystemGraph.h"

namespace _2Real
{

	Synchronization::Synchronization(Identifier const& id, SystemGraph &system) :
		RunnableGraph(id, system)
	{
	}

	Synchronization::~Synchronization()
	{
	}

	bool Synchronization::checkForSetup()
	{
		return true;
	}

	bool Synchronization::checkForUpdate()
	{
		return true;
	}

	void Synchronization::setup()
	{
		//?
	}

	void Synchronization::run()
	{
		while (m_Run || m_RunOnce)
		{
			try
			{
				Runnable::updateTimer();

				m_System.runOnce(m_Children);

				if (m_RunOnce)
				{
					m_RunOnce = false;
				}
				else
				{
					Runnable::suspend();
				}

			}
			catch (_2Real::Exception &e)
			{
				m_Run = false;
				m_RunOnce = false;

				m_System.handleException(*this, e);
			}
		}
	}

	void Synchronization::update()
	{
		try
		{
			m_System.runOnce(m_Children);
		}
		catch (_2Real::Exception &e)
		{
			m_Run = false;
			m_RunOnce = false;

			m_System.handleException(*this, e);
		}
	}

	void Synchronization::shutdown()
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			(*it)->shutdown();
		}
	}

	void Synchronization::removeChild(Identifier const& childId)
	{
		//Runnable &child = getChild(childId);
		//Runnable &root = child.root();
		//m_System.stopChild(root.identifier());

		//RunnableList::iterator it = iteratorId(childId);
		//m_Children.erase(it);
		
		//m_System.insertChild(child, 0);
	}

	void Synchronization::insertChild(Runnable &child, unsigned int index)
	{
	}

}