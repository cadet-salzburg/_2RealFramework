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
		std::cout << "private context: creating output container" << std::endl;
		ServicePtr service = m_FactoryPtr->createService("OutputContainer", "Framework").second;
		
		std::cout << "private context: unsafe cast" << std::endl;
		OutputContainerPtr result = service.unsafeCast< OutputContainer >();

		result->m_bIsConfigured = false;
		result->m_FrameworkPtr = this;

		if (result.isNull())
		{
			std::cout << "TODO: exception handling; Framework::newOutputContainer(); container ptr is null" << std::endl;
		}

		std::cout << "private context: returning result" << std::endl;
		return result;
	}

	NamedServicePtr Framework::createService(std::string const& _name, std::string const& _plugin)
	{
		std::cout << "framework: creating new service object" << std::endl;
		
		if (_plugin == "Framework")
		{
			NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);
			std::cout << "framework: created framework service: " << service.first << std::endl;
			service.second->setName(service.first);

			return service;
		}
		else 
		{
			if (m_FactoryPtr->canCreate(_name, _plugin))
			{
				NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);
				std::cout << "framework: created user service: " << service.first << std::endl;
				service.second->setName(service.first);
				NamedServicePtr container = m_FactoryPtr->createService("ServiceContainer", "Framework");
				std::cout << "framework: created service container: " << container.first << std::endl;
				container.second->setName(container.first);
			
				ServiceContainerPtr casted = container.second.unsafeCast< ServiceContainer >();
				casted->m_ServicePtr = service.second;

				return container;
			}
			else
			{
				std::cout << "framework: loading plugin" << std::endl;
				std::string path = "D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";
				PluginPtr pluginPtr = m_PluginsPtr->getPlugin(_plugin);
				
				if (pluginPtr.isNull())
				{
					std::cout << "framework: loading plugin dll" << std::endl;
					pluginPtr = PluginPtr(new Plugin(_plugin, path, *this));
					bool success = m_PluginsPtr->installPlugin(pluginPtr);
					if (success) std::cout << "juhu! " << std::endl;
				}

				if (pluginPtr->state() != Plugin::INVALID)
				{
					std::cout << "framework: not invalid!" << std::endl;
					NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);
					std::cout << "framework: created user service: " << service.first << std::endl;
					service.second->setName(service.first);
					NamedServicePtr container = m_FactoryPtr->createService("ServiceContainer", "Framework");
					std::cout << "framework: created service container: " << container.first << std::endl;
					container.second->setName(container.first);
			
					ServiceContainerPtr casted = container.second.unsafeCast< ServiceContainer >();
					casted->m_ServicePtr = service.second;

					std::cout << "framework: returning" << std::endl;
					return container;
				}
			}

			return NamedServicePtr();
		}
	}

	void Framework::registerService(std::string const& _name, std::string const& _plugin, ServiceCreator _func, bool const& _singleton)
	{
		PluginPtr pluginPtr = m_PluginsPtr->getPlugin(_plugin);
		
		if (pluginPtr.isNull())
		{
			std::cout << "TODO: exception handling; Framework::registerService(); plugin ptr is NULL" << std::endl;
		}
		else
		{
			m_FactoryPtr->registerService(_name, pluginPtr, _func, _singleton);
		}
	}

}