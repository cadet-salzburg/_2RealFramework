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

	Container::ChildList::iterator Container::findChild(unsigned int const& _id)
	{
		for (ChildList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
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
		m_Children.push_back(_child);
		_child->setFather(this);
	}

	void Container::remove(unsigned int const& _id)
	{
		ChildList::iterator it = findChild(_id);
		if (it != m_Children.end())
		{
			m_Children.erase(it);
		}

		throw Exception::failure();
	}

	AbstractContainer *const Container::getChild(unsigned int const& _id)
	{

		AbstractContainer *child;
		
		ChildList::iterator it = findChild(_id);
		if (it != m_Children.end())
		{
			return *it;
		}
		else
		{
			for (ChildList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if ((*it)->type() != IdentifierImpl::SERVICE)
				{
					Container *container = static_cast< Container * >(*it);
					if ((child = container->getChild(_id)) != NULL)
					{
						return child;
					}
				}
			}
			return NULL;
		}
	}

	AbstractContainer *const Container::getChild(unsigned int const& _id, Container *const _father)
	{
		AbstractContainer *child;
		
		ChildList::iterator it = findChild(_id);
		if (it != m_Children.end())
		{
			child = (*it);		
			if (this != _father)
			{
				m_Children.erase(it);
				_father->append(child);
				child->setFather(_father);		
			}
			return child;
		}
		else
		{
			for (ChildList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			{
				if ((*it)->type() != IdentifierImpl::SERVICE)
				{
					Container *container = static_cast< Container * >(*it);

					if (container->getChild(_id) != NULL)
					{
						container->stop();
						return container->getChild(_id, _father);
					}
				}
			}
			return NULL;
		}
	}

	void Container::addBefore(AbstractContainer *const _child, unsigned int const& _id)
	{
		try
		{
			ChildList::iterator it = findChild(_id);
				
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
		if (m_bIsConfigured && !m_bCanReconfigure)
		{
			throw Exception::failure();
		}

		try
		{
			//TODO: setup children here
		}
		catch (...)
		{
			throw Exception::failure();
		}

	}

	void Container::run()
	{
		while (m_bIsConfigured && (m_bRun || m_bRunOnce))
		{
			try
			{
			}
			catch (...)
			{

				throw Exception::failure();
			}

			m_bRunOnce = false;
			sendData(false);
		}
	}

	void Container::update()
	{
		if (!m_bIsConfigured)
		{
			throw Exception::failure();
		}

		try
		{

		}
		catch (...)
		{
			throw Exception::failure();
		}

		sendData(true);
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
}