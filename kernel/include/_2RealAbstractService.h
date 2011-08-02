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

#include "_2RealFramework.h"
#include "_2RealIService.h"
#include "_2RealData.h"
#include "_2RealServiceVariable.h"
#include "_2RealServiceParameter.h"

#include "Poco/BasicEvent.h"

namespace _2Real
{

	class AbstractService : public IService
	{

	public:
	
		AbstractService() : m_bIsConfigured(false) {}
		
		/*
			Iservice functions that need to be implemented by the user service
		*/
		virtual bool setup(ConfigMetadataPtr const& _config) = 0;
		virtual void update() = 0;
		virtual void shutdown() = 0;

		/*
			Iservicefunctions  implemented by this service
		*/
		void addListener(ServicePtr _listener);
		void removeListener(ServicePtr _listener);
		void serviceListener(DataPtr &_input);
		void outputData(bool _blocking);
		
		//configuration function - needs to be called manually in the service's setup
		const bool configure(ConfigMetadataPtr const& _config);

		template< typename T >
		void addInputVariable(std::string _name, T &_var)
		{
			if (m_bIsConfigured)
			{
				return;
			}

			ServiceVariable< T > *var = new ServiceVariable< T >(_name, _var);
			m_InputVariables.push_back(var);
		}

		template< typename T >
		void addOutputVariable(std::string _name, T &_var)
		{
			if (m_bIsConfigured)
			{
				return;
			}

			ServiceVariable< T > *var = new ServiceVariable< T >(_name, _var);
			m_OutputVariables.push_back(var);
		}

		template< typename T >
		void addSetupParameter(std::string _name, T &_param)
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

	};
}