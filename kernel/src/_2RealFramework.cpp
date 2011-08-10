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
		ServicePtr service = m_FactoryPtr->createService("OutputContainer", "Framework").second;
		
		OutputContainerPtr result = service.unsafeCast< OutputContainer >();

		result->m_bIsConfigured = false;
		result->m_FrameworkPtr = this;

		if (result.isNull())
		{
			std::cout << "TODO: exception handling; Framework::newOutputContainer(); container ptr is null" << std::endl;
		}

		return result;
	}

	NamedServicePtr Framework::createService(std::string const& _name, std::string const& _plugin)
	{

		if (_plugin == "Framework")
		{
			NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);
			//service.second->setName(service.first);

			return service;
		}
		else 
		{
			if (m_FactoryPtr->canCreate(_name, _plugin))
			{
				NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);
				//service.second->setName(service.first);
				NamedServicePtr container = m_FactoryPtr->createService("ServiceContainer", "Framework");
				//container.second->setName(container.first);
			
				ServiceContainerPtr casted = container.second.unsafeCast< ServiceContainer >();
				casted->m_ServicePtr = service.second;

				return container;
			}
			else
			{
				std::string path = "D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";
				PluginPtr pluginPtr = m_PluginsPtr->getPlugin(_plugin);
				
				if (pluginPtr.isNull())
				{
					pluginPtr = PluginPtr(new Plugin(_plugin, path, *this));
					bool success = m_PluginsPtr->installPlugin(pluginPtr);
				}

				if (pluginPtr->state() != Plugin::INVALID)
				{
					NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);
					//service.second->setName(service.first);
					NamedServicePtr container = m_FactoryPtr->createService("ServiceContainer", "Framework");
					//container.second->setName(container.first);
			
					ServiceContainerPtr casted = container.second.unsafeCast< ServiceContainer >();
					casted->m_ServicePtr = service.second;

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