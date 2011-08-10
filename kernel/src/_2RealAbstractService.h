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

#include "_2RealTypedefs.h"

#include "_2RealIService.h"
#include "_2RealIUserService.h"

#include <list>

#include "Poco/BasicEvent.h"

namespace _2Real
{

	class AbstractService : public IService
	{

	public:
	
		AbstractService(ServiceName const& _name, IUserService *const _service) : m_bIsConfigured(false), m_ServiceName(_name), m_UserService(_service) {}
		
		const bool setup(ConfigMetadataPtr const& _config);
		void update() { m_UserService->update(); }
		void shutdown() { m_UserService->shutdown(); }

		void addListener(ServicePtr &_listener);
		void removeListener(ServicePtr &_listener);
		void serviceListener(DataPtr &_input);
		void outputData(bool _blocking);
		
		ServiceName const& name() const { return m_ServiceName; }
		void setName(ServiceName const& _name) { m_ServiceName = _name; }

		template< typename T >
		void addInputVariable(Variable _name, T &_var)
		{
			if (m_bIsConfigured)
			{
				return;
			}

			ServiceVariable< T > *var = new ServiceVariable< T >(_name, _var);
			m_InputVariables.push_back(var);
		}

		template< typename T >
		void addOutputVariable(Variable _name, T &_var)
		{
			if (m_bIsConfigured)
			{
				return;
			}

			ServiceVariable< T > *var = new ServiceVariable< T >(_name, _var);
			m_OutputVariables.push_back(var);
		}

		template< typename T >
		void addSetupParameter(Variable _name, T &_param)
		{
			if (m_bIsConfigured)
			{
				return;
			}

			ServiceParameter< T > *param = new ServiceParameter< T >(_name, _param);
			m_SetupParameters.push_back(param);
		}

	private:

		typedef std::list< AbstractServiceVariable *> VariableList;
		typedef std::list< AbstractServiceVariable *> ParameterList;

		Poco::BasicEvent< DataPtr >					m_OutputEvent;

		VariableList								m_InputVariables;
		VariableList								m_OutputVariables;
		ParameterList								m_SetupParameters;

		bool										m_bIsConfigured;

		IUserService								*m_UserService;

		ServiceName									m_ServiceName;

	};
}