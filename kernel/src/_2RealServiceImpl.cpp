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
#include "_2RealException.h"

namespace _2Real
{

	ServiceImpl::ServiceImpl(IService *const _service) throw (...) : m_Service(_service)
	{
		if (m_Service == NULL)
		{
			//TODO: set error state
			throw Exception::failure();
		}
	}

	ServiceImpl::ServiceImpl(ServiceImpl const& _src) : AbstractContainer(_src)
	{
		//service container must not be copied
		throw Exception::failure();
	}

	ServiceImpl& ServiceImpl::operator=(ServiceImpl const& _src)
	{
		AbstractContainer::operator=(_src);
		//service container must not be copied
		throw Exception::failure();
		//return *this;
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
		
		//user defined service is deleted here
		delete m_Service;
	}

	void ServiceImpl::configure(ConfigurationData *const _config) throw(...)
	{
		if (m_bIsConfigured && !m_bCanReconfigure)
		{
			//TODO: set error state
			throw Exception::failure();
		}
		else if (_config == NULL)
		{
			//TODO: set error state
			throw Exception::failure();
		}

		try
		{
			//call user service's setup method
			m_Service->setup(new ServiceContext(this));
		}
		catch (...)
		{
			//TODO: set error state
			throw Exception::failure();
		}

		//save configuration
		m_Configuration = _config;
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
			//send data without waiting
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

		//send data - wait until all listeners received it
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

	void ServiceImpl::getParameterValue(AbstractValue *const _param)
	{
		//TODO
	}

	void ServiceImpl::registerInputVariable(AbstractValue *const _var)
	{
		m_InputVariables.push_back(_var);
	}

	void ServiceImpl::registerOutputVariable(AbstractValue *const _var)
	{
		m_OutputVariables.push_back(_var);
	}

}