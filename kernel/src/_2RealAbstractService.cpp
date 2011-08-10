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

#include "_2RealAbstractService.h"

#include "_2RealIUserService.h"
#include "_2RealConfigMetadata.h"
#include "_2RealAbstractServiceVariable.h"
#include "_2RealServiceContext.h"

#include "Poco/Delegate.h"

namespace _2Real
{
	
	AbstractService::AbstractService(ServiceName const& _name, IUserService *const _service) : m_bIsConfigured(false), m_ServiceName(_name), m_UserService(_service)
	{
	}

	void AbstractService::update()
	{
		m_UserService->update();
	}

	void AbstractService::shutdown()
	{
		m_UserService->shutdown();
	}

	ServiceName const& AbstractService::name() const
	{
		return m_ServiceName;
	}

	void AbstractService::addInputVariable(AbstractServiceVariable *_var)
	{
		m_InputVariables.push_back(_var);
	}

	void AbstractService::addOutputVariable(AbstractServiceVariable *_var)
	{
		m_OutputVariables.push_back(_var);
	}

	void AbstractService::addSetupParameter(AbstractServiceVariable *_param)
	{
		m_SetupParameters.push_back(_param);
	}

	const bool AbstractService::setup(ConfigMetadataPtr const& _config)
	{
		if (m_bIsConfigured)
		{
			return false;
		}

		m_UserService->setup(new ServiceContext(this));

		for (ParameterList::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
		{
			bool noError = (*it)->getFrom(_config->setupAttributes());
			if (!noError)
			{
				std::cout << "TODO: error handling; abstract service: error on input vars" << std::endl;
				return noError;
			}
		}

		for (VariableList::iterator it = m_InputVariables.begin(); it != m_InputVariables.end(); it++)
		{
			Variable frameworkName;
			
			bool noError = _config->inputParameter< Variable >((*it)->originalName(), frameworkName);
			if (!noError)
			{
				std::cout << "TODO: error handling; abstract service: error on input vars" << std::endl;
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
				std::cout << "TODO: error handling; abstract service: error on output vars" << std::endl;
				return noError;
			}
			
			(*it)->setFrameworkName(frameworkName);
		}

		m_bIsConfigured = m_UserService->init();

		return m_bIsConfigured;
	}

	void AbstractService::serviceListener(DataPtr &_input)
	{
		if (m_bIsConfigured && m_InputVariables.size() > 0)
		{
			for (VariableList::iterator it = m_InputVariables.begin(); it != m_InputVariables.end(); it++)
			{
				if ((*it)->getFrom(*_input.get()))
				{
					std::cout << "received: " << (*it)->originalName() << " as " << (*it)->frameworkName() << std::endl;
				}
			}
		}
	}

	void AbstractService::addListener(ServicePtr &_listener)
	{
		if (m_bIsConfigured)
		{
			m_OutputEvent += Poco::delegate(_listener.get(), &IService::serviceListener);
		}
	}


	void AbstractService::removeListener(ServicePtr &_listener)
	{
		if (m_bIsConfigured)
		{
			m_OutputEvent -= Poco::delegate(_listener.get(), &IService::serviceListener);
		}
	}

	void AbstractService::outputData(bool _blocking)
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