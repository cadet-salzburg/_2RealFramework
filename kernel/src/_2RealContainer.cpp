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

#include "_2RealContainer.h"
#include "_2RealException.h"
#include "_2RealServiceContainer.h"
#include "_2RealServiceSlot.h"
#include "_2RealApplicationCallback.h"

#include <iostream>

namespace _2Real
{
	
	Container::Container(Id *const _id, ApplicationCallback *const _output) :
		AbstractContainer(_id, _output),
		m_Children(),
		//TODO: find some good init values for the thread pool
		m_Threads()
	{
	}

	Container::Container(Container const& _src) : AbstractContainer(_src)
	{
		throw Exception("attempted to copy entity");
	}

	Container& Container::operator=(Container const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	Container::~Container()
	{
	}

	const unsigned int Container::childCount() const
	{
		return m_Children.size();
	}

	IDs Container::childIDs() const
	{
		IDs ids;
		for (ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			ids.push_back((*it)->id());
		}

		return ids;
	}

	std::list< AbstractContainer * > Container::children()
	{
		return m_Children;
	}

	void Container::startChild(unsigned int const& _id)
	{
		try
		{
			//must be system
			if (type() != Entity::SYSTEM)
			{
				throw Exception("internal error: only system can start children");
			}

			ContainerList::iterator it = this->child(_id);
			if (it == m_Children.end())
			{
				throw Exception("could not start container, " + name() + " does not contain this container");
			}
			if (!(*it))
			{
				throw Exception("internal error - null pointer stored in system");
			}

			AbstractContainer *child = *it;
			std::map< unsigned int, Poco::Thread * >::iterator t = m_Threads.find(child->id());
			if (t != m_Threads.end())
			{
				//container runs already, ignoring this
				return;
			}

			Poco::Thread *thread = new Poco::Thread();
			child->checkConfiguration();
			child->start(false);
			thread->start(*child);
			m_Threads.insert(std::pair< unsigned int, Poco::Thread * >(child->id(), thread));

		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Container::stopChild(unsigned int const& _id)
	{
		try
		{
			//must be system
			if (type() != Entity::SYSTEM)
			{
				throw Exception("internal error: only system can stop children");
			}

			ContainerList::iterator it = this->child(_id);
			if (it == m_Children.end())
			{
				throw Exception("could not stop container, " + name() + " does not contain this container");
			}
			if (!(*it))
			{
				throw Exception("internal error - null pointer stored in system");
			}

			AbstractContainer *child = *it;
			std::map< unsigned int, Poco::Thread * >::iterator thread = m_Threads.find(child->id());
			if (thread != m_Threads.end())
			{
				child->stop();
				thread->second->join();
				delete thread->second;
				m_Threads.erase(thread);
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	IDs Container::setupParamIDs() const
	{
		//container has no setup params
		throw Exception("a container has no setup params");
	}

	IDs Container::inputSlotIDs() const
	{
		if (type() == Entity::SEQUENCE)
		{
			//sequence: input = first child's input
			AbstractContainer *first = m_Children.front();
			return first->inputSlotIDs();
		}
		else if (type() == Entity::SYNCHRONIZATION)
		{
			//sync: input = uninon of all children's input
			IDs result;
			for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				IDs childParams = (*it)->inputSlotIDs();
				result.insert(result.end(), childParams.begin(), childParams.end());
			}

			return result;
		}
		else
		{
			//nirvana has no output params
			throw Exception("system has no output slots");
		}
	}

	IDs Container::outputSlotIDs() const
	{
		if (type() == Entity::SEQUENCE)
		{
			//sequence: output = last child's output
			AbstractContainer *last = m_Children.back();
			return last->outputSlotIDs();
		}
		else if (type() == Entity::SYNCHRONIZATION)
		{
			//sync: output = union of all children's output
			IDs result;
			for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				IDs childParams = (*it)->outputSlotIDs();
				result.insert(result.end(), childParams.begin(), childParams.end());
			}

			return result;
		}
		else
		{
			//nirvana has no output params
			throw Exception("system has no output slots");
		}
	}

	std::list< ServiceSlot * > Container::inputSlots()
	{
		if (type() == Entity::SEQUENCE)
		{
			//sequence: output = last child's output
			AbstractContainer *first = m_Children.front();
			return first->inputSlots();
		}
		else if (type() == Entity::SYNCHRONIZATION)
		{
			//sync: output = union of all children's output
			std::list< ServiceSlot * > result;
			for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				std::list< ServiceSlot * > childParams = (*it)->inputSlots();
				result.splice(result.end(), childParams);
			}

			return result;
		}
		else
		{
			//nirvana has no output params
			throw Exception("system has no output slots");
		}
	}

	std::list< ServiceSlot * > Container::outputSlots()
	{
		if (type() == Entity::SEQUENCE)
		{
			//sequence: output = last child's output
			AbstractContainer *last = m_Children.back();
			return last->outputSlots();
		}
		else if (type() == Entity::SYNCHRONIZATION)
		{
			//sync: output = union of all children's output
			std::list< ServiceSlot * > result;
			for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				std::list< ServiceSlot * > childParams = (*it)->outputSlots();
				result.splice(result.end(), childParams);
			}

			return result;
		}
		else
		{
			//nirvana has no output params
			throw Exception("system has no output slots");
		}
	}

	AbstractContainer::ContainerList::iterator Container::child(unsigned int const& _id)
	{
		for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->id() == _id)
			{
				return it;
			}
		}

		return m_Children.end();
	}

	AbstractContainer::ContainerList::const_iterator Container::child(unsigned int const& _id) const
	{
		for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->id() == _id)
			{
				return it;
			}
		}

		return m_Children.end();
	}

	AbstractContainer::ContainerList::iterator Container::position(unsigned int const& _index)
	{
		AbstractContainer::ContainerList::iterator position = m_Children.begin();
		for (unsigned int count = 0; count < _index; count++)
		{
			position++;
		}

		return position;
	}

	AbstractContainer::ContainerList::const_iterator Container::position(unsigned int const& _index) const
	{
		AbstractContainer::ContainerList::const_iterator position = m_Children.begin();
		for (unsigned int count = 0; count < _index; count++)
		{
			position++;
		}

		return position;
	}

	void Container::remove(unsigned int const& _id)
	{
		try
		{
			//find the container in question
			AbstractContainer::ContainerList::iterator it = child(_id);
			if (it == m_Children.end())
			{
				throw Exception("internal error: could not remove child from container - does not exist");
			}
			AbstractContainer *child = *it;
		
			if (type() == Entity::SYSTEM)
			{
				stopChild(child->id());
				child->resetIO();
				m_Children.erase(it);
			}
			else if (type() == Entity::SYNCHRONIZATION)
			{
				AbstractContainer *root = this->root();
				Container *nirvana = static_cast< Container * >(root->father());
				nirvana->stopChild(root->id());
				this->resetIO();
				m_Children.erase(it);
			}
			else if (type() == Entity::SEQUENCE)
			{
				AbstractContainer *root = this->root();
				Container *nirvana = static_cast< Container * >(root->father());
				nirvana->stopChild(root->id());
				child->resetIO();
				m_Children.erase(it);
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Container::add(AbstractContainer *const _child, unsigned int const& _index)
	{
		try
		{
			//make sure child does not already exist in list
			AbstractContainer::ContainerList::iterator it = child(_child->id());
			if (it != m_Children.end())
			{
				throw Exception("internal error: could not add child to container - already exists");
			}

			if (_index > m_Children.size())
			{
				throw Exception("internal error: could not add child to container - index > childcount");
			}

			//stop root
			if (type() == Entity::SYNCHRONIZATION || type() == Entity::SEQUENCE)
			{
				AbstractContainer *root = this->root();
				Container *nirvana = static_cast< Container * >(root->father());
				nirvana->stopChild(root->id());
			}

			if (_index == m_Children.size())
			{
				if (type() == Entity::SEQUENCE && m_Children.size() >= 1)
				{
					AbstractContainer *old = m_Children.back();
					if (old)
					{
						old->resetIO();
					}
				}
				if (type() == Entity::SYNCHRONIZATION)
				{
					this->resetIO();
				}

				m_Children.push_back(_child);
			}
			else if (_index == 0)
			{
				if (type() == Entity::SEQUENCE && m_Children.size() >= 1)
				{
					AbstractContainer *old = m_Children.front();
					if (old)
					{
						old->resetIO();
					}
				}
				if (type() == Entity::SYNCHRONIZATION)
				{
					this->resetIO();
				}

				m_Children.push_front(_child);
			}
			else
			{
				if (type() == Entity::SYNCHRONIZATION)
				{
					this->resetIO();
				}

				AbstractContainer::ContainerList::iterator pos = position(_index);
				m_Children.insert(pos, _child);
			}

			_child->setFather(this);

		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	AbstractContainer *const Container::get(unsigned int const& _id)
	{
		AbstractContainer *child;
		AbstractContainer::ContainerList::iterator it = this->child(_id);
		if (it != m_Children.end())
		{
			child = *it;
			remove(child->id());
			return child;
		}
		else
		{
			//search children
			for (it = m_Children.begin(); it != m_Children.end(); it++)
			{
				//only containers have children
				if ((*it)->type() != Entity::SERVICE)
				{
					Container *container = static_cast< Container * >(*it);
					if ((child = container->get(_id)) != NULL)
					{
						return child;
					}
				}
			}

			//nothing found - return null
			return NULL;
		}
	}

	AbstractContainer const *const Container::find(unsigned int const& _id) const
	{
		AbstractContainer::ContainerList::const_iterator it = this->child(_id);
		if (it != m_Children.end())
		{
			return *it;
		}
		else
		{
			//search children
			for (it = m_Children.begin(); it != m_Children.end(); it++)
			{
				//only containers have children
				if ((*it)->type() != Entity::SERVICE)
				{
					Container *child = static_cast< Container * >(*it);
					const AbstractContainer *res = child->find(_id);
					if (res)
					{
						return res;
					}
				}
			}

			//nothing found - return null
			return NULL;
		}
	}

	AbstractContainer *const Container::find(unsigned int const& _id)
	{
		AbstractContainer::ContainerList::iterator it = this->child(_id);
		if (it != m_Children.end())
		{
			return *it;
		}
		else
		{
			//search children
			for (it = m_Children.begin(); it != m_Children.end(); it++)
			{
				//only containers have children
				if ((*it)->type() != Entity::SERVICE)
				{
					Container *child = static_cast< Container * >(*it);
					AbstractContainer *res = child->find(_id);
					if (res)
					{
						return res;
					}
				}
			}

			//nothing found - return null
			return NULL;
		}
	}

	void Container::checkConfiguration()
	{
		try
		{
			//if this is a sequence, link all children
			//however, the input of first element / output of last element stays unlinked
			//this is done because check config is called recursively starting with the root
			//& the father of a container is responsible for linking first input / last output
			//or, if the father is nirvana (in case of the root), the linking must be done by the user.
			if (type() == Entity::SEQUENCE)
			{
				std::list< ServiceSlot * > outSlots;
				std::list< ServiceSlot * > inSlots;
				ContainerList::iterator out;
				ContainerList::iterator in;
				std::list< ServiceSlot * >::iterator outSlot;
				std::list< ServiceSlot * >::iterator inSlot;
				
				in = m_Children.begin();
				in++;
				for (out = m_Children.begin(); in != m_Children.end(); in++, out++)
				{
					outSlots = (*out)->outputSlots();
					inSlots = (*in)->inputSlots();

					if (outSlots.size() != inSlots.size())
					{
						//this is a simple test to see if input / output slots match in number
						throw Exception("IO mismatch");
					}

					for (inSlot = inSlots.begin(), outSlot = outSlots.begin(); inSlot != inSlots.end(); inSlot++, outSlot++)
					{
						//TODO: check if types match!
						ServiceSlot *a = *outSlot;
						ServiceSlot *b = *inSlot;
						//breaks former linkage if it differs
						a->linkWith(b);
					}
				}
			}

			//now, do the same for all children
			for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				(*it)->checkConfiguration();
			}

			m_bIsConfigured = true;
		}
		catch (Exception &e)
		{
			m_bIsConfigured = false;
			throw e;
		}
	}

	void Container::run()
	{
		while (m_bRun || m_bRunOnce)
		{
			try
			{
				if (!m_bIsConfigured)
				{
					throw Exception("could not update container - is incorrectly configured");
				}
				if (type() == Entity::SYSTEM)
				{
					throw Exception("could not update - is system");
				}
				if (type() == Entity::SEQUENCE)
				{
					for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
					{
						(*it)->update();
					}

					sendData(false);
				}
				else if (type() == Entity::SYNCHRONIZATION)
				{
					for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
					{
						(*it)->start(true);
						m_ThreadPool.start(**it, (*it)->name());
					}

					m_ThreadPool.joinAll();
					sendData(false);
				}

				m_bRunOnce = false;
			}
			catch (Exception &e)
			{
				m_bRun = false;
				m_bRunOnce = false;
				Container *root = static_cast< Container * >(this->root());
				Container *nirvana = static_cast< Container * >(root->father());
				nirvana->stopChild(root->id());
				m_Output->sendException(e);
			}
		}
	}

	void Container::update()
	{
		try
		{
			if (!m_bIsConfigured)
			{
				throw Exception("could not update container - is incorrectly configured");
			}
			if (type() == Entity::SYSTEM)
			{
				throw Exception("could not update container - is system");
			}
			else if (type() == Entity::SEQUENCE)
			{
				for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					(*it)->update();
				}

				sendData(false);
			}
			else if (type() == Entity::SYNCHRONIZATION)
			{
				for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					(*it)->start(true);
					m_ThreadPool.start(**it, (*it)->name());
				}

				m_ThreadPool.joinAll();

				sendData(false);
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Container::shutdown()
	{
		try
		{
			for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				(*it)->stop();
			}

			if (type() == Entity::SYSTEM)
			{
				for (std::map< unsigned int, Poco::Thread * >::iterator it = m_Threads.begin(); it != m_Threads.end(); it++)
				{
					it->second->join();
					delete it->second;
				}
				m_Threads.clear();
			}
			else if (type() == Entity::SYNCHRONIZATION)
			{
				for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					(*it)->stop();
				}

				m_ThreadPool.joinAll();
			}

			for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
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