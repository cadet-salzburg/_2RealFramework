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

#include "_2RealAbstractContainer.h"
#include "_2RealServiceContainer.h"
#include "_2RealGroupContainer.h"
#include "_2RealConfigData.h"

#include <stack>

#include "Poco/BasicEvent.h"

namespace _2Real
{

	class OutputListener;

	class OutputContainer : public AbstractContainer
	{

		friend class Framework;
		friend class ServiceRegistry;

	public:

		OutputContainer(ServiceName const& _name);

		const bool beginConfiguration(eContainerType const& _type);
		const bool beginConfiguration();
		const bool endConfiguration();
		
		const bool beginGroup(eContainerType const& _type);
		const bool endGroup();
		
		const bool beginServiceConfiguration(std::string const& _name, std::string const& _plugin);
		const bool endServiceConfiguration();

		const bool configureSetupParameter(AbstractServiceVariable *const _param);
		const Variable configureOutputVariable(std::string const& _name);
		const bool configureInputVariable(std::string const& _name, Variable const& _var);

		const Variable generateName(std::string const& _name);

		const bool addOutputListener(OutputListener *const _listener);
		const bool removeOutputListener(OutputListener *const _listener);



		/**
		*	container stuff
		*/
		void start(bool const& _loop) {}
		void stop() {}
		void run() {}
		const bool setup(ConfigurationData *const _config) { return true; }
		void update() {}
		void shutdown() {}
		void addListener(ServicePtr &_listener) {}
		void removeListener(ServicePtr &_listener) {}
		void serviceListener(DataPtr &_input) {}

	private:

		typedef std::stack< GroupContainer * >	ContainerStack;
		typedef std::stack< ConfigurationData * >	ConfigurationStack;

		Poco::BasicEvent< DataPtr >				m_NewData;

		//ServiceFactory						&m_ServiceFactory;
		//Framework								*m_FrameworkPtr;

		ContainerStack							m_GroupContainers;
		ConfigurationStack						m_ServiceConfigurations;

		GroupContainer							*m_TopLevelContainer;
		ConfigurationData							*m_TopLevelConfiguration;
		
		GroupContainer							*m_CurrentGroup;
		ConfigurationData							*m_CurrentConfiguration;
		
		ServiceContainer						*m_CurrentUserService;
		
		bool									m_bIsConfigured;
		unsigned int							m_iVariableCounter;

	};
}