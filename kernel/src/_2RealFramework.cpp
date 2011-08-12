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

#include "_2RealFramework.h"

#include "_2RealIService.h"
#include "_2RealServiceContainer.h"
#include "_2RealOutputContainer.h"
#include "_2RealPluginPool.h"
#include "_2RealServiceFactory.h"
#include "_2RealProductionTree.h"

#include "Poco/Path.h"

namespace _2Real
{
	Framework::Framework()
	{
		m_FactoryPtr = new ServiceFactory();
		m_PluginsPtr = new PluginPool();
	}

	Framework::~Framework()
	{
		delete m_FactoryPtr;
		delete m_PluginsPtr;
	}

	OutputContainerPtr Framework::createOutputContainer()
	{
		OutputContainerPtr result = m_FactoryPtr->createOutputContainer();

		//result->m_bIsConfigured = false;
		//result->m_FrameworkPtr = this;

		if (result.isNull())
		{
			std::cout << "TODO: exception handling; Framework::newOutputContainer(); container ptr is null" << std::endl;
		}

		return result;
	}

	GroupContainerPtr Framework::createSequenceContainer()
	{
		GroupContainerPtr result = m_FactoryPtr->createSequenceContainer();

		if (result.isNull())
		{
			std::cout << "TODO: exception handling; Framework::newOutputContainer(); container ptr is null" << std::endl;
		}

		return result;
	}

	GroupContainerPtr Framework::createSynchronizationContainer()
	{
		GroupContainerPtr result = m_FactoryPtr->createSynchronizationContainer();

		if (result.isNull())
		{
			std::cout << "TODO: exception handling; Framework::newOutputContainer(); container ptr is null" << std::endl;
		}

		return result;
	}

	NamedServicePtr Framework::createService(std::string const& _name, std::string const& _plugin)
	{
		std::cout << "framework: checking if service can be created" << std::endl;
		
		if (m_FactoryPtr->canCreate(_name, _plugin))
		{
			std::cout << "framework: service can be created" << std::endl;
				
			//std::cout << "framework: checking if service can be created" << std::endl;
			//NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);
			//service.second->setName(service.first);
			//NamedServicePtr container = m_FactoryPtr->createService("ServiceContainer", "Framework");
			//container.second->setName(container.first);
			
			//ServiceContainerPtr casted = container.second.unsafeCast< ServiceContainer >();
			//casted->m_ServicePtr = service.second;

			//return container;
		}
		else
		{
			std::cout << "framework: service can not be created" << std::endl;
				
			std::string path = "D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";
			PluginPtr pluginPtr = m_PluginsPtr->getPlugin(_plugin);
				
			if (pluginPtr.isNull())
			{
				std::cout << "framework: installing plugin" << std::endl;
					
				pluginPtr = PluginPtr(new Plugin(_plugin, path, *this));
				bool success = m_PluginsPtr->installPlugin(pluginPtr);
			}

			if (pluginPtr->state() != Plugin::INVALID)
			{
				std::cout << "attempting to create service now" << std::endl;
				
				NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);

				if (service.second.isNull())
				{
				}

				return service;

			}
		}

		return NamedServicePtr();
	}

	void Framework::registerService(std::string const& _name, std::string const& _plugin, UserServiceCreator _creator, bool const& _singleton)
	{
		PluginPtr pluginPtr = m_PluginsPtr->getPlugin(_plugin);
		
		if (pluginPtr.isNull())
		{
			std::cout << "TODO: exception handling; Framework::registerService(); plugin ptr is NULL" << std::endl;
		}
		else
		{
			m_FactoryPtr->registerService(_name, pluginPtr, _creator, _singleton);
		}
	}

	ProductionTree *const Framework::createProductionTree()
	{
		OutputContainer* result = m_FactoryPtr->createOutputContainer();
		
		//!!!!!!
		ProductionTree* config = new ProductionTree(result);
		return config;
	}

}