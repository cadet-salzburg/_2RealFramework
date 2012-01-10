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

#include <iostream>

namespace _2Real
{

	Synchronization::Synchronization(Identifier const& id, SystemGraph &system) :
		RunnableGraph(id, system)
	{
	}

	void Synchronization::run()
	{
		try
		{
			//std::cout << "sync updating: " << name() << std::endl;

			for (RunnableManager *child = getFirstChild(); child != NULL; child = getNextChild())
			{
				PooledThread &thread = m_System.getFreeThread();
				child->update(thread);
			}

			for (RunnableManager *child = getFirstChild(); child != NULL; child = getNextChild())
			{
			//	//if a child was removed in the meantime, it's not the sync's job to wait for it any more
			//	//in the rare case that an updating runnable was added, it will be waited for here
				child->wait();
			}

		}
		catch (_2Real::Exception &e)
		{
			m_System.handleException(*this, e);
		}
	}

}