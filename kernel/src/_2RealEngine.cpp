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

#include "_2RealEngine.h"
#include "_2RealEngineImpl.h"
#include "_2RealException.h"
#include "_2RealIdentifier.h"

namespace _2Real
{

	Engine::Engine() : m_Impl(NULL)
	{
#ifdef _DEBUG
		std::cout << "engine ctor called" << std::endl;
#endif

		m_Impl = EngineImpl::instance();
	}

	Engine::Engine(Engine const& _src)
	{
#ifdef _DEBUG
		std::cout << "engine copy ctor called" << std::endl;
#endif

		//increases ref count
		m_Impl = EngineImpl::instance();
	}
	
	Engine& Engine::operator=(Engine const& _src)
	{
#ifdef _DEBUG
		std::cout << "engine assignment operator called" << std::endl;
#endif

		if (this == &_src)
		{
			return *this;
		}

		if (m_Impl == NULL)
		{
			//increases ref count
			m_Impl = EngineImpl::instance();
		}

		return *this;
	}

	Engine::~Engine()
	{
#ifdef _DEBUG
		std::cout << "engine dtor called" << std::endl;
#endif

		m_Impl->release();
		m_Impl = NULL;
	}

	const Identifier Engine::installPlugin(std::string const& _name, std::string const& _path, std::string const& _class, Identifiers &_serviceIDs) throw(...)
	{
		return m_Impl->installPlugin(_name, _path, _name, _serviceIDs);
	}

	//const Exception Engine::dumpPluginInfo(Identifier const& _pluginID)
	//{
	//	return m_Impl->dumpPluginInfo(_pluginID);
	//}

	//const Exception Engine::dumpServiceInfo(Identifier const& _serviceID)
	//{
	//	return m_Impl->dumpPluginInfo(_serviceID);
	//}

	//const Exception Engine::createProductionGraph(std::string const& _containerName, Identifier &_containerID, eContainerType const& _type)
	//{
	//	return m_Impl->createProductionGraph(_containerName, _containerID, _type);
	//}

	//const Exception Engine::createService(Identifier const& _serviceID, std::string const& _containerName, Identifier &_containerID)
	//{
	//	return m_Impl->createService(_serviceID, _containerName, _containerID);
	//}

	//const Exception Engine::createMutex(Identifier const& _containerID, unsigned int const& _count, std::vector< Identifier > &_mutexIDs)
	//{
	//	return m_Impl->createMutex(_containerID, _count, _mutexIDs);
	//}

	//const Exception Engine::registerToExceptionChange(Identifier const& _containerID, ExceptionCallback _callback)
	//{
	//	return m_Impl->registerToExceptionChange(_containerID, _callback);
	//}

	//const Exception Engine::registerToNewDataAvailable(Identifier const& _containerID, NewDataCallback _callback)
	//{
	//	return m_Impl->registerToNewDataAvailable(_containerID, _callback);
	//}

	//const Exception Engine::insertInto(Identifier const& _containerIDDst, Identifier const& _containerIDSrc)
	//{
	//	return m_Impl->insertInto(_containerIDDst, _containerIDSrc);
	//}

	//const Exception Engine::startAll()
	//{
	//	return m_Impl->startAll();
	//}

	//const Exception Engine::pauseAll()
	//{
	//	return m_Impl->pauseAll();
	//}

}