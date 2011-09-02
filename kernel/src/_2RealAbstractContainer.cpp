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

#include "Poco\Delegate.h"

namespace _2Real
{

	AbstractContainer::AbstractContainer(IdentifierImpl *const _id) :
		IEntity(_id),
		m_bRunOnce(false),
		m_bRun(false), 
		m_bCanReconfigure(false), 
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
		if (!m_bIsConfigured || m_bRun)
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
		Data(NULL);
	}

	bool const& AbstractContainer::canReconfigure() const
	{
		return m_bCanReconfigure;
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
		m_DataList.push_back(_data);
	}

	void AbstractContainer::sendData(bool const& _blocking)
	{
		if (_blocking)
		{
			m_NewData.notify(this, m_DataList.front());
		}
		else
		{
			m_NewData.notifyAsync(this, m_DataList.front());
		}
	}

	void AbstractContainer::registerExceptionCallback(void (*ExceptionCallback)(Identifier const& _sender, Exception const& _exception))
	{
		//m_Exception += Poco::delegate(container, &AbstractContainer::receiveData);
	}

	void AbstractContainer::registerDataCallback(void (*NewDataCallback)(Identifier const& _sender, Data const& _data))
	{

	}

	//TODO check for multiple entries
	void AbstractContainer::listenTo(AbstractContainer *const _sender)
	{
		m_Senders.push_back(_sender);
	}

	//TODO check for multiple entries
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
}