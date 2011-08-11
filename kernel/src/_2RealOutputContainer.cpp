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

#include "_2RealOutputContainer.h"

#include "_2RealGroupContainer.h"
#include "_2RealServiceContainer.h"
#include "_2RealFramework.h"

#include "_2RealAbstractContainer.h"
#include "_2RealConfigData.h"

namespace _2Real
{
////
////	const Variable OutputContainer::generateName(std::string const& _name)
////	{
////#ifdef _DEBUG
////		return m_CurrentService->serviceName() + "." + _name;
////#else
////		return ++m_iVariableCounter;
////#endif
////	}
////
////	const bool OutputContainer::beginConfiguration()
////	{
////		if (m_bIsConfigured)
////		{
////			std::cout << "TODO: error message" << std::endl;
////			return;
////		}
////
////		m_ConfigurationPtr = ConfigDataPtr(new ConfigData(m_ServiceName));
////		m_CurrentConfiguration = m_ConfigurationPtr;
////		m_iVariableCounter = 0;
////	}
////
////	const bool OutputContainer::beginConfiguration()
////	{
////		if (m_bIsConfigured)
////		{
////			std::cout << "TODO: error message" << std::endl;
////			return;
////		}
////
////		m_ConfigurationPtr = ConfigDataPtr(new ConfigData(m_ServiceName));
////		m_CurrentConfiguration = m_ConfigurationPtr;
////		m_iVariableCounter = 0;
////	}
////
////	const bool OutputContainer::endConfiguration()
////	{
////		if (m_bIsConfigured)
////		{
////			std::cout << "TODO: error message" << std::endl;
////			return false;
////		}
////
////		m_bIsConfigured = true;
////		return true;
////	}
////
////	void OutputContainer::beginSequence()
////	{
////		std::cout << "output container: ssequence group requested" << std::endl;
////
////		if (m_bIsConfigured)
////		{
////			std::cout << "TODO: error message, OutputContainer::beginSequence" << std::endl;
////			return;
////		}
////
////		if (!m_CurrentService.isNull())
////		{
////			std::cout << "TODO: error message, OutputContainer::beginSequence" << std::endl;
////			return;
////		}
////
////		GroupContainerPtr previousGroup = m_CurrentGroup;
////		
////		m_CurrentGroup = m_FrameworkPtr->createSequenceContainer();
////		
////		std::cout << "output container: successfully created new group container" << std::endl;
////		
////		if (!previousGroup.isNull())
////		{
////			std::cout << "output container: adding to previous group" << std::endl;
////
////			m_GroupContainers.push(previousGroup);
////			
////			previousGroup->addElement(m_CurrentGroup);
////		}
////		else
////		{
////			std::cout << "output container: saving top level group" << std::endl;
////
////			m_TopLevelGroup = m_CurrentGroup;
////		}
////
////		std::cout << "output container: creating metadata" << std::endl;
////
////		MetadataPtr newConfig = MetadataPtr(new ConfigData(m_CurrentGroup->name()));
////
////		m_CurrentConfiguration->insert(newConfig);
////		
////		m_CurrentConfiguration = newConfig.unsafeCast< ConfigData >();
////
////		std::cout << "output container: returning" << std::endl;
////
////	}
////
////	void OutputContainer::beginSynchronization()
////	{
////		std::cout << "output container: synchronization group requested" << std::endl;
////
////		if (m_bIsConfigured)
////		{
////			std::cout << "TODO: error message, OutputContainer::beginSynchronization" << std::endl;
////			return;
////		}
////
////		if (!m_CurrentService.isNull())
////		{
////			std::cout << "TODO: error message, OutputContainer::beginSynchronization" << std::endl;
////			return;
////		}
////
////		GroupContainerPtr previousGroup = m_CurrentGroup;
////		
////		m_CurrentGroup = m_FrameworkPtr->createSynchronizationContainer();
////
////		std::cout << "output container: successfully created new group container" << std::endl;
////
////		if (!previousGroup.isNull())
////		{
////			std::cout << "output container: adding to previous group" << std::endl;
////
////			m_GroupContainers.push(previousGroup);
////			
////			previousGroup->addElement(m_CurrentGroup);
////		}
////		else
////		{
////			std::cout << "output container: saving top level group" << std::endl;
////
////			m_TopLevelGroup = m_CurrentGroup;
////		}
////
////		MetadataPtr newConfig = MetadataPtr(new ConfigData(m_CurrentGroup->name()));
////		m_CurrentConfiguration->insert(newConfig);
////		m_CurrentConfiguration = newConfig.unsafeCast< ConfigData >();
////	}
////
////	const bool OutputContainer::endGroup()
////	{
////		if (m_bIsConfigured)
////		{
////			return false;
////		}
////
////		if (!m_CurrentService.isNull())
////		{
////			std::cout << "cannot begin new configuration" << std::endl;
////			return false;
////		}
////
////		if (!m_GroupContainers.empty())
////		{
////			m_CurrentGroup = m_GroupContainers.top();
////			m_GroupContainers.pop();
////
////			MetadataPtr oldConfig = m_CurrentConfiguration->father();
////			m_CurrentConfiguration = oldConfig.unsafeCast< ConfigData >();
////		}
////
////		return true;
////	}
////
////	void OutputContainer::beginServiceConfiguration(std::string const& _name, std::string const& _plugin)
////	{
////		if (m_bIsConfigured)
////		{
////			std::cout << "TODO: error message, OutputContainer::beginServiceConfiguration" << std::endl;
////			return;
////		}
////
////		if (!m_CurrentService.isNull())
////		{
////			std::cout << "TODO: error message, OutputContainer::beginServiceConfiguration" << std::endl;
////			return;
////		}
////
////		std::cout << "output container: request user service creation" << std::endl;
////		ServicePtr service = m_FrameworkPtr->createService(_name, _plugin).second;
////		std::cout << "output container: user service created" << std::endl;
////		
////		if (!service.isNull())
////		{
////			std::cout << "output container: attempt unsafe cast" << std::endl;
////			m_CurrentService = service.unsafeCast< ServiceContainer >();
////			
////			std::cout << "output container: create metadata" << std::endl;
////			MetadataPtr newConfig = MetadataPtr(new ConfigData(m_CurrentService->name()));
////			m_CurrentConfiguration->insert(newConfig);
////			m_CurrentConfiguration = newConfig.unsafeCast< ConfigData >();
////
////			MetadataPtr serviceConfig = MetadataPtr(new ConfigData(m_CurrentService->serviceName()));
////			m_CurrentConfiguration->insert(serviceConfig);
////			//m_CurrentConfiguration = serviceConfig.unsafeCast< ConfigData >();
////		}
////		else
////		{
////			std::cout << "output container: user service is null" << std::endl;
////			std::cout << "TODO: error message, OutputContainer::beginServiceConfiguration" << std::endl;
////		}
////	}
////
////	const bool OutputContainer::endServiceConfiguration()
////	{
////
////		std::cout << "attempting config of current service now" << std::endl;
////
////		m_CurrentService->setup(m_CurrentConfiguration);
////
////		m_CurrentService->update();
////
////		m_CurrentService.assign(NULL);
////		MetadataPtr oldConfig = m_CurrentConfiguration->father()->father();
////		m_CurrentConfiguration = oldConfig.unsafeCast< ConfigData >();
////
////		return true;
////	}
////
////	const Variable OutputContainer::configureOutputParameter(std::string const& _name)
////	{
////		Variable var = generateName(_name);
////		m_CurrentConfiguration->setOutputParameter< Variable >(_name, var);
////		return var;
////	}
////
////	void OutputContainer::configureInputParameter(std::string const& _name, Variable const& _var)
////	{
////		m_CurrentConfiguration->setInputParameter< Variable >(_name, _var);
////	}
}