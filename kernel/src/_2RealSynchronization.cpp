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
	}

	void Synchronization::run()
	{
	}

	void Synchronization::update()
	{
	}

	void Synchronization::shutdown()
	{
	}

	void Synchronization::insertChild(Runnable &child, unsigned int index)
	{
	}

	void Synchronization::removeChild(Identifier const& id)
	{
		Runnable &root = getChild(id).root();
		m_System.stopChild(root.identifier());
	}

	//void Synchronization::removeChild(Identifier const& id)
	//{
	//	Runnable &root = getChild(id).root();
	//	RunnableList::iterator it = iteratorId(id);

	//	m_System.stopChild(root.identifier());
	////	//this->resetIO();
	//	m_Children.erase(it);

	//	Runnable::removeChild(id);
	//}

	//void Synchronization::insertChild(Runnable *const _child, unsigned int const& _index)
	//{
	//	RunnableList::iterator it = iteratorId(_child->identifier());
	//	
	//	if (it != m_Children.end())
	//	{
	//		throw Exception("internal error: could not add child to container - already exists");
	//	}
	//	if (_index > m_Children.size())
	//	{
	//		throw Exception("internal error: could not add child to container - index > childcount");
	//	}

	//	m_System.stopChild(this->root()->identifier());

	//	if (_index == m_Children.size())
	//	{
	//		//this->resetIO();
	//		m_Children.push_back(_child);
	//	}
	//	else if (_index == 0)
	//	{
	//		//this->resetIO();
	//		m_Children.push_front(_child);
	//	}
	//	else
	//	{
	//		//this->resetIO();
	//		RunnableList::iterator pos = iteratorPosition(_index);
	//		m_Children.insert(pos, _child);
	//	}

	//	_child->setFather(this);
	//}

	//void Synchronization::checkConfiguration()
	//{
	//	//try
	//	//{
	//	//	//if this is a sequence, link all children
	//	//	//however, the input of first element / output of last element stays unlinked
	//	//	//this is done because check config is called recursively starting with the root
	//	//	//& the father of a container is responsible for linking first input / last output
	//	//	//or, if the father is nirvana (in case of the root), the linking must be done by the user.
	//	//	if (type() == Entity::SEQUENCE)
	//	//	{
	//	//		std::list< OutputSlot * > outSlots;
	//	//		std::list< InputSlot * > inSlots;
	//	//		ContainerList::iterator out;
	//	//		ContainerList::iterator in;
	//	//		std::list< OutputSlot * >::iterator outSlot;
	//	//		std::list< InputSlot * >::iterator inSlot;
	//	//		
	//	//		in = m_Children.begin();
	//	//		in++;
	//	//		for (out = m_Children.begin(); in != m_Children.end(); in++, out++)
	//	//		{
	//	//			outSlots = (*out)->outputSlots();
	//	//			inSlots = (*in)->inputSlots();

	//	//			if (outSlots.size() != inSlots.size())
	//	//			{
	//	//				//this is a simple test to see if input / output slots match in number
	//	//				throw Exception("IO mismatch");
	//	//			}

	//	//			for (inSlot = inSlots.begin(), outSlot = outSlots.begin(); inSlot != inSlots.end(); inSlot++, outSlot++)
	//	//			{
	//	//				//TODO: check if types match!
	//	//				OutputSlot *a = *outSlot;
	//	//				InputSlot *b = *inSlot;
	//	//				//breaks former linkage if it differs
	//	//				//a->linkWith(b);
	//	//			}
	//	//		}
	//	//	}

	//	//	//now, do the same for all children
	//	//	for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
	//	//	{
	//	//		(*it)->checkConfiguration();
	//	//	}

	//	//	m_bIsConfigured = true;
	//	//}
	//	//catch (Exception &e)
	//	//{
	//	//	m_bIsConfigured = false;
	//	//	throw e;
	//	//}
	//}

	//void Synchronization::run()
	//{
	//	while (m_Run || m_RunOnce)
	//	{
	//		try
	//		{
	//			//if (!m_bIsConfigured)
	//			//{
	//			//	throw Exception("could not update container - is incorrectly configured");
	//			//}

	//			for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
	//			{
	//				(*it)->start(true);
	//				m_ThreadPool.start(**it, (*it)->name());
	//			}

	//			m_ThreadPool.joinAll();
	//			//sendData(false);

	//			m_RunOnce = false;
	//		}
	//		catch (Exception &e)
	//		{
	//			m_Run = false;
	//			m_RunOnce = false;

	//			m_System.handleException(*this, e);
	//		}
	//	}
	//}

	//void Synchronization::update()
	//{
	//	try
	//	{
	//		//if (!m_bIsConfigured)
	//		//{
	//		//	throw Exception("could not update container - is incorrectly configured");
	//		//}

	//		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
	//		{
	//			(*it)->start(true);
	//			m_ThreadPool.start(**it, (*it)->name());
	//		}

	//		m_ThreadPool.joinAll();
	//	}
	//	catch (Exception &e)
	//	{
	//		m_System.handleException(*this, e);
	//	}
	//}

	//void Synchronization::shutdown()
	//{
	//	for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
	//	{
	//		(*it)->stop();
	//	}

	//	m_ThreadPool.joinAll();

	//	for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
	//	{
	//		(*it)->shutdown();
	//	}
	//}
}