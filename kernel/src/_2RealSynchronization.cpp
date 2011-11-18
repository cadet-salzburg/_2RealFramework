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
#include "_2RealService.h"
#include "_2RealInputSlot.h"
#include "_2RealOutputSlot.h"

#include <iostream>

namespace _2Real
{
	
	Synchronization::Synchronization(Id *const _id, SystemGraph *const _system) :
		RunnableGraph(_id, _system)
	{
	}

	Synchronization::~Synchronization()
	{
	}

	//IDs Synchronization::inputSlotIDs() const
	//{
	//	if (type() == Entity::SEQUENCE)
	//	{
	//		//sequence: input = first child's input
	//		AbstractContainer *first = m_Children.front();
	//		return first->inputSlotIDs();
	//	}
	//	else if (type() == Entity::SYNCHRONIZATION)
	//	{
	//		//sync: input = uninon of all children's input
	//		IDs result;
	//		for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
	//		{
	//			IDs childParameters = (*it)->inputSlotIDs();
	//			result.insert(result.end(), childParameters.begin(), childParameters.end());
	//		}

	//		return result;
	//	}
	//	else
	//	{
	//		//nirvana has no output params
	//		throw Exception("system has no output slots");
	//	}
	//}

	//IDs Synchronization::outputSlotIDs() const
	//{
	//	if (type() == Entity::SEQUENCE)
	//	{
	//		//sequence: output = last child's output
	//		AbstractContainer *last = m_Children.back();
	//		return last->outputSlotIDs();
	//	}
	//	else if (type() == Entity::SYNCHRONIZATION)
	//	{
	//		//sync: output = union of all children's output
	//		IDs result;
	//		for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
	//		{
	//			IDs childParameters = (*it)->outputSlotIDs();
	//			result.insert(result.end(), childParameters.begin(), childParameters.end());
	//		}

	//		return result;
	//	}
	//	else
	//	{
	//		//nirvana has no output params
	//		throw Exception("system has no output slots");
	//	}
	//}

	//std::list< InputSlot * > Synchronization::inputSlots()
	//{
	//	if (type() == Entity::SEQUENCE)
	//	{
	//		//sequence: output = last child's output
	//		AbstractContainer *first = m_Children.front();
	//		return first->inputSlots();
	//	}
	//	else if (type() == Entity::SYNCHRONIZATION)
	//	{
	//		//sync: output = union of all children's output
	//		std::list< InputSlot * > result;
	//		for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
	//		{
	//			std::list< InputSlot * > childParameters = (*it)->inputSlots();
	//			result.splice(result.end(), childParameters);
	//		}

	//		return result;
	//	}
	//	else
	//	{
	//		//nirvana has no output params
	//		throw Exception("system has no output slots");
	//	}
	//}

	//std::list< OutputSlot * > Synchronization::outputSlots()
	//{
	//	if (type() == Entity::SEQUENCE)
	//	{
	//		//sequence: output = last child's output
	//		AbstractContainer *last = m_Children.back();
	//		return last->outputSlots();
	//	}
	//	else if (type() == Entity::SYNCHRONIZATION)
	//	{
	//		//sync: output = union of all children's output
	//		std::list< OutputSlot * > result;
	//		for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
	//		{
	//			std::list< OutputSlot * > childParameters = (*it)->outputSlots();
	//			result.splice(result.end(), childParameters);
	//		}

	//		return result;
	//	}
	//	else
	//	{
	//		//nirvana has no output params
	//		throw Exception("system has no output slots");
	//	}
	//}

	void Synchronization::removeChild(unsigned int const& _id)
	{
		RunnableList::iterator it = iteratorId(_id);
			
		if (it == m_Children.end())
		{
			throw Exception("internal error: could not remove child from container - does not exist");
		}

		Runnable *child = *it;

		m_System->stopChild(this->root()->id());
		//this->resetIO();

		m_Children.erase(it);
	}

	void Synchronization::insertChild(Runnable *const _child, unsigned int const& _index)
	{
		RunnableList::iterator it = iteratorId(_child->id());
		
		if (it != m_Children.end())
		{
			throw Exception("internal error: could not add child to container - already exists");
		}
		if (_index > m_Children.size())
		{
			throw Exception("internal error: could not add child to container - index > childcount");
		}

		m_System->stopChild(this->root()->id());

		if (_index == m_Children.size())
		{
			//this->resetIO();
			m_Children.push_back(_child);
		}
		else if (_index == 0)
		{
			//this->resetIO();
			m_Children.push_front(_child);
		}
		else
		{
			//this->resetIO();
			RunnableList::iterator pos = iteratorPosition(_index);
			m_Children.insert(pos, _child);
		}

		_child->setFather(this);
	}

	void Synchronization::checkConfiguration()
	{
		//try
		//{
		//	//if this is a sequence, link all children
		//	//however, the input of first element / output of last element stays unlinked
		//	//this is done because check config is called recursively starting with the root
		//	//& the father of a container is responsible for linking first input / last output
		//	//or, if the father is nirvana (in case of the root), the linking must be done by the user.
		//	if (type() == Entity::SEQUENCE)
		//	{
		//		std::list< OutputSlot * > outSlots;
		//		std::list< InputSlot * > inSlots;
		//		ContainerList::iterator out;
		//		ContainerList::iterator in;
		//		std::list< OutputSlot * >::iterator outSlot;
		//		std::list< InputSlot * >::iterator inSlot;
		//		
		//		in = m_Children.begin();
		//		in++;
		//		for (out = m_Children.begin(); in != m_Children.end(); in++, out++)
		//		{
		//			outSlots = (*out)->outputSlots();
		//			inSlots = (*in)->inputSlots();

		//			if (outSlots.size() != inSlots.size())
		//			{
		//				//this is a simple test to see if input / output slots match in number
		//				throw Exception("IO mismatch");
		//			}

		//			for (inSlot = inSlots.begin(), outSlot = outSlots.begin(); inSlot != inSlots.end(); inSlot++, outSlot++)
		//			{
		//				//TODO: check if types match!
		//				OutputSlot *a = *outSlot;
		//				InputSlot *b = *inSlot;
		//				//breaks former linkage if it differs
		//				//a->linkWith(b);
		//			}
		//		}
		//	}

		//	//now, do the same for all children
		//	for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		//	{
		//		(*it)->checkConfiguration();
		//	}

		//	m_bIsConfigured = true;
		//}
		//catch (Exception &e)
		//{
		//	m_bIsConfigured = false;
		//	throw e;
		//}
	}

	void Synchronization::run()
	{
		while (m_Run || m_RunOnce)
		{
			try
			{
				//if (!m_bIsConfigured)
				//{
				//	throw Exception("could not update container - is incorrectly configured");
				//}

				for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					(*it)->start(true);
					m_ThreadPool.start(**it, (*it)->name());
				}

				m_ThreadPool.joinAll();
				//sendData(false);

				m_RunOnce = false;
			}
			catch (Exception &e)
			{
				m_Run = false;
				m_RunOnce = false;

				m_System->handleException(this, e);
			}
		}
	}

	void Synchronization::update()
	{
		try
		{
			//if (!m_bIsConfigured)
			//{
			//	throw Exception("could not update container - is incorrectly configured");
			//}

			for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				(*it)->start(true);
				m_ThreadPool.start(**it, (*it)->name());
			}

			m_ThreadPool.joinAll();

			//sendData(false);
		}
		catch (Exception &e)
		{
			m_System->handleException(this, e);
		}
	}

	void Synchronization::shutdown()
	{
		try
		{
			for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				(*it)->stop();
			}

			m_ThreadPool.joinAll();

			for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				(*it)->shutdown();
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}