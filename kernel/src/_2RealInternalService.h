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

#include "_2RealIUserService.h"
#include "_2RealIService.h"
#include "_2RealData.h"
#include "_2RealConfigMetadata.h"

#include <list>

#include "Poco/BasicEvent.h"

namespace _2Real
{

	class InternalService : public IService
	{

	public:
	
		InternalService(ServiceName const& _name, UserServicePtr &_service);
		
		const bool setup(ConfigMetadataPtr const& _config);
		void update();
		void shutdown();

		void addListener(ServicePtr &_listener);
		void removeListener(ServicePtr &_listener);
		void serviceListener(DataPtr &_input);
		void outputData(bool const& _blocking);
		
		ServiceName const& name() const;

		void addInputVariable(AbstractServiceVariable *const _var);
		void addOutputVariable(AbstractServiceVariable *const _var);
		bool getSetupParameter(AbstractServiceVariable *const _param);

	private:

		typedef std::list< AbstractServiceVariable *> VariableList;
		typedef std::list< AbstractServiceVariable *> ParameterList;

		Poco::BasicEvent< DataPtr >			m_OutputEvent;

		VariableList						m_InputVariables;
		VariableList						m_OutputVariables;
		ParameterList						m_SetupParameters;

		ConfigMetadataPtr					m_ConfigPtr;

		bool								m_bIsConfigured;
		ServiceName							m_ServiceName;
		UserServicePtr						m_UserServicePtr;

	};
}