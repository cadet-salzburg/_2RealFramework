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
//#include "_2RealSystemGraph.h" -> include just because of the exception handler?
#include "_2RealRunnableManager.h"

namespace _2Real
{

	Sequence::Sequence(Identifier const& id, SystemGraph &system) :
		RunnableGraph(id, system)
	{
	}

	Sequence::~Sequence()
	{
	}

	void Sequence::setup()
	{
	}

	void Sequence::run()
	{
		//while (m_Run || m_RunOnce)
		//{
		//	try
		//	{
		//		Runnable::updateTimer();

		//		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		//		{
		//			(*it)->update();
		//		}

		//		if (m_RunOnce)
		//		{
		//			m_RunOnce = false;
		//		}
		//		else
		//		{
		//			Runnable::suspend();
		//		}
		//	}
		//	catch (_2Real::Exception &e)
		//	{
		//		m_Run = false;
		//		m_RunOnce = false;

		//		m_System.handleException(*this, e);
		//	}
		//}
	}

	void Sequence::update()
	{
		//try
		//{
		//	for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		//	{
		//		(*it)->update();
		//	}
		//}
		//catch (_2Real::Exception &e)
		//{
		//	m_Run = false;
		//	m_RunOnce = false;

		//	m_System.handleException(*this, e);
		//}
	}

	void Sequence::shutdown()
	{
		//for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		//{
		//	(*it)->shutdown();
		//}
	}

	void Sequence::remove(Identifier const& childId)
	{
	}

	void Sequence::insert(RunnableManager &child, unsigned int index)
	{
	}

	//void Container::remove(unsigned int const& _id)
	//{
	//	AbstractContainer *root = this->root();
	//	Container *nirvana = static_cast< Container * >(root->father());
	//	nirvana->stopChild(root->id());
	//	child->resetIO();
	//	m_Children.erase(it);
	//}

	//void Container::add(AbstractContainer *const _child, unsigned int const& _index)
	//{
	//		AbstractContainer::ContainerList::iterator it = child(_child->id());
	//		if (it != m_Children.end())
	//		{
	//			throw Exception("internal error: could not add child to container - already exists");
	//		}
	//		if (_index > m_Children.size())
	//		{
	//			throw Exception("internal error: could not add child to container - index > childcount");
	//		}
	//		if (type() == Entity::SYNCHRONIZATION || type() == Entity::SEQUENCE)
	//		{
	//			AbstractContainer *root = this->root();
	//			Container *nirvana = static_cast< Container * >(root->father());
	//			nirvana->stopChild(root->id());
	//		}
	//		if (_index == m_Children.size())
	//		{
	//			if (type() == Entity::SEQUENCE && m_Children.size() >= 1)
	//			{
	//				AbstractContainer *old = m_Children.back();
	//				if (old)
	//				{
	//					old->resetIO();
	//				}
	//			}
	//			if (type() == Entity::SYNCHRONIZATION)
	//			{
	//				this->resetIO();
	//			}
	//			m_Children.push_back(_child);
	//		}
	//		else if (_index == 0)
	//		{
	//			if (type() == Entity::SEQUENCE && m_Children.size() >= 1)
	//			{
	//				AbstractContainer *old = m_Children.front();
	//				if (old)
	//				{
	//					old->resetIO();
	//				}
	//			}
	//			if (type() == Entity::SYNCHRONIZATION)
	//			{
	//				this->resetIO();
	//			}
	//			m_Children.push_front(_child);
	//		}
	//		else
	//		{
	//			if (type() == Entity::SYNCHRONIZATION)
	//			{
	//				this->resetIO();
	//			}
	//			AbstractContainer::ContainerList::iterator pos = position(_index);
	//			m_Children.insert(pos, _child);
	//		}
	//		_child->setFather(this);
	//}

}