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

#include "_2RealInternalService.h"

#include "_2RealIUserService.h"
#include "_2RealConfigMetadata.h"
#include "_2RealAbstractServiceVariable.h"
#include "_2RealServiceContext.h"

#include "Poco/Delegate.h"

namespace _2Real
{
	
	InternalService::InternalService(ServiceName const& _name, UserServicePtr &_service) : m_bIsConfigured(false), m_ServiceName(_name), m_UserServicePtr(_service)
	{
	}

	void InternalService::update()
	{
		m_UserServicePtr->update();
	}

	void InternalService::shutdown()
	{
		m_UserServicePtr->shutdown();
	}

	ServiceName const& InternalService::name() const
	{
		return m_ServiceName;
	}

	void InternalService::addInputVariable(AbstractServiceVariable *_var)
	{
		m_InputVariables.push_back(_var);
	}

	void InternalService::addOutputVariable(AbstractServiceVariable *_var)
	{
		m_OutputVariables.push_back(_var);
	}

	bool InternalService::getSetupParameter(AbstractServiceVariable *_param)
	{

		if (m_ConfigPtr.isNull())
		{
			std::cout << "TODO: error handling; InternalService: - config is null" << std::endl;
			return false;
		}

		bool success = true;
		Variable frameworkName;
			
		success &= m_ConfigPtr->setupParameter< Variable >(_param->originalName(), frameworkName);
		if (!success)
		{
			std::cout << "TODO: error handling; InternalService: error on setup params" << std::endl;
			return false;
		}
			
		_param->setFrameworkName(frameworkName);

		success &= _param->getFrom(m_ConfigPtr->setupAttributes());
		if (!success)
		{
			std::cout << "TODO: error handling; InternalService: error on setup params" << std::endl;
			return false;
		}

		return true;
	}

	const bool InternalService::setup(ConfigMetadataPtr const& _config)
	{
		if (m_bIsConfigured)
		{
			return false;
		}

		m_ConfigPtr = _config;
		
		m_UserServicePtr->setup(new ServiceContext(this));

		for (VariableList::iterator it = m_InputVariables.begin(); it != m_InputVariables.end(); it++)
		{
			Variable frameworkName;
			
			bool noError = _config->inputParameter< Variable >((*it)->originalName(), frameworkName);
			if (!noError)
			{
				std::cout << "TODO: error handling; InternalService: error on input vars" << std::endl;
				return noError;
			}
			
			(*it)->setFrameworkName(frameworkName);
		}

		for (VariableList::iterator it = m_OutputVariables.begin(); it != m_OutputVariables.end(); it++)
		{
			Variable frameworkName;

			bool noError = _config->outputParameter< Variable >((*it)->originalName(), frameworkName);
			if (!noError)
			{
				std::cout << "TODO: error handling; InternalService: error on output vars" << std::endl;
				return noError;
			}
			
			(*it)->setFrameworkName(frameworkName);
		}

		return m_bIsConfigured;
	}

	void InternalService::serviceListener(DataPtr &_input)
	{
		if (m_bIsConfigured && m_InputVariables.size() > 0)
		{
			for (VariableList::iterator it = m_InputVariables.begin(); it != m_InputVariables.end(); it++)
			{
				if ((*it)->getFrom(*_input.get()))
				{
					//std::cout << "received: " << (*it)->originalName() << " as " << (*it)->frameworkName() << std::endl;
				}
			}
		}
	}

	void InternalService::addListener(ServicePtr &_listener)
	{
		if (m_bIsConfigured)
		{
			m_OutputEvent += Poco::delegate(_listener.get(), &IService::serviceListener);
		}
	}


	void InternalService::removeListener(ServicePtr &_listener)
	{
		if (m_bIsConfigured)
		{
			m_OutputEvent -= Poco::delegate(_listener.get(), &IService::serviceListener);
		}
	}

	void InternalService::outputData(bool const& _blocking)
	{
		if (m_bIsConfigured && m_OutputVariables.size() > 0)
		{
			DataPtr output(new Data());
			for (VariableList::iterator it = m_OutputVariables.begin(); it != m_OutputVariables.end(); it++)
			{
				(*it)->insertInto(*output.get());
			}
			DataPtr result(output);

			if (_blocking)
			{
				m_OutputEvent.notify(this, result);
			}
			else
			{
				m_OutputEvent.notifyAsync(this, result);
			}
		}
	}
}