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
#include "_2RealId.h"
#include "_2RealDataPacket.h"
#include "_2RealData.h"
#include "_2RealServiceSlot.h"
#include "_2RealApplicationCallback.h"

#include "Poco\Delegate.h"

#include <iostream>
#include <sstream>

namespace _2Real
{

	AbstractContainer::AbstractContainer(Id *const _id, ApplicationCallback *const _output) :
		Entity(_id),
		m_bRunOnce(false),
		m_bRun(false), 
		m_bCanReconfigure(true), 
		m_bIsConfigured(false), 
		m_Father(NULL),
		m_Output(_output)
	{
	}

	AbstractContainer::AbstractContainer(AbstractContainer const& _src) : Entity(_src)
	{
		throw Exception("attempted to copy an entity");
	}

	AbstractContainer& AbstractContainer::operator=(AbstractContainer const& _src)
	{
		throw Exception("attempted to copy an entity");
	}

	AbstractContainer::~AbstractContainer()
	{
		delete m_Output;
	}

	void AbstractContainer::start(bool const& _runOnce) throw(...)
	{
		if (!m_bIsConfigured)
		{
			throw Exception("could not start container " + name() + ", was not configured");
		}

		m_bRunOnce = _runOnce;
		m_bRun = !_runOnce;
	}

	void AbstractContainer::stop()
	{
		//calling stop will cause a running thread to finish once their update cycle is completed
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
			if (m_Father->type() == Entity::SYSTEM)
			{
				return this;
			}
			else if (!m_Father)
			{
				throw Exception("internal error: container " + name() + "'s father is null");
			}
			else 
			{
				return m_Father->root();
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void AbstractContainer::setFather(AbstractContainer *const _father)
	{
		if (!_father)
		{
			throw Exception("internal error: attempted to set " + name() + "'s father to null");
		}

		std::stringstream info;
		info << this->info();
		info << "father:\t\t" + _father->name() << std::endl;
		m_Father = _father;
		AbstractContainer *root;
		AbstractContainer *nirvana;
		
		if (_father->type() != Entity::SYSTEM)
		{
			root = _father->root();
			nirvana = root->father();
		}
		else
		{
			root = this;
			nirvana = _father;
		}
		info << "graph root:\t" + root->name() << std::endl;
		info << "system:\t\t" + nirvana->name() << std::endl;
		Entity::setInfo(info.str());
	}

	AbstractContainer *const AbstractContainer::father()
	{
		return m_Father;
	}

	void AbstractContainer::addListener(IDataQueue *const _queue)
	{
		if (!_queue)
		{
			throw Exception("internal error: attempted to add listener to " + name() + " , null pointer");
		}
		AbstractContainer *container = static_cast< AbstractContainer * >(_queue);
		m_Listeners.push_back(container);
		m_NewData += Poco::delegate(container, &AbstractContainer::receiveData);
	}

	void AbstractContainer::removeListener(IDataQueue *const _queue) throw(...)
	{
		if (!_queue)
		{
			throw Exception("internal error: attempted to remove listener from " + name() + " , null pointer");
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

			unsigned int sender = _data.first;

			//TODO: senders as list not a good idea?
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
				throw Exception("internal error: " + name() + " received data from unknown sender");
			}

			m_DataList.push_back(_data);
			m_Mutex.unlock();
		}
		catch (Exception &e)
		{
			m_Mutex.unlock();
			throw e;
		}
	}

	void AbstractContainer::sendData(bool const& _blocking)
	{
		try
		{
			if (type() == Entity::SERVICE || m_Output->hasDataListeners())
			{
				Poco::SharedPtr< DataPacket > outputData = Poco::SharedPtr< DataPacket >(new DataPacket());
				unsigned int name = id();

				std::list< ServiceSlot * > out = this->outputSlots();
				std::list< ServiceSlot * >::iterator it;

				for (it = out.begin(); it != out.end(); it++)
				{
					ServiceSlot *slot = *it;
					ServiceSlot::NamedAny any = slot->getAny();
					outputData->insertAny(any.first, any.second);
				}

				if (type() == Entity::SERVICE)
				{
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
				if (m_Output->hasDataListeners())
				{
					m_Output->sendData(outputData);
				}
			}
		}
		catch (Exception &e)
		{
			throw e;
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

	void AbstractContainer::registerExceptionCallback(ExceptionCallback _callback)
	{
		m_Output->registerExceptionCallback(_callback);
	}

	void AbstractContainer::registerDataCallback(NewDataCallback _callback)
	{
		m_Output->registerDataCallback(_callback);
	}
}