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

#include "_2RealSequence.h"
#include "_2RealRunnableManager.h"
#include "_2RealSystemGraph.h"
#include "_2RealPooledThread.h"

#include <iostream>

namespace _2Real
{

	Sequence::Sequence(Identifier const& id, SystemGraph &system) :
		RunnableGraph(id, system)
	{
	}

	void Sequence::run()
	{
		try
		{
			////carries out all accumulated insertions & removals, in the order they arrived
			//if (updateChildList())
			//{
			//	for (std::list< RunnableManager * >::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			//	{
			//		PooledThread &thread = m_System.getFreeThread();
			//		flagChildren(1);
			//		(*it)->update(thread);
			//		m_ChildrenFinished.wait();
			//	}
			//}
		}
		catch (Exception &e)
		{
			m_System.handleException(*this, e);
		}
	}

}