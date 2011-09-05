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
		try
		{
			shutdown();
		}
		catch (...)
		{
			std::cout << "service shutdown failed" << std::endl;
		}
		
		//in out slots are deleted by entity table

		//user defined service is deleted here
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
			m_InputParams.insert(NamedInput(_slot->id(), _slot));
			m_InputIds.push_back(id);
		}
		else if (type == IdentifierImpl::OUTPUT)
		{
			m_OutputParams.insert(NamedParam(_slot->name(), _slot));
			m_OutputIds.push_back(id);
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
			for (InputMap::iterator it = m_InputParams.begin(); it != m_InputParams.end(); it++)
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
				//not every output slot needs to be linked
				//if (!it->second->isLinked())
				//{
					//std::cout << "SERVICE CHECK CONFIG: output param not linked " << it->second->name() << std::endl;
					//throw Exception::failure();
				//}
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
			throw Exception::failure();
		}
	}

	void ServiceImpl::run() throw(...)
	{
		std::cout << "SERVICE RUN: " << name() << std::endl;
		while (m_bIsConfigured && (m_bRun || m_bRunOnce))
		{
			try
			{
				//call user service's update method
				std::cout << "SERVICE RUN: updating now " << name() << std::endl;
				m_Service->update();
				std::cout << "SERVICE RUN: updated " << name() << std::endl;
				//sendData(m_bRunOnce);
				std::cout << "SERVICE RUN: sent data " << name() << std::endl;
				m_bRunOnce = false;
				std::cout << "SERVICE RUN: success " << name() << std::endl;
			}
			catch (...)
			{
				std::cout << "SERVICE RUN: error " << name() << std::endl;
				m_bRun = false;
				m_bRunOnce = false;
				stop();
				//throw Exception::failure();
			}
		}
	}

	void ServiceImpl::update() throw(...)
	{
		try
		{
			std::cout << "SERVICE UPDATE: " << name() << std::endl;
			
			if (!m_bIsConfigured)
			{
				throw Exception::failure();
			}

			std::cout << "SERVICE UPDATE: updating now " << name() << std::endl;
			m_Service->update();
			std::cout << "SERVICE UPDATE: sending data " << name() << std::endl;
			//sendData(true);
			std::cout << "SERVICE UPDATE: success " << name() << std::endl;
		}
		catch (...)
		{
			std::cout << "SERVICE UPDATE: error " << name() << std::endl;
			throw Exception::failure();
		}
	}

	void ServiceImpl::shutdown() throw(...)
	{
		try
		{
			m_Service->shutdown();
		}
		catch(...)
		{
			//TODO: set error state
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
		InputMap::iterator it;
		for (it = m_InputParams.begin(); it != m_InputParams.end(); it++)
		{
			if (it->second->name() == _name)
			{
				if (it->second->isInitialized())
				{
					//exception occurs b/c of multiple initialization
					throw Exception::failure();
				}

				it->second->setValue(_var);
				break;
			}
		}

		if (it == m_InputParams.end())
		{
			throw Exception::failure();
		}
	}

	void ServiceImpl::registerOutputSlot(std::string const& _name, AbstractRef *const _var)
	{
		ParamMap::iterator it = m_OutputParams.find(_name);
		if (it == m_OutputParams.end())
		{
			throw Exception::failure();
		}

		if (it->second->isInitialized())
		{
			//exception occurs b/c of multiple initialization
			throw Exception::failure();
		}
		it->second->setValue(_var);
	}

	AbstractContainer::IdentifierList ServiceImpl::inputParams() const
	{
		return m_InputIds;
	}

	AbstractContainer::IdentifierList ServiceImpl::outputParams() const
	{
		return m_OutputIds;
	}

	AbstractContainer::IdentifierList ServiceImpl::setupParams() const
	{
		return m_SetupIds;
	}

	void ServiceImpl::resetIO()
	{
		InputMap::iterator in;
		for (in = m_InputParams.begin(); in != m_InputParams.end(); in++)
		{
			in->second->reset();
		}

		ParamMap::iterator out;
		for (out = m_OutputParams.begin(); out != m_OutputParams.end(); out++)
		{
			out->second->reset();
		}

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