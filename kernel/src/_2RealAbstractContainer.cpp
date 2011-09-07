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

#include "_2RealAbstractContainer.h"
#include "_2RealException.h"
#include "_2RealIdentifierImpl.h"
#include "_2RealDataImpl.h"
#include "_2RealData.h"
#include "_2RealServiceSlot.h"

#include "Poco\Delegate.h"

#include <iostream>

namespace _2Real
{

	AbstractContainer::AbstractContainer(IdentifierImpl *const _id) :
		IEntity(_id),
		m_bRunOnce(false),
		m_bRun(false), 
		m_bCanReconfigure(true), 
		m_bIsConfigured(false), 
		m_Father(NULL)
	{
	}

	AbstractContainer::AbstractContainer(AbstractContainer const& _src) : IEntity(_src)
	{
		throw Exception::noCopy();
	}

	AbstractContainer& AbstractContainer::operator=(AbstractContainer const& _src)
	{
		throw Exception::noCopy();
	}

	AbstractContainer::~AbstractContainer()
	{
	}

	void AbstractContainer::start(bool const& _runOnce) throw(...)
	{
		if (!m_bIsConfigured)
		{
			throw Exception::failure();
		}

		m_bRunOnce = _runOnce;
		m_bRun = !_runOnce;
	}

	void AbstractContainer::stop()
	{
		m_bRunOnce = false;
		m_bRun = false;


	}

	void AbstractContainer::setup(ServiceContext *const _contextPtr)
	{
	}

	bool const& AbstractContainer::canReconfigure() const
	{
		return m_bCanReconfigure;
	}

	AbstractContainer *const AbstractContainer::root()
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
				return m_Father->root();
			}
		}
		catch (...)
		{
			throw;
		}
	}

	void AbstractContainer::setFather(AbstractContainer *const _father)
	{
		if (_father == NULL)
		{
			throw Exception::failure();
		}

		m_Father = _father;
		IEntity::setInfo("belongs to container: " + _father->name());
	}

	AbstractContainer *const AbstractContainer::father()
	{
		return m_Father;
	}

	void AbstractContainer::addListener(IDataQueue *const _queue)
	{
		std::cout << "ABSTRACT CONTAINER ADD LISTENER: " << name() << std::endl;

		if (_queue == NULL)
		{
			throw Exception::failure();
		}

		AbstractContainer *container = static_cast< AbstractContainer * >(_queue);
		m_Listeners.push_back(container);
		m_NewData += Poco::delegate(container, &AbstractContainer::receiveData);
	}

	void AbstractContainer::removeListener(IDataQueue *const _queue) throw(...)
	{
		std::cout << "ABSTRACT CONTAINER REMOVE LISTENER " << name() << std::endl;

		if (_queue == NULL)
		{
			throw Exception::failure();
		}

		AbstractContainer *container = static_cast< AbstractContainer * >(_queue);
		for (ContainerList::iterator it = m_Listeners.begin(); it != m_Listeners.end(); it++)
		{
			if (*it == container)
			{
				m_Listeners.erase(it);
				break;
			}
		}

		m_NewData -= Poco::delegate(container, &AbstractContainer::receiveData);
	}

	void AbstractContainer::receiveData(NamedData &_data)
	{
		try
		{
			m_Mutex.lock();

			if (type() != IdentifierImpl::SERVICE)
			{
				throw Exception::failure();
			}
			
			unsigned int sender = _data.first;

			ContainerList::iterator it;
			for (it = m_Senders.begin(); it != m_Senders.end(); it++)
			{
				if ((*it)->id() == sender)
				{
					break;
				}
			}

			if (it == m_Senders.end())
			{
				throw Exception::failure();
			}

			m_DataList.push_back(_data);

			m_Mutex.unlock();
		}
		catch (...)
		{
			std::cout << "ABSTRACT CONTAINER RECEIVE DATA: error" << std::endl;
			m_Mutex.unlock();
			throw;
		}
	}

	void AbstractContainer::sendData(bool const& _blocking)
	{
		try
		{
			//std::cout << "SEND DATA: " << name() << std::endl;
			
			Poco::SharedPtr< DataImpl > outputData = Poco::SharedPtr< DataImpl >(new DataImpl());
			unsigned int name = id();

			std::list< ServiceSlot * > out = this->outputSlots();
			std::list< ServiceSlot * >::iterator it;

			for (it = out.begin(); it != out.end(); it++)
			{
				ServiceSlot *slot = *it;
				ServiceSlot::NamedAny any = slot->getAny();
				outputData->insertAny(any.first, any.second);
			}

			NamedData data(name, outputData);

			if (_blocking)
			{
				m_NewData.notify(this, data);
			}
			else
			{
				m_NewData.notifyAsync(this, data);
			}
		}
		catch (...)
		{
			std::cout << "SEND DATA: error " << name() << std::endl;
			throw;
		}
	}

	void AbstractContainer::listenTo(AbstractContainer *const _sender)
	{
		m_Senders.push_back(_sender);
	}

	void AbstractContainer::stopListeningTo(AbstractContainer *const _sender)
	{
		for (ContainerList::iterator it = m_Senders.begin(); it != m_Senders.end(); it++)
		{
			if (*it == _sender)
			{
				m_Senders.erase(it);
				break;
			}
		}
	}

	void AbstractContainer::resetIO()
	{
		std::list< ServiceSlot * > input = this->inputSlots();
		std::list< ServiceSlot * > output = this->outputSlots();

		std::list< ServiceSlot * >::iterator it;
		for (it = input.begin(); it != input.end(); it++)
		{
			ServiceSlot *out = (*it)->linked();
			if (out)
			{
				out->reset();
			}
		}

		for (it = output.begin(); it != output.end(); it++)
		{
			(*it)->reset();
		}
	}
}