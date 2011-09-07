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

#include "_2RealServiceImpl.h"
#include "_2RealServiceContext.h"
#include "_2RealServiceSlot.h"
#include "_2RealServiceValue.h"
#include "_2RealException.h"
#include "_2RealDataImpl.h"
#include "_2RealContainer.h"

#include <iostream>

namespace _2Real
{

	ServiceImpl::ServiceImpl(IService *const _service, IdentifierImpl *const _id) : 
		AbstractContainer(_id),
		m_Service(_service)
	{
		if (m_Service == NULL)
		{
			throw Exception::failure();
		}
	}

	ServiceImpl::ServiceImpl(ServiceImpl const& _src) : AbstractContainer(_src)
	{
		throw Exception::noCopy();
	}

	ServiceImpl& ServiceImpl::operator=(ServiceImpl const& _src)
	{
		throw Exception::noCopy();
	}

	ServiceImpl::~ServiceImpl()
	{
		delete m_Service;
	}

	void ServiceImpl::addSlot(unsigned int const& id, ServiceSlot *const _slot)
	{
		if (_slot == NULL)
		{
			throw Exception::failure();
		}

		IdentifierImpl::eType type = _slot->type();
		if (type == IdentifierImpl::INPUT)
		{
			m_InputParams.insert(NamedParam(_slot->name(), _slot));
			m_InputIds.push_back(id);
			m_InputSlots.push_back(_slot);
		}
		else if (type == IdentifierImpl::OUTPUT)
		{
			m_OutputParams.insert(NamedParam(_slot->name(), _slot));
			m_OutputIds.push_back(id);
			m_OutputSlots.push_back(_slot);
		}
	}

	void ServiceImpl::addValue(unsigned int const& id, ServiceValue *const _value)
	{
		if (_value == NULL)
		{
			throw Exception::failure();
		}

		m_SetupParams.insert(NamedValue(_value->name(), _value));
		m_SetupIds.push_back(id);
	}

	void ServiceImpl::checkConfiguration()
	{
		try
		{
			if (!m_bIsConfigured || m_bCanReconfigure)
			{
				m_Service->setup(new ServiceContext(this));
			}

			//check if all input params are linked
			for (ParamMap::iterator it = m_InputParams.begin(); it != m_InputParams.end(); it++)
			{
				if (!it->second->isLinked())
				{
					std::cout << "SERVICE CHECK CONFIG: input param not linked " << it->second->name() << std::endl;
					throw Exception::failure();
				}
				if (!it->second->isInitialized())
				{
					std::cout << "SERVICE CHECK CONFIG: input param not initialized " << it->second->name() << std::endl;
					throw Exception::failure();
				}
			}

			//check if all output params are linked
			for (ParamMap::iterator it = m_OutputParams.begin(); it != m_OutputParams.end(); it++)
			{
				if (!it->second->isInitialized())
				{
					std::cout << "SERVICE CHECK CONFIG: output param not initialized " << it->second->name() << std::endl;
					throw Exception::failure();
				}
			}

			//check if all setup params are initialized
			for (ValueMap::iterator it = m_SetupParams.begin(); it != m_SetupParams.end(); it++)
			{
				if (!it->second->isInitialized())
				{
					std::cout << "SERVICE CHECK CONFIG: setup param not initialized " << it->second->name() << std::endl;
					throw Exception::failure();
				}
			}

			m_bIsConfigured = true;
		}
		catch(...)
		{
			m_bIsConfigured = false;
			throw Exception::failure();
		}
	}

	void ServiceImpl::run()
	{
		while (m_bRun || m_bRunOnce)
		{
			try
			{
				m_Mutex.lock();
				
				if (!m_bIsConfigured)
				{
					throw Exception::failure();
				}

				DataImpl input;
				ContainerList copy(m_Senders);
				ContainerList::iterator container;
				std::list< NamedData >::reverse_iterator data;
				for (data = m_DataList.rbegin(); data != m_DataList.rend(); data++)
				{
					unsigned int sender = data->first;

#ifdef _VERBOSE
	std::cout << "SERVICE RUN " << name() << " data available from " << sender << std::endl;
#endif

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

				for (ParamMap::iterator in = m_InputParams.begin(); in != m_InputParams.end(); in++)
				{
					unsigned int id = in->second->linked()->id();
					if (input.contains(id))
					{
#ifdef _VERBOSE
	std::cout << "SERVICE RUN: " << name() << " found data for " << in->second->name() << std::endl;
#endif
						DataImpl::SharedAny any = input.getAny(id);
						in->second->extractFrom(any);
					}
					else
					{
						throw Exception::failure();
					}
				}

				//call user service's update method
				m_Service->update();
				sendData(m_bRunOnce);
				m_bRunOnce = false;
			}
			catch (...)
			{
				std::cout << "SERVICE RUN: error " << name() << std::endl;
				m_Mutex.unlock();
				m_bRun = false;
				m_bRunOnce = false;
				Container *root = static_cast< Container * >(this->root());
				Container *nirvana = static_cast< Container * >(root->father());
				nirvana->stopChild(root->id());
			}
		}
	}

	void ServiceImpl::update()
	{
		try
		{
			m_Mutex.lock();

			if (!m_bIsConfigured)
			{
				throw Exception::failure();
			}

			//extract data
			DataImpl input;
			ContainerList copy(m_Senders);
			ContainerList::iterator container;
			std::list< NamedData >::reverse_iterator data;
			for (data = m_DataList.rbegin(); data != m_DataList.rend(); data++)
			{
				unsigned int sender = data->first;

#ifdef _VERBOSE
				std::cout << "SERVICE UPDATE " << name() << " data available from " << sender << std::endl;
#endif

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

			for (ParamMap::iterator in = m_InputParams.begin(); in != m_InputParams.end(); in++)
			{
				unsigned int id = in->second->linked()->id();
				if (input.contains(id))
				{
#ifdef _VERBOSE
					std::cout << "SERVICE RUN: " << name() << " found data for " << in->second->name() << std::endl;
#endif
					DataImpl::SharedAny any = input.getAny(id);
					in->second->extractFrom(any);
				}
			}

			m_Service->update();
			sendData(true);
		}
		catch (...)
		{
			std::cout << "SERVICE UPDATE: error " << name() << std::endl;
			m_Mutex.unlock();
			throw Exception::failure();
		}
	}

	void ServiceImpl::shutdown()
	{
		try
		{
			std::cout << "SERVICE SHUTDOWN: " << name() << std::endl;
			
			m_Service->shutdown();
		}
		catch(...)
		{
			throw Exception::failure();
		}
	}

	void ServiceImpl::getParameterValue(std::string const& _name, AbstractRef *const _param)
	{
		ValueMap::iterator it = m_SetupParams.find(_name);
		if (it == m_SetupParams.end())
		{
			throw Exception::failure();
		}

		Poco::Any any = (it->second->value());
		_param->extractFrom(any);
	}

	void ServiceImpl::registerInputSlot(std::string const& _name, AbstractRef *const _var)
	{
		ParamMap::iterator it = m_InputParams.find(_name);
		if (it == m_InputParams.end())
		{
			throw Exception::failure();
		}
		//if (it->second->isInitialized())
		//{
		//	throw Exception::failure();
		//}
		it->second->setValue(_var);
	}

	void ServiceImpl::registerOutputSlot(std::string const& _name, AbstractRef *const _var)
	{
		ParamMap::iterator it = m_OutputParams.find(_name);
		if (it == m_OutputParams.end())
		{
			throw Exception::failure();
		}
		//if (it->second->isInitialized())
		//{
		//	throw Exception::failure();
		//}
		it->second->setValue(_var);
	}

	AbstractContainer::IdentifierList ServiceImpl::inputParamIDs() const
	{
		return m_InputIds;
	}

	AbstractContainer::IdentifierList ServiceImpl::outputParamIDs() const
	{
		return m_OutputIds;
	}

	AbstractContainer::IdentifierList ServiceImpl::setupParamIDs() const
	{
		return m_SetupIds;
	}

	std::list< ServiceSlot * > ServiceImpl::inputSlots()
	{
		return m_InputSlots;
	}

	std::list< ServiceSlot * > ServiceImpl::outputSlots()
	{
		return m_OutputSlots;
	}
}