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

namespace _2Real
{

	const bool AbstractService::configure(ConfigMetadataPtr const& _config)
	{
		if (m_bIsConfigured)
		{
			return false;
		}

		m_ServiceName = _config->name();

		bool noError = true;

		/*
			those are the setup params;
			they can be gotten directly from the config metadata's setup attribs
		*/
		for (ParameterList::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
		{
			noError &= (*it)->getFrom(_config->setupAttributes());
		}

		if (!noError)
		{
			std::cout << "abstract service: error on setup params" << std::endl;
			return noError;
		}

		/*
			get the framework defined names for input variables
		*/
		for (VariableList::iterator it = m_InputVariables.begin(); it != m_InputVariables.end(); it++)
		{
			std::string frameworkName;
			_config->inputParameter<std::string>((*it)->originalName(), frameworkName);
			(*it)->setFrameworkName(frameworkName);
		}

		if (!noError)
		{
			std::cout << "abstract service: error on input vars" << std::endl;
			return noError;
		}

		/*
			get the framework defined names for output variables
		*/
		for (VariableList::iterator it = m_OutputVariables.begin(); it != m_OutputVariables.end(); it++)
		{
			std::string frameworkName;
			_config->outputParameter<std::string>((*it)->originalName(), frameworkName);
			(*it)->setFrameworkName(frameworkName);
		}

		if (!noError)
		{
			std::cout << "abstract service: error on output vars" << std::endl;
			return noError;
		}

		m_bIsConfigured = true;
		return true;
	}

	void AbstractService::inputListener(Data &_input)
	{
		if (m_bIsConfigured && m_InputVariables.size() > 0)
		{
			lock();
			for (VariableList::iterator it = m_InputVariables.begin(); it != m_InputVariables.end(); it++)
			{
				if ((*it)->getFrom(_input))
				{
					std::cout << m_ServiceName << " received: " << (*it)->originalName() << " as " << (*it)->frameworkName() << std::endl;
				}
			}
			unlock();
		}
	}

	void AbstractService::outputData()
	{
		if (m_bIsConfigured)
		{
			Data output;
			for (VariableList::iterator it = m_OutputVariables.begin(); it != m_OutputVariables.end(); it++)
			{
				(*it)->insertInto(output);
			}

			m_NewDataEvent.notify(this, output);
		}
	}

	void AbstractService::addListener(ServicePtr _listener)
	{
		if (m_bIsConfigured)
		{
			m_NewDataEvent += Poco::delegate(_listener.get(), &IService::inputListener);
		}
	}
}