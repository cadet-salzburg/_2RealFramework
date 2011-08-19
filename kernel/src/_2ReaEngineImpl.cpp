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

#include "_2RealEngineImpl.h"

namespace _2Real
{
	EngineImpl::EngineImpl()
	{
		//m_FactoryPtr = new ServiceFactory();
		//m_PluginsPtr = new PluginPool();
	}

	EngineImpl::~EngineImpl()
	{
		//delete m_FactoryPtr;
		//delete m_PluginsPtr;
	}

	//OutputContainerPtr Engine::createOutputContainer()
	//{
	//	OutputContainerPtr result = m_FactoryPtr->createOutputContainer();

	//	//result->m_bIsConfigured = false;
	//	//result->m_EnginePtr = this;

	//	if (result.isNull())
	//	{
	//		std::cout << "TODO: exception handling; Engine::newOutputContainer(); container ptr is null" << std::endl;
	//	}

	//	return result;
	//}

	//GroupContainerPtr Engine::createSequenceContainer()
	//{
	//	GroupContainerPtr result = m_FactoryPtr->createSequenceContainer();

	//	if (result.isNull())
	//	{
	//		std::cout << "TODO: exception handling; Engine::newOutputContainer(); container ptr is null" << std::endl;
	//	}

	//	return result;
	//}

	//GroupContainerPtr Engine::createSynchronizationContainer()
	//{
	//	GroupContainerPtr result = m_FactoryPtr->createSynchronizationContainer();

	//	if (result.isNull())
	//	{
	//		std::cout << "TODO: exception handling; Engine::newOutputContainer(); container ptr is null" << std::endl;
	//	}

	//	return result;
	//}

	//NamedServicePtr Engine::createService(std::string const& _name, std::string const& _plugin)
	//{
	//	std::cout << "Engine: checking if service can be created" << std::endl;
	//	
	//	if (m_FactoryPtr->canCreate(_name, _plugin))
	//	{
	//		std::cout << "Engine: service can be created" << std::endl;
	//			
	//		//std::cout << "Engine: checking if service can be created" << std::endl;
	//		//NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);
	//		//service.second->setName(service.first);
	//		//NamedServicePtr container = m_FactoryPtr->createService("ServiceContainer", "Engine");
	//		//container.second->setName(container.first);
	//		
	//		//ServiceContainerPtr casted = container.second.unsafeCast< ServiceContainer >();
	//		//casted->m_ServicePtr = service.second;

	//		//return container;
	//	}
	//	else
	//	{
	//		std::cout << "Engine: service can not be created" << std::endl;
	//			
	//		std::string path = "D:\\cadet\\trunk\\_2RealEngine\\kernel\\testplugins\\bin\\";
	//		PluginPtr pluginPtr = m_PluginsPtr->getPlugin(_plugin);
	//			
	//		if (pluginPtr.isNull())
	//		{
	//			std::cout << "Engine: installing plugin" << std::endl;
	//				
	//			pluginPtr = PluginPtr(new Plugin(_plugin, path, *this));
	//			bool success = m_PluginsPtr->installPlugin(pluginPtr);
	//		}

	//		if (pluginPtr->state() != Plugin::INVALID)
	//		{
	//			std::cout << "attempting to create service now" << std::endl;
	//			
	//			NamedServicePtr service = m_FactoryPtr->createService(_name, _plugin);

	//			if (service.second.isNull())
	//			{
	//			}

	//			return service;

	//		}
	//	}

	//	return NamedServicePtr();
	//}

	//void Engine::registerService(std::string const& _name, std::string const& _plugin, UserServiceCreator _creator, bool const& _singleton)
	//{
	//	PluginPtr pluginPtr = m_PluginsPtr->getPlugin(_plugin);
	//	
	//	if (pluginPtr.isNull())
	//	{
	//		std::cout << "TODO: exception handling; Engine::registerService(); plugin ptr is NULL" << std::endl;
	//	}
	//	else
	//	{
	//		m_FactoryPtr->registerService(_name, pluginPtr, _creator, _singleton);
	//	}
	//}

	//ProductionTree *const Engine::createProductionTree()
	//{
	//	OutputContainer* result = m_FactoryPtr->createOutputContainer();
	//	
	//	//!!!!!!
	//	ProductionTree* config = new ProductionTree(result);
	//	return config;
	//}

}