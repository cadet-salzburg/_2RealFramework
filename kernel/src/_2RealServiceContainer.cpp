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

namespace _2Real
{

	ServiceContainer::ServiceContainer(ContainerName const& _name) : AbstractContainer(_name)
	{
	}

	ServiceContainer::ServiceContainer(ServiceContainer const& _src) : AbstractContainer(_src)
	{
#ifdef _DEBUG
		std::cout << "service container copy constructor called" << std::endl;
#endif
	}

	ServiceContainer& ServiceContainer::operator=(ServiceContainer const& _src)
	{
		AbstractContainer::operator=(_src);

#ifdef _DEBUG
		std::cout << "service container assignment operator called" << std::endl;
#endif
		
		return *this;
	}

	ServiceContainer::~ServiceContainer()
	{
		//NOTE-TO-SELF:
		//service ptr is not owning
		//services must be deleted by factory
	}

	void ServiceContainer::configure(ConfigurationData *const _dataPtr) throw(...)
	{
		if (!m_bIsConfigured || m_bCanReconfigure)
		{
			//TODO: throw
			return;
		}

		//TODO: set m_bCanReconfigure

		try
		{
			//call user service's setup method
			m_ServicePtr->setup(new ServiceContext(this));
		}
		catch (...)
		{
			//TODO: throw
			return;
		}

		//save configuration
		m_ConfigurationPtr = _dataPtr;
	}

	void ServiceContainer::run() throw(...)
	{
		while (m_bRun || m_bRunOnce)
		{
			try
			{
				//call user service's update method
				m_ServicePtr->update();
			}
			catch (...)
			{
				//TODO: throw
				return;
			}

			m_bRunOnce = false;
			sendData(false);
		}
	}

	void ServiceContainer::update() throw(...)
	{
		if (!m_bIsConfigured)
		{
			return;
		}

		try
		{
			m_ServicePtr->update();
		}
		catch (...)
		{
			return;
		}

		//output vars should have updated data now
		//blocking send -> function returns when listeners have been notified
		sendData(true);
	}

	void ServiceContainer::shutdown() throw(...)
	{
		try
		{
			m_ServicePtr->shutdown();
		}
		catch(...)
		{
		}
	}

	void ServiceContainer::getParameterValue(AbstractValue *const _param)
	{
		//get name of param from config
		//look up param value
		//set value
	}

	void ServiceContainer::registerInputVariable(AbstractValue *const _var)
	{
		m_InputVariables.push_back(_var);
	}

	void ServiceContainer::registerOutputVariable(AbstractValue *const _var)
	{
		m_OutputVariables.push_back(_var);
	}

}