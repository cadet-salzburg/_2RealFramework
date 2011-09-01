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
#include "_2RealIdentifier.h"

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

	void ServiceImpl::addSlot(Identifier const& id, ServiceSlot *const _slot)
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

	void ServiceImpl::addValue(Identifier const& id, ServiceValue *const _value)
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
			if (!m_bIsConfigured)
			{
				//call user service's setup method
				m_Service->setup(new ServiceContext(this));
			}
			else if (m_bCanReconfigure)
			{
				//call user service's setup method
				m_Service->setup(new ServiceContext(this));
			}

			//check in/out slots
			//for ()

			m_bIsConfigured = true;
		}
		catch(...)
		{
			throw Exception::failure();
		}
	}

	void ServiceImpl::run() throw(...)
	{
		while (m_bIsConfigured && (m_bRun || m_bRunOnce))
		{
			try
			{
				//call user service's update method
				m_Service->update();
			}
			catch (...)
			{
				//TODO: set error state
				throw Exception::failure();
			}

			m_bRunOnce = false;
			//send DataImpl without waiting
			sendData(false);
		}
	}

	void ServiceImpl::update() throw(...)
	{
		if (!m_bIsConfigured)
		{
			//todo: set error state
			throw Exception::failure();
		}

		try
		{
			m_Service->update();
		}
		catch (...)
		{
			throw Exception::failure();
		}

		//send DataImpl - wait until all listeners received it
		sendData(true);
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

}