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
#include "_2RealRunnableManager.h"
#include "_2RealSystemGraph.h"
#include "_2RealPooledThread.h"

namespace _2Real
{

	Synchronization::Synchronization(Identifier const& id, SystemGraph &system) :
		RunnableGraph(id, system)
	{
	}

	void Synchronization::run()
	{
		while (m_Run || m_RunOnce)
		{
			try
			{
				Runnable::updateTimer();

				for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					PooledThread &thread = m_System.getFreeThread();
					(*it)->update(thread);
				}

				for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					(*it)->wait();
				}

				if (m_RunOnce)
				{
					break;
				}

				Runnable::suspend();
			}
			catch (_2Real::Exception &e)
			{
				Runnable::stop();
				m_System.handleException(*this, e);
			}
		}
	}

	void Synchronization::removeChild(Identifier const& child)
	{
		RunnableList::iterator it = iteratorId(child);
		m_Children.erase(it);
	}

	void Synchronization::insertChild(RunnableManager &child, unsigned int index)
	{
		RunnableList::iterator it = iteratorPosition(index);
		m_Children.insert(it, &child);
		child.getManagedRunnable().setFather(*this);
	}

}