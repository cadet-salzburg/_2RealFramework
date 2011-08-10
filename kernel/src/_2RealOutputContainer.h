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
#include "_2RealConfigMetadata.h"

#include <stack>

namespace _2Real
{
	class OutputContainer : public AbstractContainer
	{

		friend class Framework;

	public:

		OutputContainer(ServiceName const& _name) : AbstractContainer(_name) {}

		void beginConfiguration();
		const bool endConfiguration();
		
		void beginSequence();
		void beginSynchronization();
		const bool endGroup();
		
		void beginServiceConfiguration(std::string const& _name, std::string const& _plugin);
		const bool endServiceConfiguration();

		template< typename T >
		void configureSetupParameter(std::string const& _name, T const& value)
		{
			ConfigMetadataPtr test = (m_CurrentConfiguration->child("ImageProcessing.RandomImage_ushort.1")).unsafeCast< ConfigMetadata >();
			if (test.isNull())
			{
				std::cout << "test is null" << std::endl;
			}

			std::cout << _name << std::endl;
			test->setSetupParameter< T >(_name, value);
			//m_CurrentConfiguration->setSetupParameter< T >(_name, value);
		}

		const Variable configureOutputParameter(std::string const& _name);
		void configureInputParameter(std::string const& _name, Variable const& _var);

		const Variable generateName(std::string const& _name) const;

		void start(bool const& _loop) {}
		void stop() {}
		void run() {}
		const bool setup(ConfigMetadataPtr const& _config) { return true; }
		void update() {}
		void shutdown() {}
		void addListener(ServicePtr &_listener) {}
		void removeListener(ServicePtr &_listener) {}
		void serviceListener(DataPtr &_input) {}

	private:

		typedef std::stack< GroupContainerPtr >	ContainerStack;
		typedef std::stack< ConfigMetadataPtr >	ConfigurationStack;

		Framework								*m_FrameworkPtr;

		ContainerStack							m_GroupContainers;
		ConfigurationStack						m_Configurations;

		GroupContainerPtr						m_TopLevelGroup;
		ConfigMetadataPtr						m_ConfigurationPtr;
		
		GroupContainerPtr						m_CurrentGroup;
		ConfigMetadataPtr						m_CurrentConfiguration;
		
		ServiceContainerPtr						m_CurrentService;
		
		bool									m_bIsConfigured;
		unsigned int							m_iVariableCounter;

	};
}