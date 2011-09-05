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

namespace _2Real
{
	
	Container::Container(IdentifierImpl *const _id) : AbstractContainer(_id)
	{
	}

	Container::Container(Container const& _src) : AbstractContainer(_src)
	{
	}

	Container& Container::operator=(Container const& _src)
	{
		throw Exception::noCopy();
	}

	Container::~Container()
	{
		try
		{
			shutdown();
		}
		catch (...)
		{
			std::cout << "service shutdown failed" << std::endl;
		}
		
		//TODO: children are deleted here
	}

	const unsigned int Container::childCount() const
	{
		return m_Children.size();
	}

	std::list< unsigned int > Container::children() const
	{
		std::list< unsigned int > ids;
		for (ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			ids.push_back((*it)->id());
		}

		return ids;
	}

	void Container::startChild(unsigned int const& _id)
	{
		try
		{
			//
			if (type() != IdentifierImpl::NIRVANA)
			{
				std::cout << "CONTAINER START CHILD: not nirvana" << std::endl;
				throw Exception::failure();
			}

			AbstractContainer *child = NULL;
			for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if ((*it)->id() == _id)
				{
					child = *it;
					break;
				}
			}

			if (!child)
			{
				std::cout << "CONTAINER START CHILD: child notfound" << std::endl;
				throw Exception::failure();
			}

			std::cout << "CONTAINER START CHILD: checking config" << std::endl;
			child->checkConfiguration();
			std::cout << "CONTAINER START CHILD: preparing start" << std::endl;
			child->start(false);
			std::cout << "CONTAINER START CHILD: starting thread" << std::endl;
			m_Threads.start(*child, child->name());
			std::cout << "CONTAINER START CHILD: success" << std::endl;
		}
		catch (...)
		{
			std::cout << "CONTAINER START CHILD: error" << std::endl;
			throw;
		}
	}

	void Container::stopChild(unsigned int const& _id)
	{
		try
		{
			//
			if (type() != IdentifierImpl::NIRVANA)
			{
				throw Exception::failure();
			}

			AbstractContainer *child = NULL;
			for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if ((*it)->id() == _id)
				{
					child = *it;
					break;
				}
			}

			if (!child)
			{
				throw Exception::failure();
			}

			child->stop();
		}
		catch (...)
		{
			throw;
		}
	}

	AbstractContainer::IdentifierList Container::setupParams() const
	{
		return IdentifierList();
	}

	AbstractContainer::IdentifierList Container::inputParams() const
	{
		if (type() == IdentifierImpl::NIRVANA)
		{
			throw Exception::failure();
		}
		else if (type() == IdentifierImpl::SEQUENCE)
		{
			IdentifierList result;
			AbstractContainer *first = m_Children.front();
			return first->inputParams();
		}
		else if (type() == IdentifierImpl::SYNCHRONIZATION)
		{
			IdentifierList result;
			for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				IdentifierList childParams = (*it)->inputParams();
				result.splice(result.end(), childParams);
			}

			return result;
		}

		throw Exception::failure();
	}

	AbstractContainer::IdentifierList Container::outputParams() const
	{
		if (type() == IdentifierImpl::NIRVANA)
		{
			throw Exception::failure();
		}
		else if (type() == IdentifierImpl::SEQUENCE)
		{
			IdentifierList result;
			AbstractContainer *last = m_Children.back();
			return last->outputParams();
		}
		else if (type() == IdentifierImpl::SYNCHRONIZATION)
		{
			IdentifierList result;
			for (AbstractContainer::ContainerList::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				IdentifierList childParams = (*it)->outputParams();
				result.splice(result.end(), childParams);
			}

			return result;
		}

		throw Exception::failure();
	}

	AbstractContainer::ContainerList::iterator Container::findChild(unsigned int const& _id)
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

	void Container::append(AbstractContainer *const _child)
	{
		AbstractContainer *last = NULL;
		if (!m_Children.empty())
		{
			last = m_Children.back();
		}

		m_Children.push_back(_child);
		_child->setFather(this);

		if (type() == IdentifierImpl::NIRVANA)
		{
		}
		else if (type() == IdentifierImpl::SYNCHRONIZATION)
		{
			//add listener
			_child->addListener(this);
			listenTo(_child);
		}
		else if (type() == IdentifierImpl::SEQUENCE)
		{
			if (last != NULL)
			{
				//listen to the new last element
				last->removeListener(this);
				stopListeningTo(last);
			}

			_child->addListener(this);
			listenTo(_child);
		}
	}

	void Container::remove(unsigned int const& _id)
	{
		AbstractContainer::ContainerList::iterator it = findChild(_id);
		if (it == m_Children.end())
		{
			throw Exception::failure();
		}

		AbstractContainer *child = *it;
		if (type() == IdentifierImpl::NIRVANA)
		{
			//simply erase
			m_Children.erase(it);
		}
		else if (type() == IdentifierImpl::SYNCHRONIZATION)
		{
			//stop superior
			this->root()->stop();

			child->setFather(NULL);
			child->resetIO();

			//erase from children
			m_Children.erase(it);
		}
		else if (type() == IdentifierImpl::SEQUENCE)
		{
			//stop superior
			 this->root()->stop();

			 child->setFather(NULL);
			 child->resetIO();

			//check if this was the last child in sequence
			if (child == m_Children.back())
			{
				//delete from children
				m_Children.erase(it);
				
				AbstractContainer *last = m_Children.back();
				if (last != NULL)
				{
					//listen to the new last child
					last->addListener(this);
					listenTo(last);
				}
			}
			else
			{
				//delete from children
				m_Children.erase(it);
			}
		}
	}

	AbstractContainer *const Container::root()
	{
		try
		{
			if (m_Father->type() == IdentifierImpl::NIRVANA)
			{
				return this;
			}
			else if (m_Father->type() == IdentifierImpl::SERVICE)
			{
				throw Exception::failure();
			}
			else if (m_Father == NULL)
			{
				throw Exception::failure();
			}
			else 
			{
				Container *father = static_cast< Container * >(m_Father);
				return father->root();
			}
		}
		catch (...)
		{
			throw;
		}
	}

	AbstractContainer *const Container::getChild(unsigned int const& _id)
	{
		AbstractContainer *child;
		AbstractContainer::ContainerList::iterator it = findChild(_id);
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
				if ((*it)->type() != IdentifierImpl::SERVICE)
				{
					Container *container = static_cast< Container * >(*it);
					if ((child = container->getChild(_id)) != NULL)
					{
						return child;
					}
				}
			}

			//nothing found - return null
			return NULL;
		}
	}

	void Container::addBefore(AbstractContainer *const _child, unsigned int const& _id)
	{
		try
		{
			AbstractContainer::ContainerList::iterator it = findChild(_id);
				
			if (it == m_Children.end())
			{
				throw Exception::failure();
			}
				
			m_Children.insert(it, _child);
			_child->setFather(this);

		}
		catch (...)
		{
			throw;
		}
	}

	void Container::checkConfiguration() throw(...)
	{
		try
		{
			//if (m_bIsConfigured && !m_bCanReconfigure)
			//{
			//	throw Exception::failure();
			//}
			for (ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				(*it)->checkConfiguration();
			}

			m_bIsConfigured = true;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	void Container::run()
	{
		std::cout << "CONTAINER RUN" << std::endl;
		while (m_bIsConfigured && (m_bRun || m_bRunOnce))
		{
			try
			{
				if (type() == IdentifierImpl::NIRVANA)
				{
					std::cout << "CONTAINER RUN: is nirvana" << std::endl;
					throw Exception::failure();
				}
				else if (type() == IdentifierImpl::SEQUENCE)
				{
					for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
					{
						//if child is a service, will send data (blocking)
						std::cout << "SEQUENCE CONTAINER RUN: updating now "  << (*it)->name() << std::endl;
						(*it)->update();
						std::cout << "SEQUENCE CONTAINER RUN: updated successfully "  << (*it)->name() << std::endl;
					}
					//if (!m_Listeners.empty())
					//{
					//	sendData(false);
					//}
					std::cout << "SEQUENCE CONTAINER RUN: success" << std::endl;
				}
				else if (type() == IdentifierImpl::SYNCHRONIZATION)
				{
					//initialize children for running once only
					for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
					{
						std::cout << "SYNCHRONIZATION CONTAINER RUN: starting " << (*it)->name() << std::endl;
						(*it)->start(true);
					}

					//start threads
					//service containers will send their data automatically at the end of update (blocking)
					for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
					{
						std::cout << "SYNCHRONIZATION CONTAINER RUN: starting thread " << (*it)->name() << std::endl;
						m_Threads.start(**it, (*it)->name());
					}

					std::cout << "SYNCHRONIZATION CONTAINER RUN: waiting for threads" << std::endl;
					//wait until everyone has finished
					m_Threads.joinAll();
					std::cout << "SYNCHRONIZATION CONTAINER RUN: all threads completed" << std::endl;

					//if (!m_Listeners.empty())
					//{
					//	sendData(false);
					//}
					std::cout << "SYNCHRONIZATION CONTAINER RUN: success" << std::endl;
				}

				m_bRunOnce = false;
			}
			catch (...)
			{
				std::cout << "CONTAINER RUN: error" << std::endl;
				m_bRun = false;
				m_bRunOnce = false;
				//this is a container. if run was called, that means the father is nirvana.
				//stop everything, tell father that this happened
				stop();
				//throw Exception::failure();
			}
		}
	}

	void Container::update()
	{
		try
		{
			if (!m_bIsConfigured)
			{
				std::cout << "CONTAINER UPDATE: not configured" << std::endl;
				throw Exception::failure();
			}

			if (type() == IdentifierImpl::NIRVANA)
			{
				std::cout << "CONTAINER UPDATE: is nirvana" << std::endl;
				throw Exception::failure();
			}
			else if (type() == IdentifierImpl::SEQUENCE)
			{
				for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					std::cout << "SEQUENCE CONTAINER UPDATE: updating now: " << (*it)->name() << std::endl;
					(*it)->update();
					std::cout << "SEQUENCE CONTAINER UPDATE: updated successfully: " << (*it)->name() << std::endl;
				}
				//if (!m_Listeners.empty())
				//{
				//	sendData(false);
				//}
				std::cout << "SEQUENCE CONTAINER UPDATE: success" << std::endl;
			}
			else if (type() == IdentifierImpl::SYNCHRONIZATION)
			{
				//initialize children for running once only
				for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					std::cout << "SYNCHRONIZATION CONTAINER UPDATE: starting " << (*it)->name() << std::endl;
					(*it)->start(true);
				}

				//start threads
				//service containers will send their data automatically at the end of run (blocking)
				for (AbstractContainer::ContainerList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				{
					std::cout << "SYNCHRONIZATION CONTAINER UPDATE: starting thread " << (*it)->name() << std::endl;
					m_Threads.start(**it, (*it)->name());
				}

				std::cout << "SYNCHRONIZATION CONTAINER UPDATE: waiting for threads" << std::endl;
				//wait until everyone has finished
				m_Threads.joinAll();
				std::cout << "SYNCHRONIZATION CONTAINER UPDATE: all threads completed" << std::endl;
				//if (!m_Listeners.empty())
				//{
				//	sendData(false);
				//}
				std::cout << "SYNCHRONIZATION CONTAINER UPDATE: success" << std::endl;
			}
		}
		catch (...)
		{
			std::cout << "CONTAINER UPDATE: error" << std::endl;
			throw Exception::failure();
		}
	}

	void Container::shutdown()
	{
		try
		{
		}
		catch(...)
		{
			throw Exception::failure();
		}
	}

	void Container::resetIO()
	{
		AbstractContainer::ContainerList::iterator it;

		for (it = m_Listeners.begin(); it != m_Listeners.end(); it++)
		{
			(*it)->stopListeningTo(this);
		}
		m_Listeners.clear();

		for (it = m_Senders.begin(); it != m_Senders.end(); it++)
		{
			(*it)->removeListener(this);
		}
		m_Senders.clear();
	}
}