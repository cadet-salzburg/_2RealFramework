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

#pragma once

#include "_2RealServiceContainer.h"
#include "_2RealServiceContext.h"
#include "_2RealInputSlot.h"
#include "_2RealOutputSlot.h"
#include "_2RealServiceParameter.h"
#include "_2RealException.h"
#include "_2RealDataPacket.h"
#include "_2RealContainer.h"
#include "_2RealApplicationCallback.h"

#include <iostream>

namespace _2Real
{

	ServiceContainer::ServiceContainer(IService *const _service, Id *const _id, ApplicationCallback *const _output) : 
		AbstractContainer(_id, _output),
		m_Service(_service)
	{
		if (m_Service == NULL)
		{
			throw Exception("service container could not be created - null pointer");
		}
	}

	ServiceContainer::ServiceContainer(ServiceContainer const& _src) : AbstractContainer(_src)
	{
		throw Exception("attempted to copy entity");
	}

	ServiceContainer& ServiceContainer::operator=(ServiceContainer const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	ServiceContainer::~ServiceContainer()
	{
		delete m_Service;
	}

	void ServiceContainer::addInputSlot(unsigned int const& id, InputSlot *const _slot)
	{
		if (_slot == NULL)
		{
			throw Exception("service slot could not be added - null pointer");
		}

		m_InputParams.insert(NamedInput(_slot->name(), _slot));
		m_InputIds.push_back(id);
		m_InputSlots.push_back(_slot);
	}

	void ServiceContainer::addOutputSlot(unsigned int const& id, OutputSlot *const _slot)
	{
		if (_slot == NULL)
		{
			throw Exception("service slot could not be added - null pointer");
		}

		m_OutputParams.insert(NamedOutput(_slot->name(), _slot));
		m_OutputIds.push_back(id);
		m_OutputSlots.push_back(_slot);
	}

	void ServiceContainer::addSetupValue(unsigned int const& id, ServiceParameter *const _value)
	{
		if (_value == NULL)
		{
			throw Exception("setup parameter could not be added - null pointer");
		}

		m_SetupParams.insert(NamedParam(_value->name(), _value));
		m_SetupIds.push_back(id);
		m_SetupValues.push_back(_value);
	}

	void ServiceContainer::checkConfiguration()
	{
		try
		{
			if (!m_bIsConfigured || m_bCanReconfigure)
			{
				m_Service->setup(new ServiceContext(this));
			}

			//check if all input params are linked & initialized
			for (InputMap::iterator it = m_InputParams.begin(); it != m_InputParams.end(); it++)
			{
				if (!it->second->isLinked())
				{
					throw Exception("service setup error: input slot " + it->first + " is not linked");
				}
				if (!it->second->isInitialized())
				{
					throw Exception("service setup error: input slot " + it->first + " was not initialized by the service");
				}
			}

			//check if all output params are initialized
			for (OutputMap::iterator it = m_OutputParams.begin(); it != m_OutputParams.end(); it++)
			{
				if (!it->second->isInitialized())
				{
					throw Exception("service setup error: output slot " + it->first + " was not initialized by the service");
				}
			}

			//check if all setup params are initialized
			for (ParamMap::iterator it = m_SetupParams.begin(); it != m_SetupParams.end(); it++)
			{
				if (!it->second->isInitialized())
				{
					throw Exception("service setup error: value for setup parameter " + it->first + " was not provided");
				}
			}

			m_bIsConfigured = true;
		}
		catch(Exception &e)
		{
			m_bIsConfigured = false;
			throw e;
		}
	}

	void ServiceContainer::run()
	{
		while (m_bRun || m_bRunOnce)
		{
			try
			{
				m_Mutex.lock();

				if (!m_bIsConfigured)
				{
					throw Exception("attempted to update incorrectly configured service " + name());
				}

				DataPacket input;
				ContainerList copy(m_Senders);
				ContainerList::iterator container;
				std::list< NamedData >::reverse_iterator data;
				for (data = m_DataList.rbegin(); data != m_DataList.rend(); data++)
				{
					unsigned int sender = data->first;

					for (container = copy.begin(); container != copy.end(); container++)
					{
						if (sender == (*container)->id())
						{
							break;
						}
					}

					if (container != copy.end())
					{
						input.merge(*data->second.get());

						copy.erase(container);
						if (copy.empty())
						{
							break;
						}
					}
				}

				m_DataList.clear();
				m_Mutex.unlock();

				bool canUpdate = true;
				for (InputMap::iterator in = m_InputParams.begin(); in != m_InputParams.end(); in++)
				{
					unsigned int id = in->second->linkedOutput()->id();
					if (input.contains(id))
					{
						DataPacket::SharedAny any = input.getAny(id);
						in->second->extractFrom(any);
					}
					else
					{
						if (m_bRunOnce)
						{
							throw Exception("attempted to update service with lacking input data: " + in->second->name());
						}
						else
						{
							canUpdate = false;
						}
					}
				}

				if (canUpdate)
				{
					m_Service->update();
					sendData(m_bRunOnce);
					m_bRunOnce = false;
				}
			}
			catch (Exception &e)
			{
				m_Mutex.unlock();
				m_bRun = false;
				m_bRunOnce = false;
				Container *root = static_cast< Container * >(this->root());
				Container *nirvana = static_cast< Container * >(root->father());
				nirvana->stopChild(root->id());
				m_Output->sendException(e);
			}
		}
	}

	void ServiceContainer::update()
	{
		try
		{
			m_Mutex.lock();

			if (!m_bIsConfigured)
			{
				throw Exception("attempted to update incorrectly configured service " + name());
			}

			//extract data
			DataPacket input;
			ContainerList copy(m_Senders);
			ContainerList::iterator container;
			std::list< NamedData >::reverse_iterator data;
			for (data = m_DataList.rbegin(); data != m_DataList.rend(); data++)
			{
				unsigned int sender = data->first;

				for (container = copy.begin(); container != copy.end(); container++)
				{
					if (sender == (*container)->id())
					{
						break;
					}
				}

				if (container != copy.end())
				{
					input.merge(*data->second.get());
					
					copy.erase(container);
					if (copy.empty())
					{
						break;
					}
				}
			}

			m_DataList.clear();
			m_Mutex.unlock();

			for (InputMap::iterator in = m_InputParams.begin(); in != m_InputParams.end(); in++)
			{
				unsigned int id = in->second->linkedOutput()->id();
				if (input.contains(id))
				{
					DataPacket::SharedAny any = input.getAny(id);
					in->second->extractFrom(any);
				}
				else
				{
					throw Exception("attempted to update service with lacking input data: " + in->second->name());
				}
			}

			m_Service->update();
			sendData(true);
		}
		catch (Exception &e)
		{
			m_Mutex.unlock();
			throw e;
		}
	}

	void ServiceContainer::shutdown()
	{
		try
		{
			m_Service->shutdown();
		}
		catch(Exception &e)
		{
			throw e;
		}
	}

	void ServiceContainer::getParameterValue(std::string const& _name, AbstractRef *const _param)
	{
		ParamMap::iterator it = m_SetupParams.find(_name);
		if (it == m_SetupParams.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		Poco::Any any = (it->second->value());
		_param->extractFrom(any);
	}

	void ServiceContainer::registerInputSlot(std::string const& _name, AbstractRef *const _ref)
	{
		InputMap::iterator it = m_InputParams.find(_name);
		if (it == m_InputParams.end())
		{
			throw Exception("attempted to register non-existant input slots");
		}
		it->second->setReference(_ref);
	}

	void ServiceContainer::registerOutputSlot(std::string const& _name, AbstractRef *const _ref)
	{
		OutputMap::iterator it = m_OutputParams.find(_name);
		if (it == m_OutputParams.end())
		{
			throw Exception("attempted to register non-existant output slot");
		}
		it->second->setReference(_ref);
	}

	IDs ServiceContainer::inputSlotIDs() const
	{
		return m_InputIds;
	}

	IDs ServiceContainer::outputSlotIDs() const
	{
		return m_OutputIds;
	}

	IDs ServiceContainer::setupParamIDs() const
	{
		return m_SetupIds;
	}

	std::list< InputSlot * > ServiceContainer::inputSlots()
	{
		return m_InputSlots;
	}

	std::list< OutputSlot * > ServiceContainer::outputSlots()
	{
		return m_OutputSlots;
	}

	std::list< ServiceParameter * > ServiceContainer::setupParams()
	{
		return m_SetupValues;
	}

}