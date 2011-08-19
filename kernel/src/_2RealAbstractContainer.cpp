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
#include "_2RealErrorState.h"
#include "_2RealConfigurationData.h"
#include "_2RealData.h"

#include "Poco\Delegate.h"

namespace _2Real
{

	AbstractContainer::AbstractContainer(IdentifierImpl const& _id) :
		m_ID(_id), m_bRunOnce(false), m_bRun(false), m_bCanReconfigure(false), m_bIsConfigured(false), m_Configuration(NULL)
	{
	}

	AbstractContainer::AbstractContainer(AbstractContainer const& _src)
	{
#ifdef _DEBUG
		std::cout << "abstract container copy constructor called" << std::endl;
#endif
		//does nothing
	}

	AbstractContainer& AbstractContainer::operator=(AbstractContainer const& _src)
	{
#ifdef _DEBUG
		std::cout << "abstract container assignment operator called" << std::endl;
#endif
		//does nothing
		return *this;
	}

	AbstractContainer::~AbstractContainer()
	{
		delete m_Configuration;
		m_NewData.clear();
	}

	IdentifierImpl const& AbstractContainer::id() const
	{
		return m_ID;
	}

	void AbstractContainer::start(bool const& _runOnce) throw(...)
	{
		if (!m_bIsConfigured || m_bRun)
		{
			throw ErrorState::failure();
		}

		m_bRunOnce = _runOnce;
		m_bRun = !_runOnce;
	}

	void AbstractContainer::stop()
	{
		m_bRunOnce = false;
		m_bRun = false;
	}

	void AbstractContainer::setup(ServiceContext *const _contextPtr) throw(...)
	{
		//does nothing
	}

	bool const& AbstractContainer::canReconfigure() const
	{
		return m_bCanReconfigure;
	}

	void AbstractContainer::addListener(IDataQueue *const _queue) throw(...)
	{
		if (_queue == NULL)
		{
			//TODO: set error state
			throw ErrorState::failure();
		}

		m_NewData += Poco::delegate(_queue, &IDataQueue::receiveData);
	}

	void AbstractContainer::removeListener(IDataQueue *const _queue) throw(...)
	{
		if (_queue == NULL)
		{
			//TODO: set error state
			throw ErrorState::failure();
		}

		m_NewData -= Poco::delegate(_queue, &IDataQueue::receiveData);
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

}